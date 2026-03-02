# BreakSudokuC

C version of [BreakSudoku](https://github.com/CubeMan940331/BreakSudoku)

Because I want this program to run on a CPU on FPGA,
there are some difference between the C++ version to save memory.

- `solver_state` instance is shared across the dfs recursion
- using bit packing to store notes
- avoid heap allocation
- minimal dependency
