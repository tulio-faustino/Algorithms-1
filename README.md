# Algorithms-1
Repository containing my own study material for my uni course "Agorithms 1" at Federal University of Minas Gerais - Brazil


## ----- Graphs 

Implementation of graph data structures and traversal algorithms in C++, featuring an automated Python pipeline for visualization.

### What's Implemented

* **Depth-First Search (DFS):**
  * Tracks vertex states using 3-color taxonomy (`WHITE`, `GREY`, `BLACK`).
  * Computes discovery time ($d$), finish time ($f$), and predecessor ($\pi$) tree links.
  * Robust handling of connected and disconnected components (forest generation).

* **Visualizer Pipeline (C++ ➔ Python):**
  * **JSON Serializer:** Exports graph topology and DFS metadata seamlessly via modern C++.
  * **Plot Engine:** Automated Python script (`NetworkX` + `Matplotlib`) rendering custom styled nodes, edge directions, and execution metrics.

---

### Preview

<p align="center">
  <img src="docs/dfs_example.png" alt="DFS Visualization" width="600"/>
</p>

---

