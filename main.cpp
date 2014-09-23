/*************************************************************************
  FileName     [ main.cpp ] 
  PackageName  [ main ]
  Synopsis     [ For CommLab lab5 ]
  Author       [ Hsu bo-kai ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
*************************************************************************/
#include <iostream>
#include <string>
#include "simuModul.h"
using namespace std;

// Convert string "str" to integer "num". Return false if str does not appear
// to be a number

extern bool myStr2Int(const string& str, int& num);
extern int myStrNCmp(const string& s1, const string& s2, unsigned n);

int err(int i) {
   if(i==0) cerr<<"the argv number err!!\n";
   else cerr<<"argv["<<i<<"] err!!\n";
   return 0;
}

// bin/lab6  16 14 -bmp lena.bmp qam16/out14.bmp [-c]
// bin/lab6  16 14 [-c]
int main(int argc, char* argv[] ) {
   if(!(argc==6 || argc==7 || argc==4 || argc==3)) return err(0);
/*   int maxErr = 10,Eb = 20;
   if(myStr2Int(argv[1],maxErr)==0) return 0;
   cout<<"maxErr = "<<maxErr<<"\n";
   if(myStr2Int(argv[2],Eb)==0) return 0;*/
   int SNR_dB = 10;
   if(myStr2Int(argv[2],SNR_dB)==0) return err(2);
   int modulation = 16;
   if(myStr2Int(argv[1],modulation)==0) return err(1);
   
   SimuModul simu(modulation, SNR_dB);
   if(argc==3) {
      printf("%10.6f \t",simu.rate(300,-1));// -1 means BitER, -2 means SybER, 0 means Bit0Er
      return 0;
   }

   else if(argc==4) {
      simu.init(modulation,SNR_dB-3);
      if(myStrNCmp("-c",argv[3],2)==0) printf("%10.6f \t",simu.rate_code(300,-1));// -1 means BitER, -2 means SybER, 0 means Bit0Er
      else return err(3);
      return 0;
   }

   if(simu.openInput(argv[4])==0) return 0; // argv[4]==input, argv[5]==output
   if(simu.openOutput(argv[5])==0) return 0;
   bool isCode=0;
   if(argc==7) {
      if( myStrNCmp("-c",argv[6],2)==0) isCode=1;
      else return err(6);
   }

   if(myStrNCmp("-bmp",argv[3],4)==0) simu.graph(bmp,isCode);
   else if(myStrNCmp("-jpg",argv[3],4)==0) simu.graph(jpg,isCode);
   else if(myStrNCmp("-eps",argv[3],4)==0) simu.graph(eps,isCode);
   else if(myStrNCmp("-gif",argv[3],4)==0) simu.graph(gif,isCode);
   else if(myStrNCmp("-sound",argv[3],6)==0) simu.sound(isCode);
   else return err(3);
   return 0;
}
