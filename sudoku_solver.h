#ifndef CubeMan_BreakSudokuC_solver_H
#define CubeMan_BreakSudokuC_solver_H

#include"Sudoku.h"

#ifdef __cplusplus
extern "C" {
#endif

// solve the sudoku puzzle and store the solution
// return 0 if there's no solution
// return 1 if there exist a solution
int solve_sudoku(Sudoku *ptr);

// solve the sudoku puzzle and store the solution
// return 0 if there's no solution
// return 1 only if there exist exactly one solution
// return 2 if there are multiple solutions
int check_sudoku(Sudoku *ptr);

#ifdef __cplusplus
}
#endif

#endif
