#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <tbb/tick_count.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/task_group.h>
#include "sudoku.h"

using namespace std;
using namespace tbb;

bool fin;

class SudokuFunctor {
  int my_i, my_j;
  int *my_sol, *my_mascara;
  int* aux;
  
  public:
    SudokuFunctor(int i, int j, int* sol, int* mas) :
      my_i(i),
      my_j(j),
      my_mascara(mas)
    {
      int k;
      aux = (int*)malloc(81*sizeof(int));
      for(k = 0; k < 81; k++){
        aux[k] = sol[k];
      }
    }
    void operator()() const { sudoku_sol(my_i,my_j,aux,my_mascara); }

};

int main( int argc, char *argv[] ) {

  if(argc != 2){
    cout << "Usage: ./sudoku numThreads\n" << endl;
    exit(-1);
  }
  int threads = atoi(argv[1]);
  fin = false;
  task_scheduler_init init( threads );

  int mascara[81];
  int sol[81];
  int i, j;

  printf("sudoku inicial: \n");
  init_sudoku("muy dificil",sol);
  prin_sudoku(sol);

  for( i = 1; i <= 9; i++ ) {
    for( j = 1; j <= 9; j++ ) {
       mascara(i,j) = sol(i,j) != 0;
    }
  }

  tick_count t0 = tick_count::now();
  sudoku_sol( 1, 1, sol, mascara );
  tick_count t1 = tick_count::now();  
  printf("Work took %g seconds\n", (t1-t0).seconds());

  return 0;
}
void sudoku_sol( int i, int j, int sol[81], int mascara[81] ) {
  if(fin == false){
   int k;
   task_group g;
   if( mascara(i, j) == 0 ) {
      for( k = 1; k <= 9; k++ ) {
         sol( i, j ) = k;                                
         if( es_factible( i, j, sol ) ) {
            if( i == 9 && j == 9 ) {
              printf("Solucion: \n");
              prin_sudoku(sol);
              fin = true;
            }
            if( i < 9 && j == 9 ) {
              g.run(SudokuFunctor(i+1, 1, sol, mascara));
            }
            if( i <= 9 && j < 9 ) {
              g.run(SudokuFunctor(i, j+1, sol, mascara));
            }
         }
      } 
      g.wait();
      sol(i, j) = 0;                             
   } else { 
      if( i == 9 && j == 9 ) {
         printf("Solucion:\n");
         prin_sudoku(sol);
         fin = true;
      }
      if( i < 9 && j == 9 ) {
         g.run(SudokuFunctor( i+1, 1, sol, mascara ));
      }
      if( i <= 9 && j < 9 ) {
         g.run(SudokuFunctor(i , j+1, sol, mascara ));
      }
    g.wait();
   }
 }
}

