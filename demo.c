#include<stdio.h>
#include"sudoku_solver.h"
signed main(){
    Sudoku data;
    while(read_sudoku(&data)!=EOF){
        char const *msg="no solution";
        if(solve_sudoku(&data)) msg="solved";
        printf("%s\n", msg);
        print_sudoku(&data,1);
        printf("\n");
    }
    
    return 0;
}
