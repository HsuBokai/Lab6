
#ifndef _ERRRATE_H_
#define _ERRRATE_H_


#include "modul.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include "randomVar.h"
using namespace std;

#include "tempFun.h"

typedef char symbol;

class ErrRate
{
private:
   const size_t syb_per_bit; //static
public:
   ErrRate(const size_t spb):syb_per_bit(spb) {}
   virtual ~ErrRate() {}
   virtual symbol demodulate(const double ry,const double rx) = 0;
   virtual void simSyb(const symbol& s,symbol& r) = 0;


   symbol 
   QAM16::demodulate(const double ry,const double rx) {
      symbol r = '\0';
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
   QAM16::simSyb(const symbol& s,symbol& r) {
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

   virtual void
   QAM16::compute(symbol s, symbol r, unsigned& err, unsigned& runSyb) {
      if(r != s) ++errSyb;
      if(r%2!=s%2) ++errBit0;
      r = r/2;
      s = s/2;
      if(r%2!=s%2) ++errBit1;
      r = r/2;
      s = s/2;
      if(r%2!=s%2) ++errBit2;
      r = r/2;
      s = s/2;
      if(r%2!=s%2) ++errBit3;
      errBit = errBit0 + errBit1 + errBit2 + errBit3;
      ++runSyb;
      _errRate = err/runSyb;
   }

   void 
   QAM16::sim_1point(unsigned maxErr,double SNR_dB, int rateType) {
      srand(time(NULL));
      init(SNR_dB);
      unsigned err = 0, runSyb = 0;
      while( err < maxErr) {
         unsigned ssss = rand();
         vector<symbol> s;
         while(ssss>0) {
            s.push_back(ssss%(1<<4));
            ssss = ssss >> 4;
         }
	 vector<symbol> r(s.size(),0);
         for(size_t i =0, size = s.size(); i < size; ++i)  {
	    simSyb(s[i],r[i]);
	    ++runSyb;
	    if(rateType==-2) {
	       if(s[i]!=r[i]) ++err;
	       _errRate = err/runSyb;
	    }
	    else if(rateType==-1) {
	       
	       if()
	    }
	    else {
	       token = (1<<rateTypt);
	    }
	 }
      }
      print(SNR_dB);
   }

   void 
   QAM16::sim(size_t maxSNR_dB, char* s) {
      ofstream _ofile;
      _ofile.open(s);
      if(!_ofile.is_open()) return;
      for(size_t i = 0,s = maxSNR_dB; i<s; ++i) {
         if(i<10) sim_1point(1000,i);
	 else sim_1point(100,i);
	 _ofile<<i<<"\t";
	 _ofile<<errSyb/(double)runSyb<<"\t";
	 _ofile<<errBit0/(double)runSyb<<"\t";
	 _ofile<<errBit1/(double)runSyb<<"\t";
	 _ofile<<errBit2/(double)runSyb<<"\t";
	 _ofile<<errBit3/(double)runSyb<<"\t";
	 _ofile<<errBit/(double)runSyb/4<<"\n";
      }
      _ofile.close();
   }
   void 
   QAM16::print(double SNR_dB) {
      cerr<<"SNR_dB = "<<SNR_dB<<"\n";
      cerr<<"Eb = "<<d*d/0.4<<"\n";
      cerr<<"N0 = "<<N0<<"\n";
      cerr<<"d = "<<d<<"\n";
      cerr<<"runSyb = "<<runSyb<<"\n";
      cerr<<"SER = "<<errSyb<<"\n";
      cerr<<"BER0 = "<<errBit0<<"\n";
      cerr<<"BER1 = "<<errBit1<<"\n";
      cerr<<"BER2 = "<<errBit2<<"\n";
      cerr<<"BER3 = "<<errBit3<<"\n";
      cerr<<"BER = "<<errBit<<"\n";
   }
};
#endif //ErrRate.h
