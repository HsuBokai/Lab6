#ifndef _CODE_H_
#define _CODE_H_

#include <vector>
#include "BitArray.h"

class Code
{
private:
   unsigned initNum;
   int  *_weight0 , *_weight1;
   vector<const unsigned*> _generator;
public:
   Code(const unsigned n):initNum(n), _weight0(0), _weight1(0) {}
   virtual ~Code() { freeWeightTable(); }

   void setGenerator(const unsigned* g) {
      _generator.push_back(g);
   }

   void encode(const BitArray& s120, vector<BitArray>& s_code) {
      s_code.clear();
      BitArray initial;
      initial.init(s120.size());
      for(size_t j = 0, g = _generator.size(); j<g; ++j) {
         s_code.push_back(initial);
         unsigned init = 0;
	 for(size_t t = 0, time = s120.size(); t<time; ++t) {
	    init = init>>1;
	    if(s120.get(t)==1) init |= (1<<initNum);
	    bool sum = 0;
	    for(size_t i = 0, s = initNum+1; i<s; ++i) {
	       bool b = init & (1<<i);
	       if( (_generator[j][i/3]&(1<<(i%3))) != 0) sum = sum^b;
	    }
	    s_code[j].set(t,sum);
	 }
      }
//      for(size_t i = 0, s = s120.size(); i<s; i+=8) {
//         vector<char> temp;
//         encode(s120.getChar(i),temp);
//         for(size_t j = 0, g = _generator.size(); j<g; ++j)
//	    s_code[j].setChar(i,temp[j]);
//      }
   }


   void decode(const vector<BitArray>& cipher, BitArray& clear) {
      if(_weight0 == 0 || _weight1 == 0) return;
      if(cipher.size() != _generator.size()) return; // assert();
      const size_t time = cipher[0].size(), stateNum = 1<<initNum; // initNum<31
      clear.init(time);
      unsigned tree[ stateNum ][ time ];
      vector<int> currentWeight( stateNum , 99999990 );
      currentWeight[0] = 0;
      for(size_t t = 0; t<time; ++t) {
         size_t c = 0; // generator.size() < 32;
	 for(size_t i = 0, s = cipher.size(); i<s; ++i) {
	    c = c<<1;
	    if(cipher[s-i-1].get(t)==1) c = c|1;
	 }
	 //cerr<<"c is "<<c<<"\n";
         size_t gg = (1<<_generator.size());
	 vector<int> preWeight(currentWeight);
         for(size_t state = 0; state < stateNum; ++state) {
	    size_t preState0 = (state<<1) % stateNum;
	    int w0 = preWeight[ preState0 ] + _weight0[ state*gg + c];
	    int w1 = preWeight[ preState0+1 ] + _weight1[ state*gg + c];
	    tree[state][t] = (w0<w1)? preState0  : (preState0+1);
	    currentWeight[state] = (w0<w1)? w0: w1;
	  //  cerr<<tree[state][t]<<","<<currentWeight[state]<<"     ";
	 }
	// cerr<<"\n";
      }
      size_t state = 0;
      for(size_t t = 0; t<time; ++t) {
         size_t tt = time-t-1;
	 bool b = (state >= (stateNum>>1))? 1:0; //care about >> initNum < 31
	 clear.set(tt,b);
	 state = tree[state][tt];
      }
   }

   void weightTable() {
      size_t ss = (1<<initNum), gg = (1<<_generator.size());
      _weight0 = new int[ ss * gg ]; 
      _weight1 = new int[ ss * gg ]; 
      for(size_t g = 0; g < gg; ++g) {
         BitArray recieve(g);
         for(size_t s = 0; s < ss; ++s) {
	    BitArray state(s);
	    int err0 = 0, err1 = 0;
            for(size_t j = 0, t = _generator.size(); j<t; ++j) {
	       char c = '\0';
	       for(size_t i = 0, s = initNum+1; i<s; ++i) {
	          char ch = (i == 0)? 0:state.get(i-1); 
	          if( (_generator[j][i/3]&(1<<(i%3))) != 0) c = c^ch;
	       }
	       if(recieve.get(j)!=c) {
	          ++err0;
	          if((_generator[j][0]&1)==0) ++err1;
	       }
	       else if((_generator[j][0]&1)!=0) ++err1;
            }
	    _weight0[s*gg + g] = err0; 
	    _weight1[s*gg + g] = err1; 
	//    cerr<<err0<<","<<err1<<" ";
         }
	 //cerr<<"\n";
      }
   }

   void freeWeightTable() {
      if(_weight0 != 0) delete [] _weight0;
      _weight0 = 0;
      if(_weight1 != 0) delete [] _weight1;
      _weight1 = 0;
   }
};
/*
   void encode(const char& ss, vector<char>& s_code) {
      s_code.clear();
      static vector<char> init(initNum,0);
      // if(reset_init == 1)  clear(init);

      vector<char> nextInit(initNum,0);
      char shiftR0_ss = (ss>>1) & (~(1<<7));
      for(size_t i = 0, s = init.size(); i<s; ++i) { // init.size() < 8

         size_t ii = s-i-1;
         init[ii] = init[ii] | ss<<(i+1);
         nextInit[ii] = shiftR0_ss>>(6-i);
      }
      for(size_t j = 0, t = _generator.size(); j<t; ++j) {
	 char c = 0;
	 for(size_t i = 0, s = initNum+1; i<s; ++i) {
	    char ch = (i < initNum)? init[i] : ss;
	    if( (_generator[j][i/3]&(1<<(i%3))) != 0) c = c^ch;
	 }
	 s_code.push_back(c);
      }
      for(size_t i = 0, s = init.size(); i<s; ++i)  
         init[i] = nextInit[i];
   }
*/

#endif // code.h
