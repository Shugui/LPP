#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"

void init_sudoku( const char *tipo, int sol[81] ) {
  if( strcmp(tipo,"normal")    && 
      strcmp(tipo,"muy facil") && 
      strcmp(tipo,"muy dificil") ) {
    printf("El tipo de init_sudoku(tipo,sol) debe ser \"normal\" o \"muy facil\" para desarrollar codigo \no \"muy dificil\" para realizar el analisis de prestaciones de los algoritmos\n");
    exit(-1);
  }
  printf("Tipo = %s\n",tipo);

  int i, j;
  for( i = 1; i <= 9; i++ ) {
    for( j = 1; j <= 9; j++ ) {
       sol(i,j) = 0;
    }
  }
  
  if( !strcmp(tipo,"normal") ) {
  /* Normal */
  sol(1,4) = 7;
  sol(1,8) = 5;
  sol(1,9) = 3;
  
  sol(2,2) = 9;
  sol(2,4) = 3;
  sol(2,9) = 4;

  sol(3,3) = 6;
  sol(3,5) = 4;
  sol(3,7) = 8;

  sol(4,6) = 5;
  sol(4,8) = 9;

  sol(5,1) = 4;
  sol(5,2) = 7;
  sol(5,5) = 9;
  sol(5,6) = 6;

  sol(6,1) = 9;
  sol(6,3) = 5;
  sol(6,5) = 3;

  sol(7,2) = 2;
  sol(7,3) = 7;
  sol(7,4) = 5;
  sol(7,8) = 3;

  sol(8,2) = 4;
  sol(8,3) = 9;
  sol(8,4) = 2;
  sol(8,8) = 8;
  sol(8,9) = 7;

  sol(9,1) = 5;
  sol(9,3) = 3;
  sol(9,7) = 2;
  sol(9,8) = 4;
  }

  if( !strcmp(tipo,"muy facil") ) {
  /* Muy facil */
  sol(1,4) = 3;
  sol(1,5) = 4;
  sol(1,7) = 6;
  sol(1,8) = 8;

  sol(2,1) = 4;
  sol(2,5) = 1;
  sol(2,9) = 7;

  sol(3,1) = 3;
  sol(3,5) = 2;
  sol(3,6) = 8;
  sol(3,7) = 5;

  sol(4,1) = 8;
  sol(4,2) = 6;
  sol(4,3) = 7;
  sol(4,5) = 5;
  sol(4,7) = 2;
  sol(4,8) = 3;
  sol(4,9) = 4;

  sol(5,3) = 2;
  sol(5,6) = 4;
  sol(5,9) = 6;

  sol(6,4) = 8;
  sol(6,5) = 6;
  sol(6,6) = 2;

  sol(7,7) = 1;
  sol(7,9) = 2;

  sol(8,4) = 2;
  sol(8,6) = 6;
  sol(8,8) = 5;

  sol(9,2) = 2;
  sol(9,6) = 5;
  sol(9,7) = 4;
  sol(9,8) = 9;
  }

  if( !strcmp(tipo,"muy dificil") ) {
  /* Muy dificil */
  sol(1,3) = 4;
  sol(1,4) = 3;
  sol(1,7) = 8;
  sol(1,8) = 5;

  sol(2,9) = 2;

  sol(3,6) = 1;
  sol(3,7) = 6;

  sol(4,2) = 1;
  sol(4,6) = 3;

  sol(5,2) = 7;
  sol(5,4) = 6;
  sol(5,7) = 4;

  sol(6,1) = 3;
  sol(6,3) = 2;
  sol(6,4) = 8;

  sol(7,1) = 6;
  sol(7,2) = 9;
  sol(7,4) = 5;
  sol(7,5) = 4;

  sol(8,3) = 5;
  sol(8,5) = 2;
  sol(8,9) = 3;

  sol(9,1) = 7;
  sol(9,6) = 8;
  }
}

