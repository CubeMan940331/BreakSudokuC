#ifndef CubeMan_BreakSudokuC_Sudoku_H
#define CubeMan_BreakSudokuC_Sudoku_H

#include<stdint.h>
typedef struct Sudoku_{
    uint8_t puzzle[9][9];
} Sudoku;

#ifdef __cplusplus
extern "C" {
#endif

void print_sudoku(Sudoku *ptr, int with_grid);
int read_sudoku(Sudoku *ptr);

#ifdef __cplusplus
}
#endif

#endif
