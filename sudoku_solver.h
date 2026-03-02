#ifndef CubeMan_BreakSudokuC_solver_H
#define CubeMan_BreakSudokuC_solver_H

#include"Sudoku.h"

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

// solve the sudoku puzzle and store the solution
// return 0 if there's no solution
// return 1 if there exist a solution
EXTERN int solve_sudoku(Sudoku *ptr);

// solve the sudoku puzzle and store the solution
// return 0 if there's no solution
// return 1 only if there exist exactly one solution
// return 2 if there are multiple solutions
EXTERN int check_sudoku(Sudoku *ptr);

#endif
