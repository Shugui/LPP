
/* 
 * Cholesky por bloques. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <clapack.h>
#include "ctimer.h"
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
using namespace std;
using namespace tbb;

#define	L(i,j)	L[j*n+i]
#define	A(i,j)	A[j*n+i]
#define	C(i,j)	C[j*n+i]

inline int min(int a, int b) { return (a < b) ? a : b; }

void Foo(int n, int b, int k, double *C){
  int i, j, mk, mi, mj;
  
    mk = min(k+b,n);
    clapack_dpotrf(CblasColMajor,CblasLower,mk-k,&C[k+k*n],n);
    for(i = k+b; i < n; i+=b){
      mi = min(i+b,n);
      cblas_dtrsm(CblasColMajor,CblasRight,CblasLower,CblasTrans,CblasNonUnit,mi-i,mk-k,
          1,&C[k+k*n],n,&C[i+k*n],n) ;
    }

    for(i = k+b; i < n; i+=b){
      mi = min(i+b,n);
      for(j = k + b; j < (i-1); j+=b){
        mj = min(j+b,n);
        cblas_dgemm(CblasColMajor,CblasNoTrans,CblasTrans,mi-i,mj-j,mk-k,-1,&C[i+k*n],n
            ,&C[j+k*n],n,1,&C[i+j*n],n);
      }
      cblas_dsyrk(CblasColMajor,CblasLower,CblasNoTrans,mi-i,mk-k,-1,&C[i+k*n],n,1,&C[i+i*n],n);
    }
}


class ApplyChol{
  
public:
  double *const my_C;
  int const my_b;
  int const my_n;
  void operator()(const tbb::blocked_range<size_t>&r){
      double *c = my_C;
      int b = my_b;
      int n = my_n;
      for(size_t i = r.begin(); i!=r.end();i+=b)
        Foo(n,b,i,c);
  }
  ApplyChol(int n,int b, double *C):my_n(n), my_b(b),my_C(C){ }

};

void ParallelCholeskyBloques(  int n ,  int b, double *C, double *A   ){
  cblas_dcopy(n*n,A,1,C,1);
  tbb::parallel_for(tbb::blocked_range<size_t>(0,n,100000), ApplyChol(n,b,C));
}


void choleskyBloques( int n, int b, double *C, double *A);

int main( int argc, char *argv[] ) {
  int n, b, i, j, info;
  double *L, *A, *C;

  if( argc<3 ) {
    fprintf(stderr,"usage: %s n block_size\n",argv[0]);
    exit(-1);
  }
  sscanf(argv[1],"%d",&n);
  if( ( L = (double*) malloc(n*n*sizeof(double)) ) == NULL ) {
    fprintf(stderr,"Error en la reserva de memoria para la matriz L\n");
    exit(-1);
  }

  //Generar matriz triangular inferior
  for( j=0; j<n; j++ ) {
    for( i=0; i<j; i++ ) {
      L(i,j) = 0.0;
    }
    for( i=j; i<n; i++ ) {
      L(i,j) = ((double) rand()) / RAND_MAX;
    }
    L(j,j) += n;
  }

  if( ( A = (double*) malloc(n*n*sizeof(double)) ) == NULL ) {
    fprintf(stderr,"Error en la reserva de memoria para la matriz A\n");
    exit(-1);
  }

  if( ( C = (double*) malloc(n*n*sizeof(double)) ) == NULL ) {
    fprintf(stderr,"Error en la reserva de memoria para la matriz C\n");
    exit(-1);
  }
  /*********************************************************/
  /* Multiplicación A=L*L', donde L es triangular inferior */
  /* Devuelve la parte triangular inferior en A */
  int zero = 0.0;
  int one = 1.0;
  cblas_dsyrk(CblasColMajor,CblasLower,CblasNoTrans,n,n,one,&L(0,0),n,zero,&A(0,0),n);
   
  //dsyrk_( "L", "N", &n, &n, &one, &L(0,0), &n, &zero, &A(0,0), &n );
  /*********************************************************/

  sscanf(argv[2],"%d",&b);


  double t1, t2, ucpu, scpu;
  ctimer_( &t1, &ucpu, &scpu );
  ParallelCholeskyBloques( n, b, C, A );
  ctimer_( &t2, &ucpu, &scpu );

  if( info != 0 ) {
    fprintf(stderr,"Error = %d en la descomposición de Cholesky de la matriz A\n",info);
    exit(-1);
  }

  /* ¿ C = L ? */
  double error = 0.0;
  for( j=0; j<n; j++ ) {
    for( i=j; i<n; i++ ) {
      double b = (C(i,j)-L(i,j));
      error += b*b;
    }
  }
  error = sqrt(error);
  printf("Error = %10.4e\n",error);
  printf("Tiempo del algoritmo tamaño %d y b = %d -->%20.10f seg\n",n, b, t2-t1);
  free(A);
  free(L);
  free(C);
}




void choleskyBloques( size_t n, int b, double *C, double *A ) {
  cblas_dcopy(n*n,A,1,C,1);
  for(size_t k = 0; k < n; k+=b)
    Foo(n,b,k,C);
}



