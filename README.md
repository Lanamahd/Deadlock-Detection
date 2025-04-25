# Deadlock Detection

This project implements a deadlock detection algorithm in C, based on system resource allocation, availability, and request matrices. It simulates a real-world scenario in operating systems where processes compete for limited resources, and helps identify whether the system is in a safe state or if a deadlock has occurred.

---

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [Files in the Repository](#files-in-the-repository)
4. [How It Works](#how-it-works)
5. [Getting Started](#getting-started)
   - [Prerequisites](#prerequisites)
   - [Compiling and Running](#compiling-and-running)
6. [Example Output](#example-output)
7. [EConclusion](#conclusion)

---

## Introduction

Deadlocks occur when processes in a system are waiting indefinitely for resources held by each other. This project aims to detect such conditions using a safe-state algorithm. By analyzing the `Allocation`, `Request`, and `Available` resource matrices, the program determines whether the system is in a safe state or if deadlocks exist.

---

## Features

- **Deadlock Detection Algorithm**:
  - Runs an algorithm to detect deadlocks based on the current resource allocation and request states.
  - Identifies processes that are part of the deadlock, if any.

- **File Parsing**:
  - Reads input matrices (`Allocation`, `Request`, and `Available`) from CSV files.
  - Performs validation to ensure consistent dimensions across files.

- **Safe Sequence Identification**:
  - If the system is in a safe state, the program outputs the safe sequence of process execution.

- **Error Handling**:
  - Detects and handles inconsistent file dimensions or missing data.
  - Provides meaningful error messages for debugging.

---

## Files in the Repository

- **`main.c`**: The main C program implementing the deadlock detection algorithm.
- **`Allocation.csv`**: Contains the allocation matrix for each process.
- **`Request.csv`**: Contains the resource request matrix for each process.
- **`Available.csv`**: Contains the available resource vector.
- **`README.md`**: Documentation for the project.

### Example CSV File Structures

##### Allocation.csv
    ```csv
    Process,ResourceA,ResourceB,ResourceC,ResourceD,ResourceE
    P1,5,6,0,3,1
    P2,1,0,6,5,4
    ...

#### Request.csv
    ```csv
    Process,ResourceA,ResourceB,ResourceC,ResourceD,ResourceE
    P1,2,0,4,5,5
    P2,3,4,2,2,3
    ...

#### Available.csv
    ```csv
    ResourceA,ResourceB,ResourceC,ResourceD,ResourceE
    2,2,2,1,3

---

## How It Works
1. **Input Files**:

- The program reads Allocation.csv, Request.csv, and Available.csv.
- Validates the dimensions of the matrices for consistency.

2. Deadlock Detection Algorithm:

- Uses the Banker's Algorithm to detect whether the system is in a safe state.
- Iteratively checks if processes can be executed with the currently available resources.
- If all processes are executed successfully, the system is in a safe state.
- If not, it identifies processes that are deadlocked.

3. Output:
   - If the system is in a safe state, the program prints the safe sequence.
   - If deadlocks are detected, it lists the processes involved in the deadlock.

---

## Getting Started

### Prerequisites
- A C compiler (e.g., GCC).
- Basic knowledge of operating systems and deadlock detection algorithms.

### Compiling and Running
1. Clone the repository:
    ```bash
    git clone https://github.com/Lanamahd/Deadlock-Detection.git
    cd Deadlock-Detection

2. Compile the program:
   ```bash
   gcc main.c -o deadlock_detection

3. Run the program:
   ```bash
    ./deadlock_detection

Make sure the Allocation.csv, Request.csv, and Available.csv files are in the same directory as the executable.

---

## Example Output

### Case 1: Safe State
CONSISTENT IN ROWS ACROSS ALLOCATION & REQUEST FILES, AND CONSISTENT IN COLUMNS ACROSS ALL FILES.

Allocation.csv DATA:
P1           5           6           0           3           1
...

Request.csv DATA:
P1           2           0           4           5           5
...

Available.csv DATA:
2           2           2           1           3

NO DEADLOCK, THE SAFE SEQUENCE IS: P1 -> P2 -> P3 -> ... -> P10

### Case 2: Deadlock Detected

P4 IS DEADLOCKED.
P7 IS DEADLOCKED.

DEADLOCK IS DETECTED.

--- 

## Conclusion

The Deadlock Detection project provides an effective implementation of a resource allocation system to identify potential deadlocks in operating systems. By analyzing the `Allocation`, `Request`, and `Available` matrices, the program can determine whether the system is in a safe state or identify processes involved in a deadlock. 

This project serves as a practical demonstration of deadlock detection algorithms, including file parsing, validation, and process execution simulation. It is a valuable tool for understanding resource management in operating systems, and its modular design allows for further extensions or integrations into larger systems.
