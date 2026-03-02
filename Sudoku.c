#include"Sudoku.h"
#include<stdio.h>

void print_sudoku(Sudoku *ptr, int with_grid){
    for(int i=0;i<9;++i){
        for(int j=0;j<9;++j){
            if(with_grid&&(j==3||j==6)) printf("|");
            else if(j) printf(" ");
            printf("%d", ptr->puzzle[i][j]);
        }
        printf("\n");
        if(with_grid&&(i==2||i==5)) printf("-----+-----+-----\n");
    }
}
int read_sudoku(Sudoku *ptr){
    int ret;
    for(int i=0;i<9;++i){
        for(int j=0;j<9;++j){
            int tmp;
            ret=scanf("%d", &tmp);
            if(ret==EOF) return ret;
            ptr->puzzle[i][j]=tmp;
        }
    }
    return ret;
}
