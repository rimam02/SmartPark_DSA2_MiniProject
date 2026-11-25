# SmartPark – Intelligent Parking Slot Allocation and Management System

A menu-driven C++17 parking management case study demonstrating data structures and algorithms.

## Features

- Vehicle registration with full owner names
- Nearest-slot allocation using a Min Heap (priority queue)
- FIFO waiting queue and automatic allocation when a slot is released
- Vehicle entry and exit tracking
- Vehicle search using `unordered_map` and Binary Search Tree
- Inorder, preorder and postorder BST traversals
- Parking history using a singly linked list
- Parking-slot sorting using Bubble Sort
- Shortest route calculation using Dijkstra's algorithm on a weighted graph
- Closest available-slot pair analysis using Two Pointers
- Available-slot group analysis using Sliding Window
- File persistence using text files

## Project structure

```text
miniproject_SmartPark/
├── src/
│   └── main.cpp
├── data/
│   ├── vehicles.txt
│   ├── parking.txt
│   └── history.txt
├── docs/
├── README.md
└── .gitignore
```

## Requirements

- C++ compiler supporting C++17 (for example, `clang++` or `g++`)
- Terminal / command line

## Compile and run

Run these commands from the project root directory:

```bash
clang++ -std=c++17 src/main.cpp -o smartpark
./smartpark
```

The `data` directory and the three text files should exist before running. Run the executable from the project root because the program uses relative paths such as `data/vehicles.txt`.

## Menu

1. Register Vehicle
2. Allocate Parking Slot
3. View Available Slots
4. Vehicle Entry
5. Vehicle Exit
6. Search Vehicle (Hashing)
7. View Waiting Queue
8. View Parking History (Linked List)
9. Sort Parking Slots by Distance
10. Search Vehicle Using BST
11. Inorder Traversal
12. Preorder Traversal
13. Postorder Traversal
14. Display BST Structure
15. Find Shortest Route (Dijkstra)
16. Find Closest Slot Pair (Two Pointers)
17. Sliding Window Parking Analysis
18. Exit

## Data structures and algorithms

| Concept | Purpose |
|---|---|
| Array | Vehicle and parking-slot storage |
| Hash Map | Average O(1) vehicle lookup |
| FIFO Queue | Waiting vehicle management |
| Min Heap | Select nearest available slot |
| Singly Linked List | Parking event history |
| Binary Search Tree | Vehicle search and traversals |
| Bubble Sort | Sort slots by distance |
| Graph + Dijkstra | Shortest route in the sample parking graph |
| Two Pointers | Closest pair of available slot distances |
| Sliding Window | Lowest-average consecutive group of available slots |

## Data persistence

- `data/vehicles.txt`: vehicle number and owner name
- `data/parking.txt`: slot states and waiting queue
- `data/history.txt`: parking history events

Data is saved during operations and when the Exit menu option is selected.

## Notes

- The system models 10 parking slots and up to 100 registered vehicles.
- Slot distances and graph weights are predefined sample values for algorithm demonstration; they are not live GPS measurements.
- Build and run from the project root so relative data-file paths resolve correctly.

## Project team

- Rima Maji – 150096725215
- Jiya Jana – 150096725167
- Nandini Solanki – 150096725189

Project Guide: Dr. Aarti Pardeshi  
Subject: Data Structures and Algorithms  
Cohort: 2025–2029
