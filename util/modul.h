
#ifndef _MODUL_H_
#define _MODUL_H_

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
//#include <fstream>
using namespace std;

#include "code.h"
#include "BitArray.h"
#include "randomVar.h"
#include "tempFun.h" //just for debug to showSyb

typedef char Symbol;

class Modul
{
protected:
   const size_t syb_per_bit; //static
   Code* _code;
public:
   Modul(const size_t spb):syb_per_bit(spb),_code(0) {}
   virtual ~Modul() { reset(); }
   void reset() {
      if(_code!=0) delete _code;
      _code = 0;
   }
   virtual Symbol demodulate(const double& ry,const double& rx) = 0;
   virtual void simSyb(const Symbol& s,Symbol& r) = 0;

   template< class T>
   void separation(vector<Symbol>& s, T ss) {
         s.clear();
         for(size_t i = 0, size = ceil(sizeof(T)*8/(double)syb_per_bit); i<size; ++i) {
            s.push_back( ss & ((1<<syb_per_bit)-1) );
            ss = ss >> syb_per_bit;
         }
   }

   template<class T> // T must apply operator &,|,>>,<<
   void simT (T ss, T& rr ) {
         rr &= '\0';
         vector<Symbol> s;
	 separation(s,ss);
	 vector<Symbol> r(s.size(),0);
         for(size_t i =0, size = s.size(); i < size; ++i) {
	    simSyb(s[size-i-1],r[size-i-1]);
//            cerr<<"======================\n";
//	    showSyb(s[size-i-1]);
//	    showSyb(r[size-i-1]);
	    rr = rr << syb_per_bit;
	    rr |= r[size-i-1];
	 }
   }

   double rate(const unsigned& maxErr, const int& rateType) {// -1 means BitER, -2 means SybER, n>=0 means Bit(n)Er
      srand(time(NULL));
      //init(SNR_dB);
      unsigned err = 0, runSyb = 0;

      while( err < maxErr) {
         //cerr<<runSyb<<"\n";
         unsigned ssss = rand();
         vector<Symbol> s;
	 separation(s,ssss);
         for(size_t i =0, size = s.size(); i < size; ++i)  {
	    //showSyb(s[i]);

	    Symbol r;
	    simSyb(s[i],r);
	    //showSyb(s[i]);
	    //showSyb(r);
	    Symbol xorSyb = s[i]^r;
	    if(rateType==-2) {
	       if(xorSyb!=0) ++err;
	    }
	    else if(rateType==-1) {
	       for(size_t j = 0; j<syb_per_bit; ++j) {
	          Symbol token = (1<<j);
	          if((xorSyb & token) !=0) ++err;
	       }
	    }
	    else {
	       Symbol token = (1<<rateType);
	       if((xorSyb & token) !=0) ++err;
	    }
	    ++runSyb;
	 }
	 //cerr<<(double)err/runSyb*(syb_per_bit)<<"\n";
      }
      double _errRate = err/(double)runSyb;
      return (rateType==-1)? (_errRate/syb_per_bit) : _errRate;
   }

   void setCode(const unsigned& initNum) {
       _code = new Code(initNum);
#define gNum 2 
      const unsigned g[gNum][4] = { {5, 2, 7, 7} , {7, 1, 7, 5} };
      for(size_t i = 0; i<gNum; ++i) 
         _code->setGenerator(g[i]);
      _code->weightTable();
   }

   void freeWeightTable() {
      _code->freeWeightTable();
   }

   double rate_code(const unsigned& maxErr, const int& rateType) {// -1 means BitER, -2 means SybER, n>=0 means Bit(n)Er
      srand(time(NULL));
      unsigned err = 0, runSyb = 0;
      unsigned initNum = 2;
      setCode(initNum);
      while( err < maxErr) {
         //cerr<<runSyb<<"\n";
         BitArray s120;
	 s120.init(120 + initNum);
	 for(size_t i = 0; i<120; i += 8) {
            char ss = rand();
	    s120.setChar(i,ss);
	 }
	 BitArray r120;
	 simuPacket_code(s120,r120);

	 for(size_t t=0, time = s120.size(); t<time; ++t) {
	    if(s120.get(t)!=r120.get(t)) err++;
	 }
	 runSyb = runSyb + 120;
      }
      freeWeightTable();
      return err/(double)runSyb;
   }

   void simuPacket_code(const BitArray& s120, BitArray& r120){
	 //s120.show();
	 vector<BitArray> r_code;
	 _code->encode(s120, r_code);
	 for(size_t i = 0, s = r_code.size(); i<s; ++i) {
	    for(size_t j = 0,t = s120.size(); j<t; j+=8) {
	       char recieve = 0;
               simT(r_code[i].getChar(j),recieve);
	       r_code[i].setChar(j,recieve);
	    }
	    //r_code[i].show();
	 }
	 _code->decode(r_code, r120);
   }



};


