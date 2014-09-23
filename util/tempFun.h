/****************************************************************************
  FileName     [ tempFun.h ] 
  PackageName  [ util ]
  Synopsis     [ For make TEMPLATE function to be used ]
  Author       [ Hsu bo-kai ]
  Copyright    [ Copyleft(c) 2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef _TEMPFUN_H_
#define _TEMPFUN_H_

#include <iostream>
using namespace std;

   template<class T> //T must apply operator >> &
   T r0shift (T ch, const size_t n) {
      ch = ch>>n;
      if(ch<0) ch = ch ^ (1<<((sizeof(T)*8)-1));
      return ch;
   }

   template<class T> // T must apply operator >> &
   void showSyb(const T& s) {
      T token = 1<<((sizeof(T)*8)-1);
      for(size_t i = 0; i< sizeof(T)*8; ++i) {
         cerr<<bool(s & token);
	 token = r0shift(token,1) ;
      }
      cerr<<"\n";
   }

#endif // tempFun.h
