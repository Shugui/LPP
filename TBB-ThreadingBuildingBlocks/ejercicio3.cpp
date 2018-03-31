
#include <stdlib.h>
#include <iostream>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
using namespace std;
using namespace tbb;

float Foo( float f ) { 
  unsigned int u = (unsigned int) f;
  float a = (float) rand_r(&u) / RAND_MAX;
  a = a*a; 
  return a;
}

class ApplyFoo{
  float* my_a;
public:
  float sum;
  void operator() (const blocked_range<size_t>& r){
    float *a = my_a;
    for(size_t i = r.begin(); i!=r.end();++i)
      sum += Foo(a[i]);
  } 
  ApplyFoo(float a[]): my_a(a),sum(0){}
  ApplyFoo(ApplyFoo& x, split) : my_a(x.my_a),sum(0){}

  void join (const ApplyFoo& y){ sum+=y.sum;}
};

float ParallelSumFoo(float a[], size_t n){
  ApplyFoo psf(a);
  parallel_reduce(blocked_range<size_t>(0,n,10000), psf);
  return psf.sum;
}

float SerialSumFoo( float a[], size_t n ) { 
  float sum = 0;
  for( size_t i=0; i!=n; ++i ) 
    sum += Foo(a[i]);
  return sum;
}

int main( )  {

  long int n = 1000000;
  float *A = (float *) malloc( n*sizeof(float) );
  for( size_t i=0; i<n; ++i ) A[i] = i;
  //float suma = SerialSumFoo( A, n );
  float suma = ParallelSumFoo( A, n );
  cout << "Suma = " << suma << endl;

}


