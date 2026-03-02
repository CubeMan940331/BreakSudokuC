#include"sudoku_solver.h"
#include<string.h>

#define bitset_size(n) ((n)/(sizeof(int)*8)+((n)%(sizeof(int)*8)!=0))
#define note_idx(i,j,n) (81*((n)-1)+(i)*9+(j))
#define n_in_idx(n,i) (((n)-1)*9+(i))

typedef struct solver_state_{
    int note[bitset_size(9*9*9)];
    int n_in_x[bitset_size(9*9)];
    int n_in_y[bitset_size(9*9)];
    int n_in_chunk[bitset_size(9*9)];

    Sudoku *sudoku_p;

    uint8_t note_cnt[9][9];
    uint8_t complete_state;
} solver_state;

// ================================================================
// bitset functions

int bitset_get(int *data, int idx){
    int q=idx/(sizeof(int)*8);
    int r=idx%(sizeof(int)*8);
    return (data[q]&(1<<r))!=0;
}
void bitset_set(int *data, int idx){
    int q=idx/(sizeof(int)*8);
    int r=idx%(sizeof(int)*8);
    data[q] |= (1<<r);
}
void bitset_unset(int *data, int idx){
    int q=idx/(sizeof(int)*8);
    int r=idx%(sizeof(int)*8);
    data[q] &= ~(1<<r);
}

// ================================================================
// solver_state functions

void solver_state_init(solver_state *ptr){
    memset(ptr->note, (~0), bitset_size(9*9*9)*sizeof(int));
    memset(ptr->n_in_x, 0, bitset_size(9*9)*sizeof(int));
    memset(ptr->n_in_y, 0, bitset_size(9*9)*sizeof(int));
    memset(ptr->n_in_chunk, 0, bitset_size(9*9)*sizeof(int));
    for(int i=0;i<9;++i){
        for(int j=0;j<9;++j){
            ptr->note_cnt[i][j]=9;
        }
    }
    ptr->complete_state=0;
    ptr->sudoku_p=NULL;
}

// mark number n is possible at (x,y)
int solver_state_set_note(solver_state *ptr, int x, int y, int n){
    int idx=note_idx(x,y,n);
    int tmp=bitset_get(ptr->note, idx);
    if(!tmp) ++ptr->note_cnt[x][y];
    bitset_set(ptr->note, idx);
    return tmp;
}

// mark number n is not possible at (x,y)
int solver_state_unset_note(solver_state *ptr, int x, int y, int n){
    int idx=note_idx(x,y,n);
    int tmp=bitset_get(ptr->note, idx);
    if(tmp) --ptr->note_cnt[x][y];
    bitset_unset(ptr->note, idx);
    return tmp;
}

