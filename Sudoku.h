#ifndef CubeMan_BreakSudokuC_Sudoku_H
#define CubeMan_BreakSudokuC_Sudoku_H

#include<stdint.h>
typedef struct Sudoku_{
    uint8_t puzzle[9][9];
} Sudoku;
void print_sudoku(Sudoku*, int);
int read_sudoku(Sudoku*);

#endif
