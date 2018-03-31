
#include <stdlib.h>
#include <cfloat>
#include <iostream>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
using namespace std;
using namespace tbb;

float Foo( float f ) { 
  float a = f*f; 
  return a;
}

class ApplyMin{
  float* my_a;
public:
  float value_of_min;
  long index_of_min;
  void operator()(const blocked_range<size_t>&r){
    float *a = my_a;
    for(size_t i = r.begin(); i!=r.end(); ++i){
      float value = Foo(a[i]);
      if(value < value_of_min){
        index_of_min = i;  
        value_of_min = value;
      }      
    }
  }

  ApplyMin(float a[]): my_a(a), value_of_min(FLT_MAX), index_of_min(-1){}
  
  void join (const ApplyMin &y){
    if(y.value_of_min < value_of_min){
      index_of_min = y.index_of_min;
      value_of_min = y.value_of_min;
    }
  }

  ApplyMin(ApplyMin &y, split): my_a(y.my_a), value_of_min(FLT_MAX), index_of_min(-1){}
};


long ParallelApplyMin(float a[], size_t n){
  ApplyMin am(a);
  parallel_reduce(blocked_range<size_t>(0,n,10000), am);
  return am.index_of_min;
}

long SerialMinIndexFoo( const float a[], size_t n ) {
  float value_of_min = FLT_MAX; 
  long index_of_min = -1; 
  for( size_t i=0; i<n; ++i ) {
    float value = Foo(a[i]); 
    if( value < value_of_min ) {
      value_of_min = value; 
      index_of_min = i;
    }
  } 
  return index_of_min;
}

int main( )  {

  long int n = 1000000;
  float *A = (float *) malloc( n*sizeof(float) );
  for( size_t i=0; i<n; ++i ) A[i] = (float) rand() / RAND_MAX;
  long indiceSec1 = SerialMinIndexFoo( A, n );
  long indice = ParallelApplyMin( A, n );
  cout << "Minimo número paralelo = " << A[indice] << endl;
  cout << "Minimo número secuencial = " << A[indiceSec1] << endl;

}

