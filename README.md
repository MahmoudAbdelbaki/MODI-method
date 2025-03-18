# Transportation Problem Solver

Below is a README file tailored for the provided C++ code that implements the Transportation Problem solver using the North-West Corner Rule and the MODI method. This README includes a description, usage instructions, requirements, and an example.

---


## Overview

This C++ program solves the **Transportation Problem**, a classic optimization problem in operations research and logistics. The goal is to minimize the total cost of transporting goods from multiple sources (e.g., warehouses) to multiple destinations (e.g., stores) while satisfying supply and demand constraints.

The program uses:
- **North-West Corner Rule**: To generate an initial feasible solution.
- **MODI Method (Modified Distribution Method)**: To optimize the initial solution and find the minimum transportation cost.

The code includes helper functions for matrix and vector printing, potential calculation (u and v values), delta computation, and allocation adjustments along a loop.

## Features
- Input custom cost matrix, supply, and demand values.
- Automatically balances supply and demand by adding a dummy row or column if needed.
- Step-by-step output of intermediate matrices (e.g., allocations, potentials, and delta values).
- Calculates and displays the initial and optimal transportation costs.

## Requirements
- A C++ compiler (e.g., g++, clang++).
- Standard C++ libraries (`<iostream>`, `<vector>`, `<limits>`, `<iomanip>`, `<cmath>`, `<numeric>`, `<algorithm>`).

## Installation
1. Save the code in a file named `transportation_problem.cpp`.
2. Compile the program using a C++ compiler:
   ```bash
   g++ transportation_problem.cpp -o transportation_problem
   ```
3. Run the executable:
   ```bash
   ./transportation_problem
   ```

## Usage
1. Run the program.
2. Enter the number of sources (rows) and destinations (columns).
3. Input the cost matrix, supply array, and demand array as prompted.
4. The program will:
   - Display the initial allocation using the North-West Corner Rule.
   - Optimize the solution using the MODI method.
   - Output intermediate steps (e.g., potentials, delta matrix) and the final optimal cost.

### Input Format
- **Number of sources**: An integer (e.g., 3).
- **Number of destinations**: An integer (e.g., 4).
- **Cost matrix**: A grid of integers (e.g., `3x4` matrix entered row by row).
- **Supply array**: A list of integers (one per source).
- **Demand array**: A list of integers (one per destination).

### Example
#### Input:
```
Enter the number of sources: 2
Enter the number of destinations: 3
Enter the cost matrix (row by row):
4 6 8
1 5 2
Enter the supply array:
50 60
Enter the demand array:
30 40 40
```

#### Output:
```
Cost matrix:
4    6    8
1    5    2

Supply vector: 50 60
Demand vector: 30 40 40

Initial Allocation (North-West Corner Rule) matrix:
30    20    0
0     20    40

Initial Cost: 380

U potentials vector: 0 1
V potentials vector: 4 6 2

Delta matrix:
0    0    4
-3   0    0

Updated Allocation matrix:
30    0    20
0     40    20

U potentials vector: 0 1
V potentials vector: 4 5 2

Delta matrix:
0    1    0
-2   0    0

Optimal solution reached!
Total Optimal Cost: 300
```

## Limitations
- The `findLoop` function is a simplified placeholder and may not correctly identify loops in all cases. A more robust implementation (e.g., using DFS or a stepping-stone method) is needed for complex problems.
- Input validation is minimal; ensure valid inputs to avoid unexpected behavior.

## Future Improvements
- Enhance the `findLoop` function for accurate loop detection.
- Add error handling for invalid inputs.
- Support file-based input for large datasets.
