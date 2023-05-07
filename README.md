# 8-Puzzle Solver

## Description

This C++ program is an 8-puzzle solver. It uses search algorithms to find the optimal solution to reach the goal state from a given initial state. The 8-puzzle is a sliding tile puzzle that is made up of a 3x3 grid, with numbered tiles from 1 to 8 and one blank tile. The goal is to slide the tiles to reach the goal state, which is the ordered state with the blank tile in the bottom-right corner.

## Features

- Different search algorithms:
  - Uniform-cost search
  - Misplaced tile heuristic
  - Manhattan distance heuristic
- Handles various test cases with different initial states and depths
- Provides information about the solution depth, number of nodes expanded, and maximum queue size

## Usage

1. Compile the C++ code using a C++ compiler, such as `g++`:

```sh
g++ -o puzzle_solver main.cpp
```

2. Run the compiled program:

```sh
./puzzle_solver
```

3. Follow the prompts to choose the search algorithm and test case to solve.

4. The program will display the best state to expand, the optimal solution, and additional information about the solution.

## Example Output

```
===============================================================
The solution is with a g(n) = 14 and h(n) = 0.
[1  2  3]
[4  5  6]
[7  8  0]
With the Depth Of 14

Solution depth was: 14
Number of nodes expanded: 123
Max queue size: 56
===============================================================
```

## Note

This program requires a C++ compiler with support for C++11 or later.
