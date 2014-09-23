#ifndef _BITARRAY_H_
#define _BITARRAY_H_

#include<iostream>
#include<vector>
using namespace std;

class BitArray
{

private:
   size_t _size;
   char* _lsb;

   char position(const size_t& pos, size_t& row) const {
      if(pos>=_size) return '\0'; // assert()
      row = pos / 8;
      size_t column = pos % 8;
      return (1<<column);
   }

   size_t number() const {
      size_t num = _size/8;
      if((_size%8)!=0) ++num;
      return num;
   }
/*
   template<class T> //T must apply operator >> &
   T r0shift (T ch, const size_t n) const {
      ch = ch>>n;
      if(ch<0) ch = ch ^ (1<<((sizeof(T)*8)-1));
      return ch;
   }
*/
   void showChar(const char& s) const {
      char token = 1<<7;
      cerr<<bool(s & token);
      token = 1<<6;
      for(size_t i = 1; i< 8; ++i) {
         cerr<<bool(s & token);
	 token = token>>1;
      }
   }

public:
   BitArray():_size(0), _lsb(0) {}
   BitArray(const size_t& input):_size(0), _lsb(0) { reset(input); }
   BitArray(const char& input):_size(0), _lsb(0) { reset(input); }
   BitArray(const int& input):_size(0), _lsb(0) { reset(input); }
   BitArray(const BitArray& ba):_size(ba._size), _lsb(0) { (*this) = ba; }
   virtual ~BitArray() { reset(); }
   void init(const size_t s) {
      reset();
      _size = s;
      size_t num = number();
      _lsb = new char[num];
      for(size_t i = 0, s = num; i<s; ++i) _lsb[i] = '\0';
   }
   void reset() {
      if(_lsb!=0) delete [] _lsb;
      _lsb=0;
      _size = 0;
   }
   bool empty() const { return _size==0; }
   size_t size() const { return _size; }

   template<class T>
   void reset(T t) {
      reset();
      _size = sizeof(T)*8;
      _lsb = new char[sizeof(T)];
      for(size_t i = 0, s = sizeof(T); i<s; ++i) {
         _lsb[i] = t;
	 t = t>>8;
      }
   }
   template<class T>
   void getArray(vector<T>& t) const {
      t.clear();
      T temp = 0;
      for(size_t i = 0, s = number(); i<s; ++i) {
	 temp <<= 8;
	 const char ch = _lsb[i];
	 const char one_seven = (1<<7);
	 temp |= (ch<0)? (ch&(~one_seven)) : ch;
	 if(ch<0) temp |= (1<<7);
         if( (i+1)%sizeof(T)==0 || i==(s-1) ) {
            t.push_back(temp);
	    temp = 0;
	 }
      }
   }
   bool get(const size_t& pos) const {
      size_t row;
      char token = position(pos,row);
      if(token == '\0') return 0; // assert()
      return (bool)(_lsb[row] & token);
   }
   char getChar(const size_t& pos) const { // assert pos%8 == 0
      if(pos>=_size || pos%8!=0) return 0;
      return _lsb[ pos/8 ];
   }
   template<class T>
   void getT(const size_t& pos, T& t) const {
      if(pos>=_size || pos%8!=0) return;
      t = 0;
      for(size_t i = 0, s = sizeof(T); i<s; ++i) {
         t = t<<8;
	 const char temp = _lsb[(pos/8)+(s-i-1)];
	 const char one_seven = (1<<7);
	 t |= (temp<0)? (temp&(~one_seven)) : temp;
	 if(temp<0) t |= (1<<7);
      }
   }
   void set(const size_t& pos, const bool& b) {
      size_t row;
      char token = position(pos,row);
      if(token == '\0') return ; // assert()
      if(b==1) _lsb[row] |= (token);
      else _lsb[row] &= (~token);
   }
   void setChar(const size_t& pos, const char& ch) { // assert pos%8 == 0
      if(pos>=_size || pos%8!=0) return;
      //size_t row;
      //char token = position(pos,row);
      //if(token == '\0') return ; // assert()
      _lsb[ pos/8 ] = ch;
   }
   template<class T>
   void setT(const size_t& pos, T t) {
      if(pos>=_size || pos%8!=0) return;
      for(size_t i = 0; i< sizeof(T); ++i) {
         _lsb[(pos/8)+i] = t;
         t = t>>8;
      }
   }
   void show() const { 
      size_t num = number();
      showChar(_lsb[num-1]); // print more QQ need change!!!
      for(size_t i = 0, s = num-1; i<s; ++i)
         showChar(_lsb[num-2-i]);
      cerr<<"\n";
   }
   void operator = (const BitArray& ba) {
      init(ba._size);
      for(size_t i = 0, s = number(); i<s; ++i) 
         _lsb[i] = ba._lsb[i];
   }

};



#endif // BitArray.h
