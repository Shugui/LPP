
/* 
 * Cholesky por bloques. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <clapack.h>

#define L(i,j)  L[j*n+i]
#define A(i,j)  A[j*n+i]
#define C(i,j)  C[j*n+i]

void choleskyBloques( int n, int b, double *C, double *A);

int main( int argc, char *argv[] ) {
  int n, b, i, j;
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
  choleskyBloques( n, b, C, A );
  ctimer_( &t2, &ucpu, &scpu );

  /* ¿ A = L ? */
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

inline int min(int a, int b) { return (a < b) ? a : b; }

void choleskyBloques( int n, int b, double *C, double *A ) {
  int i, j, k, mk, mi, mj;
  cblas_dcopy(n*n,A,1,C,1);


    for(k = 0; k < n; k+=b)
  {
    mk = min(k+b,n);
    clapack_dpotrf(CblasColMajor,CblasLower,mk-k,&C[k+k*n],n);
    
    #pragma omp parallel for private(mi)
    for(i = k+b; i < n; i+=b)
    {
      mi = min(i+b,n);
      cblas_dtrsm(CblasColMajor,CblasRight,CblasLower,CblasTrans,CblasNonUnit,mi-i,mk-k,
          1,&C[k+k*n],n,&C[i+k*n],n) ;
    }
    #pragma omp parallel for private(j, mi, mj)
    for(i = k+b; i < n; i+=b)
    {
      mi = min(i+b,n);
      for(j = k + b; j < (i-1); j+=b)
      {
        mj = min(j+b,n);
        cblas_dgemm(CblasColMajor,CblasNoTrans,CblasTrans,mi-i,mj-j,mk-k,-1,&C[i+k*n],n
            ,&C[j+k*n],n,1,&C[i+j*n],n);
      }
      cblas_dsyrk(CblasColMajor,CblasLower,CblasNoTrans,mi-i,mk-k,-1,&C[i+k*n],n,1,&C[i+i*n],n);
    }
  }
}

