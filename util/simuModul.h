#ifndef _SIMUMODUL_H_
#define _SIMUMODUL_H_

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "modul.h"
//#include "code.h"
class BitArray;
// gif 999up
enum InputType { jpg = 40000, bmp = 54, eps = 220400, gif = 260000 };

class SimuModul
{

private:
   Modul* _modul;
   ifstream _ifile;
   ofstream _ofile;
public:
   SimuModul(const size_t& num = 16, const double& snr = 10):_modul(0) { init(num,snr); }

   void init(const size_t& num, const double& snr) {
       reset();
       if(num==16) _modul = new QAM16(2,snr);
       else if(num==8) _modul = new PSK8(2,snr);
       else _modul = new BPSK(2,snr);
   }

   bool openInput(char* fiName) { 
      _ifile.open(fiName, ios::binary);
      return _ifile.is_open(); 
   }

   bool openOutput(char* foName) { 
      _ofile.open(foName, ios::binary);
      return _ofile.is_open(); 
   }

   ~SimuModul() { 
      reset(); 
      _ifile.close();
      _ofile.close();
   }
   void reset() {
      if(_modul!=0) delete _modul;
      _modul = 0;
   }

   double rate_code(const unsigned& maxErr, const int& rateType) {
      return _modul->rate_code(maxErr,rateType);
   }

   double rate(const unsigned& maxErr, const int& rateType) {
      return _modul->rate(maxErr,rateType);
   }
   void graph(const InputType it, const bool& isCode) {
      if(isCode==1) return graph_code(it);
      const unsigned headerSize = (unsigned)it;
      char* header = new char[headerSize];
      _ifile.read(header,sizeof(char)*headerSize);
      _ofile.write(header,sizeof(char)*headerSize);
      delete [] header;

      while(!_ifile.eof()) {
            char ss = 0,rr = 0;
            _ifile.read(&ss,sizeof(char));
	    _modul->simT(ss,rr);
	    _ofile.write(&rr,sizeof(char));
      }
   }

   void graph_code(const InputType it) {
      const unsigned headerSize = (unsigned)it;
      char* header = new char[headerSize];
      _ifile.read(header,sizeof(char)*headerSize);
      _ofile.write(header,sizeof(char)*headerSize);
      delete [] header;

      unsigned initNum = 2;
      _modul->setCode(initNum);
      while(!_ifile.eof()) {
         BitArray s120;

	 s120.init(120 + initNum);
         size_t actualRunTime;
	 for(size_t i = 0; i<120 && (!_ifile.eof()) ; actualRunTime = i += 8*sizeof(char)) {
            char ss = 0;
            _ifile.read(&ss,sizeof(char));
	    s120.setChar(i,ss);
	 }
	 BitArray r120;
	 //s120.show();
	 _modul->simuPacket_code(s120,r120);
	 //r120.show();
	 for(size_t i = 0; i<actualRunTime; i += 8*sizeof(char)) {
            char rr = r120.getChar(i);
	    _ofile.write(&rr,sizeof(char));
	 }
      }
      _modul->freeWeightTable();
   }

   void sound(const bool& isCode = 0) {
      if(isCode==1) return sound_code();
      const unsigned granularity = 100000000;
      while(!_ifile.eof()) {
         double sd = 0;
         _ifile>>sd;
         int ss = int(sd*granularity),rr = 0;
         _modul->simT(ss,rr);
         double rd = rr/(double)granularity;
         _ofile<<rd<<" ";
      }
   }

   void sound_code() {
      const unsigned granularity = 100000000;
      unsigned initNum = 2;
      _modul->setCode(initNum);
      while(!_ifile.eof()) {
         BitArray s128;
	 s128.init(128 + initNum);
	 size_t actualRunTime;
	 for(size_t i = 0; i<128 && (!_ifile.eof()) ; actualRunTime = i += 8*sizeof(int)) {
            double sd = 0;
            _ifile>>sd;
            int ss = int(sd*granularity);
	    s128.setT(i,ss);
	    //cerr<<ss<<" s ";
	 }
	 //cerr<<"==============\n";
	 //s128.show();
	 BitArray r128;
	 _modul->simuPacket_code(s128,r128);
	 //r128.show();
	 for(size_t i = 0; i<actualRunTime; i += 8*sizeof(int)) {
            int rr = 0;
            r128.getT(i,rr);
	    //cerr<<rr<<" r ";
            double rd = rr/(double)granularity;
            _ofile<<rd<<" ";
	 }
      }
      _modul->freeWeightTable();
   }
};

//   void showCode (vector<char>& code) {
//      for(size_t i = 0, s = code.size(); i<s; ++i) showSyb(code[i]);
//   }


#endif
