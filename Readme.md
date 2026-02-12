# Maze Solver & Generator (C++)

A terminal-based C++ application that generates random mazes using a Recursive Backtracking algorithm and solves them using Depth-First Search (DFS) with a custom-built stack implementation.

---

##  Features

* **Custom Data Structure:** Implements a template-based `CustomStack` using a Linked List rather than relying on the STL `std::stack`.
* **Maze Generation:** Uses a randomized algorithm to ensure every generated maze is unique and solvable.
* **Real-time Visualization:** Watch the solver navigate the maze in the terminal with a step-by-step animation.
* **Hybrid Modes:** Choose between a predefined static maze or a procedurally generated one with custom dimensions.

---

##  How It Works

### 1. The Stack Implementation
The program uses a custom-built `CustomStack` class. This mimics the behavior of a standard stack (LIFO) but is implemented via a **Singly Linked List**, ensuring efficient memory management and demonstrating core data structure principles.

### 2. Generation Algorithm
The `MazeGenerator` class utilizes a **Recursive Backtracking** approach:
* Starts at a random cell and carves a path to a neighbor two cells away.
* Removes the wall between them.
* Continues until it hits a dead end, then backtracks to find the next available path.

### 3. Solving Algorithm
The `MazeSolver` uses **Depth-First Search (DFS)**:
* It pushes the start position onto the stack.
* It marks the current path with `+`.
* If it hits a dead end, it **backtracks** by popping the stack and clearing the path markers until it finds a new direction.

---

##  Usage

### Prerequisites
* A C++ compiler (e.g., `g++`, `clang`, or MSVC).
* **Windows environment:** The current code uses `system("cls")` for visualization. 
    * *Note: For Linux/macOS, change `system("cls")` to `system("clear")` in the `display` function.*

### Compilation
```bash
g++ -o maze_app maze.cpp
```