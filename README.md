# Flight-Network-Management-System
A menu-driven Flight Network Management System implemented in C using Graphs, Linked Lists, and BFS. It dynamically manages airline routes, finds the shortest path between cities, checks connectivity, and generates adjacency and map-style graph outputs using persistent file storage.
# ✈️ Flight Network Management System

A menu-driven Flight Network Management System developed in **C** using **Graphs (Adjacency List)**, **Linked Lists**, **Queue ADT**, and **Breadth-First Search (BFS)**. The system allows users to dynamically manage flight routes, check connectivity, and find the shortest path between cities.

---

## 📌 Features

- ➕ Add Flight Routes
- ➖ Remove Flight Routes
- 🌍 Display Flight Network (Adjacency List)
- 🔍 Check Connectivity using BFS
- 🛫 Find Shortest Flight Path
- 💾 Automatic File Updates
- 📄 Generates Adjacency List Output
- 🗺️ Generates Text-Based Graph Representation

---

## 🛠️ Data Structures Used

- Graph (Adjacency List)
- Linked List
- Queue (Array Implementation)

---

## 🧠 Algorithms Used

- Breadth-First Search (BFS)
- Graph Traversal
- Queue Operations

---

## 📂 Project Structure

```
Flight-Network-Management-System/
│── main.c
│── flight_network.c
│── flight_network.h
│── routes.txt
│── adjacency.txt
│── graph_output.txt
│── output.txt
│── README.md
│── .gitignore
```

---

## 📁 Output Files

| File | Purpose |
|------|----------|
| routes.txt | Stores all flight routes |
| adjacency.txt | Stores adjacency list representation |
| graph_output.txt | Stores map-style text visualization |
| output.txt | Stores BFS and shortest path results |

---

## ▶️ How to Compile

```bash
gcc main.c flight_network.c -o flight_network
```

---

## ▶️ Run

Windows

```bash
flight_network.exe
```

Linux / macOS

```bash
./flight_network
```

---

## 📸 Sample Menu

```
===== FLIGHT NETWORK MENU =====

1. Add Flight Route
2. Remove Flight Route
3. Display Flight Network
4. Check Connectivity (BFS)
5. Find Shortest Path
6. Exit
```

---

## 🎯 Applications

- Airline Route Management
- Transportation Network Analysis
- Logistics & Cargo Planning
- Route Optimization
- Educational Graph Algorithm Demonstration

---

## 🚀 Future Enhancements

- Dijkstra's Algorithm for shortest travel time
- Flight fare calculation
- Airport codes
- Graphical User Interface (GUI)
- Interactive route visualization
- Database integration

---

## 👥 Team Members

- Baby L N
- Pushpa M M

---

## 📜 License

This project is developed for educational and academic purposes.
