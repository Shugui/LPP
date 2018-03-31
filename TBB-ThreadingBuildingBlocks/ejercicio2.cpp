
#include <stdlib.h>
#include <iostream>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
using namespace std;

void Foo( float& f ) { 
  unsigned int u = (unsigned int) f;
  f = (float) rand_r(&u) / RAND_MAX;
  f = f*f; 
}
 
class ApplyFoo{
  float *const my_a;
public:
  void operator()(const tbb::blocked_range<size_t>&r)const{
      float *a = my_a;
      for(size_t i = r.begin(); i!=r.end();++i)
        Foo(a[i]);
  }
  ApplyFoo(float a[]):
    my_a(a)
    {}
};

void SerialApplyFoo( float a[], size_t n ) { 
  for( size_t i=0; i<n; ++i )
    Foo(a[i]);
}

void ParallelApllyFoo(float a[], size_t n) {
  tbb::parallel_for(tbb::blocked_range<size_t>(0,n,100000), ApplyFoo(a));
}

int main( )  {

  long int n = 1000000;
  float *A = (float *) malloc( n*sizeof(float) );
  for( size_t i=0; i<n; ++i ) A[i] = i;
  //SerialApplyFoo( A, n );
  ParallelApllyFoo(A, n);
  for( size_t i=0; i<n; ++i )
    cout << A[i] << endl;
}

