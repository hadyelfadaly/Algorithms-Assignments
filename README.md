# Algorithms Assignments

Algorithms coursework for Cairo University, Faculty of Computers and Artificial Intelligence, implemented in C++.

## Repository Structure

```text
Assignments/
|- Assignment 1/
|  |- Assignment #1 - Part [A].pdf
|  |- Assignment #1 - Part [B].pdf
|  |- Assignment #1 - Part [C].pdf
|  |- Part A Solution.pdf
|  |- Part B Code/
|  |  |- Task 1 (Fibonacci Series).cpp
|  |  |- Task 2 (Search Algorithms).cpp
|  |  |- Task 3 (Heap, Priority Queue and Heap Sort).cpp
|  |- Part C Research.pdf
|
|- Assignment 2/
|  |- ArcadiaEngine.h
|  |- ArcadiaEngine.cpp
|  |- main_test_students.cpp
|  |- TeamInfo.txt
|  |- CMakeLists.txt
```

## Assignment 1 - Fibonacci, Search, and Heap Algorithms

### Team Members

- Hady Hassan El Fadaly - [Github Profile](https://github.com/hadyelfadaly)
- Mohamed Amr Ahmed - [Github Profile](https://github.com/moharam1)
- Omar Waleed ElSobky - [Github Profile](https://github.com/Omarsobky)

### Features

#### Task 1: Fibonacci Series

- Recursive Fibonacci implementation.
- Matrix exponentiation with divide-and-conquer optimization.
- Iterative dynamic programming approach with O(1) extra space.

#### Task 2: Search Algorithms

- Sequential search (iterative).
- Sequential search (recursive).
- Binary search (iterative).
- Binary search (recursive).
- Interactive input flow to test search behavior on custom arrays.

#### Task 3: Heap, Priority Queue, and Heap Sort

- Max-heap implementation using array/vector storage.
- Heap operations: build heap, heapify, insert, get max, and get min.
- Priority queue implementation on top of heap behavior.
- Heap sort functionality.

### Run Task Files (Assignment 1)

```bash
cd "Assignment 1/Part B Code"

g++ "Task 1 (Fibonacci Series).cpp" -o task1
./task1

g++ "Task 2 (Search Algorithms).cpp" -o task2
./task2

g++ "Task 3 (Heap, Priority Queue and Heap Sort).cpp" -o task3
./task3
```

## Assignment 2 - Arcadia Engine

### Team Members

- Hady Hassan El Fadaly - [Github Profile](https://github.com/hadyelfadaly)
- Mohamed Amr Ahmed - [Github Profile](https://github.com/moharam1)
- Ziad Tarek El Marsafawy - [Github Profile](https://github.com/ziad-91)
- Ibrahim Wael El Noty - [Github Profile](https://github.com/ibrahimelnouty)
- Marwan Sameh El Sayed - [Github Profile](https://github.com/marwanmoafy)
### Part A: Data Structures

- Player Table using double hashing.
- Leaderboard using skip list.
- Auction tree using red-black tree.

### Part B: Dynamic Programming

- Loot split optimization (minimum subset difference).
- Inventory carry value maximization (0/1 knapsack).
- String possibility counting with DP transitions.

### Part C: Graph Algorithms

- Path existence using BFS.
- Minimum bribe cost using Kruskal MST + DSU.
- Sum of shortest-path distances using Floyd-Warshall, output in binary form.

### Part D: Greedy Scheduling

- CPU task scheduling with cooldown intervals via max-heap strategy.

### Testing

- Includes a happy-path student test runner in `main_test_students.cpp`.
- Factory functions expose concrete implementations for the testing harness.

### Build and Run (Assignment 2)

```bash
cd "Assignment 2"
cmake -S . -B build
cmake --build build
./build/Assignment_2
```

## About

This repository contains algorithm-focused assignments covering recursion, dynamic programming, searching, heaps, hashing, skip lists, balanced trees, graph algorithms, and greedy strategies.
