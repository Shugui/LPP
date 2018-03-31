#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define L(i,j)  L[j*n+i]
#define A(i,j)  A[j*n+i]
void chol_escalar(int n, double *C) {

double c;
int i,j,k;

  for(k = 0; k < n; k++) {
    c = sqrt(C[k*n+k]);
    C[k*n+k] = c;
    #pragma omp parallel for
    for(i = k+1; i < n; i++) {
      C[k*n+i] = C[k*n+i]/c;
    }
    #pragma omp for private(j)
    for(i = k+1; i < n; i++) {
      for(j = k+1; j <= i-1; j++) {
       C[j*n+i] = C[j*n+i]-(C[k*n+i]*C[k*n+j]);
      }
      C[i*n+i] = C[i*n+i]-(C[k*n+i]*C[k*n+i]);
    }
  }

}




int main( int argc, char *argv[] ) {
  int n, i, j, info;
  double *L, *A;

  if( argc<2 ) {
    fprintf(stderr,"usage: %s n\n",argv[0]);
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

  /*********************************************************/
  /* Multiplicación A=L*L', donde L es triangular inferior */
  /* Devuelve la parte triangular inferior en A */
  double zero = 0.0;
  double one = 1.0;
  dsyrk_( "L", "N", &n, &n, &one, &L(0,0), &n, &zero, &A(0,0), &n );
  /*********************************************************/

  //Calculo de la descomposición de cholesky
  double t1, t2, ucpu, scpu;
  ctimer_( &t1, &ucpu, &scpu );
  chol_escalar(n,A);
  ctimer_( &t2, &ucpu, &scpu );

  /* ¿ A = L ? */
  double error = 0.0;
  for( j=0; j<n; j++ ) {
    for( i=j; i<n; i++ ) {
      double b = (A(i,j)-L(i,j));
      error += b*b;
    }
  }
  error = sqrt(error);
  printf("Error = %10.4e\n",error);
  printf("Tiempo del algoritmo tamaño %d -->%20.10f seg\n",n, t2-t1);

  free(A);
  free(L);
}


