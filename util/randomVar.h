#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <cstdlib>
using namespace std;

#ifndef _RANDOM_VAR_
#define _RANDOM_VAR_

class RandomVar
{
public:
   RandomVar() {}
   virtual ~RandomVar() {}
   virtual double getVar() const =0;
/*
   virtual void show(double max) const {
      static size_t granularity = 999;
      vector<unsigned> pdf(granularity,0);
      for(size_t i =0; i<999999; ++i) {
         double n1 = getVar();
	 n1 = n1/max*granularity;
         if(n1>0 && n1<granularity) ++pdf[(unsigned)(n1)];
      }
      vector<double> y,x;
      for(size_t i =0; i<granularity; ++i) {
         cerr<<pdf[i]<<" ";
	 x.push_back(i*max/granularity);
	 y.push_back((double)pdf[i]/999999);
      }
//      save("data",y,x);
   }*/
};

class GaussianVar :public RandomVar
{
   double _mean, _variance;
public:
   GaussianVar(double m = 0, double v = 1):_mean(m), _variance(v) { srand(time(NULL)); }
   ~GaussianVar() {}
   void setMean(const double m) {_mean = m;}
   void setVariance(const double v) {_variance = v;}

   double getVar() const { double r1,r2; get2Var(r1,r2); return r1; }
   void get2Var(double& r1, double& r2) const {
      double x,y,w;
      do {
          x = (rand()/(double)RAND_MAX)*2 - 1;
          y = (rand()/(double)RAND_MAX)*2 - 1;
          w = x*x + y*y;
      } while(w>=1);
      w = sqrt((-2)*log(w)/w);
      r1 = w*x*_variance+_mean;
      r2 = w*y*_variance+_mean;
   }
};

// here is a bug
// the _mean and _variance is wrong!!!!!!!!!!!!!!
class LogNormalVar :public GaussianVar
{
   double _mean, _variance;
public:
   LogNormalVar(double m = 0, double v = 1):_mean(m), _variance(v) { srand(time(NULL)); }
   ~LogNormalVar() {}
   void setMean(const double m) {_mean = m;}
   void setVariance(const double v) {_variance = v;}
   double getVar() const {
      double r1 = GaussianVar::getVar();
      r1 = pow(2.718281,r1);
      return r1;
   }
};

class UniformVar :public RandomVar
{
   double _min, _max;
public:
   UniformVar(double m = 0, double M = 1):_min(m), _max(M) { srand(time(NULL)); }
   ~UniformVar() {}
   double getVar() const {
      double r1 = rand()/(double)RAND_MAX;
      r1 = r1*(_max - _min) + _min;
      return r1;
   }
};

#endif
