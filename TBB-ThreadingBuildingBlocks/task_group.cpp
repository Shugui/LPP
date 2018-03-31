
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <tbb/tick_count.h>
#include <tbb/task_group.h>

using namespace std;
using namespace tbb;

extern void quicksort(long begin, long end, vector<double>& v );

long dividir( long begin, long end, vector<double>& v ) {
  long ind = end-1;
  double valor = v[ind];
  long x = begin;
  long y = ind;
  while( x<y ) {
    while( v[x]<valor && x<y ) { x++; }
    while( v[y]>=valor && x<y ) { y--; }
    if( x<y ) {
      double a = v[x];
      v[x] = v[y];
      v[y] = a;
    }
  }
  double a = v[x];
  v[x] = v[ind];
  v[ind] = a;
  return x;
}

class QuicksortClass{
  long my_begin, my_end;
  vector<double>& my_v;
public:
  QuicksortClass(long begin, long end, vector<double>&v): 
    my_begin(begin),
    my_end(end),
    my_v(v)
  {}

  void operator()() const{
    quicksort(my_begin,my_end,my_v);
  }
};

/* Sorts the elements in the range [begin,end) into ascending order */
void quicksort( long begin, long end, vector<double>& v ) {
  if( begin<end ) {
    long size = end-begin;
    if( size<1000 ) {
      sort(v.begin()+begin,v.begin()+end);
    } else {
      long medio = dividir( begin, end, v );
      task_group g;
      g.run( QuicksortClass(begin, medio, v));
      g.run( QuicksortClass(medio+1, end, v));
      g.wait();
    }
  } 
}

int main( int argc, char *argv[] ) {

  if( argc<2 ) {
    cout << "Usage: " << argv[0] << " n" << endl;
    return 0;
  }
  long n;
  istringstream dato(argv[1]);
  dato >> n;

  vector<double> v(n);
  for( long i=0; i<n; i++ ) {
    v[i] = static_cast <double>(rand()) / static_cast<double>(RAND_MAX);
  }
  
  tick_count t0 = tick_count::now(); 
  quicksort( 0, n, v );
  tick_count t1 = tick_count::now(); 
  cout << "Tiempo = " << (t1-t0).seconds() << endl;

  /* Comprobación */
  int error = 0;
  for( long i=0; i<n-1; i++ ) {
    if( v[i] > v[i+1] ) {
      error = 1;
    }
  }
  if( error ) wcerr << "Mal ordenado" << endl;
  
}