class QAM16 :public Modul
{

private:
   double N0,d;

public:
   QAM16(const double& n = 2, const double& snr_db = 0):Modul(4), N0(n) { init(snr_db); }
   virtual ~QAM16() {}
   void init(const double& SNR_dB) {
      d = sqrt(1.6*pow(10,SNR_dB/10)*N0);
   }

/****************************************
   16 QAM Table: Symbol(index)
   0010(12) 0110(13) | 1110(14) 1010(15)
   0011(8)  0111(9)  | 1111(10) 1011(11)
   ------------------+------------------
   0001(4)  0101(5)  | 1101(8)  1001(7)
   0000(0)  0100(1)  | 1100(2)  1000(3)
 ****************************************/

   Symbol 
   demodulate(const double& ry,const double& rx) {
      Symbol r = '\0';
      if (ry > d) r += 2;
      else if (ry > 0) r += 3;
      else if (ry > (-1)*d) r += 1;
      else;
      if (rx > d) r += 8;
      else if (rx > 0) r += 12;
      else if (rx > (-1)*d) r += 4;
      else;
      return r;
   }      

   void 
   simSyb(const Symbol& s,Symbol& r) {
      //modulate
      static double gray_map[4] = {-1.5, -0.5, 1.5, 0.5};
      double sy = gray_map[s&3]*d;
      double sx = gray_map[(s>>2)&3]*d;
      //AWGN channel
      double ny = 0, nx = 0;
      static GaussianVar rv(0,sqrt(N0/2));
      rv.get2Var(ny,nx);
      r = demodulate(sy + ny ,sx + nx);
   }
/*
   virtual void compute(Symbol s, Symbol r);

   void sim_1point(unsigned maxErr,double SNR_dB);

   void sim(size_t maxSNR_dB, char* s);
   void print(double SNR_dB);
*/

};

class PSK8 :public Modul
{

private:
   double N0,d;

public:
   PSK8(const double& n = 2, const double& snr_db = 0):Modul(3), N0(n) { init(snr_db); }
   virtual ~PSK8() {}
   void init(const double& SNR_dB) {
      d = sqrt(3*pow(10,SNR_dB/10)*N0);
   }

/****************************************
   8 PSK Table: Symbol(index)
                   011(3)
            010(2)   |   001(1)
   ---110(6)---------+---------000(0)----
            111(7)   |   100(4)
                   101(5)
 ****************************************/
   
   Symbol 
   demodulate(const double& ry,const double& rx) {
      Symbol r = '\0';
      double slope = ry/rx;
      if (slope > tan(3*M_PI/8)) {
         if(rx>0) r += 3;
	 else r += 5;
      }
      else if (slope > tan(M_PI/8)) {
         if(rx>0) r += 1;
	 else r += 7;
      }
      else if (slope > tan(-1*M_PI/8)) {
         if(rx>0) r += 0;
	 else r += 6;
      }
      else if (slope > tan(-3*M_PI/8)) {
         if(rx>0) r += 4;
	 else r += 2;
      }
      else {
         if(rx>0) r += 5;
	 else r += 3;
      }
      return r;
   }      

   void 
   simSyb(const Symbol& s,Symbol& r) {
      //modulate
      static double gray_map[] = {0,1,3,2,7,6,4,5};
      double sy = sin(gray_map[s&7]*M_PI/4)*d;
      double sx = cos(gray_map[s&7]*M_PI/4)*d;
      //AWGN channel
      double ny = 0, nx = 0;
      static GaussianVar rv(0,sqrt(N0/2));
      rv.get2Var(ny,nx);
      r = demodulate(sy + ny ,sx + nx);
 //     compute(s,r);
   }

};


class BPSK :public Modul
{

private:
   double N0,d;

public:
   BPSK(const double& n = 2, const double& snr_db = 0):Modul(1), N0(n) { init(snr_db); }
   virtual ~BPSK() {}
   void init(const double& SNR_dB) {
//   cerr<<SNR_dB;
      d = sqrt(pow(10,SNR_dB/10)*N0);
   }

/****************************************
   2 PSK Table: Symbol(index)
                     |
                     | 
   -----1(1)---------+---------0(0)------
                     |
                     |
 ****************************************/

   Symbol 
   demodulate(const double& ry, const double& rx) {
      Symbol r = '\0';
      r = r - r;
      if (rx < 0) r |= 1;
      return r;
   }      

   void 
   simSyb(const Symbol& s,Symbol& r) {
      //modulate
//      cerr<<"s is "<<s<<" ";
      static double gray_map[] = {1,-1};
      double sx = gray_map[(s&1)]*d;
      //AWGN channel
      double ny = 0, nx = 0;
      static GaussianVar rv(0,sqrt(N0/2));
      rv.get2Var(ny,nx);
      r = demodulate(ny,sx + nx);
 //     compute(r,s);
   }
};
#endif