// note inference
int solver_state_update_note(solver_state *ptr){
    int isc=0;
    int sx,sy,ex,ey;
    int x,y,count;
    for(int c=0;c<9;++c){//chunk 0~8
        sx=(c%3)*3;sy=(c/3)*3;
        ex=sx+2;ey=sy+2;
        for(int n=1;n<=9;++n){//number 1~9
            int ise;
            count=0;
            for(int i=sx;i<=ex;++i){//check if all note in the same x
                ise=0;
                for(int j=sy;j<=ey;++j){
                    if(bitset_get(ptr->note, note_idx(i,j,n))){
                        x=i;
                        ise=1;
                        break;
                    }
                }
                count+=ise;
            }
            if(count==1){
                for(int j=0;j<9;++j){
                    if(j>=sy&&j<=ey) continue;//skip this chunk
                    if(solver_state_unset_note(ptr,x,j,n)) isc=1;
                }
            }
            count=0;
            for(int j=sy;j<=ey;++j){//check if all note in the same y
                ise=0;
                for(int i=sx;i<=ex;++i){
                    if(bitset_get(ptr->note, note_idx(i,j,n))){
                        y=j;
                        ise=1;
                        break;
                    }
                }
                count+=ise;
            }
            if(count==1){
                for(int i=0;i<9;++i){
                    if(i>=sx&&i<=ex) continue;//skip this chunk
                    if(solver_state_unset_note(ptr, i,y,n)) isc=1;
                }
            }
        }
    }
    for(int n=1;n<=9;++n){//n 1~9
        for(int t=0;t<9;++t){//0~8
            //y
            count=0;
            for(int c=0;c<3;++c){//3 chunks
                sy=c*3;ey=sy+2;
                for(int j=sy;j<=ey;++j){
                    if(bitset_get(ptr->note, note_idx(t,j,n))){
                        ++count;
                        y=j;
                        break;
                    }
                }
            }
            if(count==1){
                int chunk=(t/3)+(y/3)*3;
                sx=(chunk%3)*3;sy=(chunk/3)*3;
                ex=sx+2;ey=sy+2;
                for(int i=sx;i<=ex;++i){
                    for(int j=sy;j<=ey;++j){
                        if(i==t) continue;
                        if(solver_state_unset_note(ptr, i,j,n)) isc=1;
                    }
                }
            }
            //x
            count=0;
            for(int c=0;c<3;++c){//3 chunks
                sx=c*3;ex=sx+2;
                for(int i=sx;i<=ex;++i){
                    if(bitset_get(ptr->note, note_idx(i,t,n))){
                        ++count;
                        x=i;
                        break;
                    }
                }
            }
            if(count==1){
                int chunk=(x/3)+(t/3)*3;
                sx=(chunk%3)*3;sy=(chunk/3)*3;
                ex=sx+2;ey=sy+2;
                for(int i=sx;i<=ex;++i){
                    for(int j=sy;j<=ey;++j){
                        if(j==t) continue;
                        if(solver_state_unset_note(ptr, i,j,n)) isc=1;
                    }
                }
            }
        }
    }
    return isc;
}

// set grid (x,y) to number n
void solver_state_set_n(solver_state *ptr, int x, int y, int n){
    // allow sudoku_p to be NULL for solver_state_load
    if(ptr->sudoku_p) ptr->sudoku_p->puzzle[x][y]=n;

    for(int i=0;i<9;++i){
        solver_state_unset_note(ptr, x, y, i+1);
        solver_state_unset_note(ptr, x, i, n);
        solver_state_unset_note(ptr, i, y, n);
    }
    int chunk=(x/3)+(y/3)*3;
    int sx=x-x%3,sy=y-y%3;
    for(int i=0;i<3;++i){
        for(int j=0;j<3;++j){
            solver_state_unset_note(ptr, i+sx,j+sy,n);
        }
    }
    bitset_set(ptr->n_in_chunk, n_in_idx(n,chunk));
    bitset_set(ptr->n_in_x, n_in_idx(n, x));
    bitset_set(ptr->n_in_y, n_in_idx(n, y));
    ++ptr->complete_state;
}

// number inference
int solver_state_update_n(solver_state *ptr){
    int isc=0;
    int sx,ex,sy,ey,count,x,y;
    for(int n=1;n<=9;++n){
        //serach for pos_count==1
        for(int i=0;i<9;++i){
            for(int j=0;j<9;++j){
                if(ptr->note_cnt[i][j]==1 && bitset_get(ptr->note, note_idx(i,j,n))){
                    solver_state_set_n(ptr, i,j,n);
                    isc=1;
                }
            }
        }
        //search for only noted n in chunk
        for(int c=0;c<9;++c){
            if(bitset_get(ptr->n_in_chunk, n_in_idx(n,c))) continue;
            sx=(c%3)*3;sy=(c/3)*3;
            ex=sx+2;ey=sy+2;
            count=0;
            for(int i=sx;i<=ex;++i){
                for(int j=sy;j<=ey;++j) if(bitset_get(ptr->note, note_idx(i,j,n))){
                    ++count;
                    x=i;
                    y=j;
                }
            }
            if(count==1){
                solver_state_set_n(ptr, x,y,n);
                isc=1;
            }
        }
        for(int t=0;t<9;++t){
            //search for only noted n in same x
            if(bitset_get(ptr->n_in_x, n_in_idx(n,t))){
                count=0;
                for(int j=0;j<9;++j) if(bitset_get(ptr->note, note_idx(t,j,n))){
                    ++count;
                    y=j;
                }
                if(count==1){
                    solver_state_set_n(ptr, t,y,n);
                    isc=1;
                }
            }
            //search for only noted n in same y
            if(bitset_get(ptr->n_in_y, n_in_idx(n,t))){
                count=0;
                for(char i=0;i<9;++i) if(bitset_get(ptr->note, note_idx(i,t,n))){
                    ++count;
                    x=i;
                }
                if(count==1){
                    solver_state_set_n(ptr, x,t,n);
                    isc=1;
                }
            }
        }
    }
    return isc;
}
int solver_state_is_legal(solver_state *ptr){
    for(int i=0;i<9;++i){
        for(int j=0;j<9;++j){
            // if no possible number in empty grid
            if(
                ptr->note_cnt[i][j]==0 &&
                ptr->sudoku_p->puzzle[i][j]==0
            ) return 0;
        }
    }
    return 1;
}
void solver_state_inference(solver_state *ptr){
    int flag=1;
    while(flag){
        flag=0;
        if(solver_state_update_n(ptr)) flag=1;
        if(solver_state_update_note(ptr)) flag=1;
    }
}

