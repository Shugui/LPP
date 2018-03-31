
#ifndef _SUDOKU
#define _SUDOKU

#define sol(a,b) sol[(a-1)*9+(b-1)]
#define mascara(a,b) mascara[(a-1)*9+(b-1)]

#ifdef __cplusplus
extern "C" {
#endif
  void init_sudoku( const char *tipo, int sol[81] );
  void prin_sudoku( int sol[81] );
  void sudoku_sol( int i, int j, int sol[81], int mascara[81] );
  int es_factible( int i, int j, int sol[81] );
#ifdef __cplusplus
}
#endif
int correspondencia3x3( int i );

#endif