// load Sudoku and sanity check on sudoku_p
// must call `solver_state_init` on `solver_state` first
int solver_state_load(solver_state *ptr, Sudoku *sudoku_p){
    for(int i=0;i<9;++i){
        for(int j=0;j<9;++j){
            if(sudoku_p->puzzle[i][j]==0) continue;
            if(!bitset_get(
                ptr->note,
                note_idx(i,j,sudoku_p->puzzle[i][j])
            )) return 0;
            solver_state_set_n(ptr, i,j,sudoku_p->puzzle[i][j]);
        }
    }
    ptr->sudoku_p=sudoku_p;
    return 1;
}

// ================================================================
// sudoku_solver functions
/*
The memory holding `solver_state` is shared across the recursion
*/


typedef struct sudoku_solver_ {
    solver_state state;
    Sudoku *sudoku;
    int solution_cnt;
    int test_multi;
} sudoku_solver;

int sudoku_solver_dfs(sudoku_solver *ptr, Sudoku *node){
    solver_state_init(&ptr->state);
    solver_state_load(&ptr->state, node);
    solver_state_inference(&ptr->state);

    if(ptr->state.complete_state==81){
        ++ptr->solution_cnt;
        memcpy(ptr->sudoku, node, sizeof(Sudoku));
        if(ptr->test_multi) return ptr->solution_cnt>1;
        return 1;
    }
    if(!solver_state_is_legal(&ptr->state)) return 0;

    // get guess of a grid
    int i,j, guess=0;
    for(i=0;i<9;++i){
        for(j=0;j<9;++j){
            if(node->puzzle[i][j]) continue;
            for(int n=1;n<=9;++n){
                if(bitset_get(ptr->state.note, note_idx(i,j,n))){
                    guess|= (1<<n);
                }
            }
            if(guess) break;
        }
        if(guess) break;
    }
    if(guess==0) return 0;
    // recurse
    for(int n=1;n<=9;++n){
        if((guess&(1<<n))==0) continue;
        Sudoku next;
        memcpy(&next, node, sizeof(Sudoku));
        next.puzzle[i][j]=n;

        if(sudoku_solver_dfs(ptr, &next)) return 1;
    }

    return 0;
}

void sudoku_solver_solve(sudoku_solver *ptr){
    Sudoku data;
    memcpy(&data, ptr->sudoku, sizeof(Sudoku));
    
    // sanity check
    solver_state_init(&ptr->state);
    if(!solver_state_load(&ptr->state, &data)) return;
    if(!solver_state_is_legal(&ptr->state)) return;

    sudoku_solver_dfs(ptr, &data);
}

int solve_sudoku(Sudoku *ptr){
    if(!ptr) return 0;
    
    sudoku_solver ctx;
    ctx.solution_cnt=0;
    ctx.test_multi=0;
    ctx.sudoku=ptr;

    sudoku_solver_solve(&ctx);
    return ctx.solution_cnt>0;
}

int check_sudoku(Sudoku *ptr){
    if(!ptr) return 0;

    sudoku_solver ctx;
    ctx.solution_cnt=0;
    ctx.test_multi=1;
    ctx.sudoku=ptr;
    
    sudoku_solver_solve(&ctx);
    return ctx.solution_cnt;
}
