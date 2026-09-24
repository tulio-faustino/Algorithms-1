#ifndef MST_HPP
#define MST_HPP

// Minimal Spanning Tree Algorithms
// Content: Kruskal's and Prim's algorithms for finding the minimal spanning tree

#include "graph.hpp"
#include "utilities.hpp"

struct m_edge{ //Edge struct used for mst algs
    double weight;
    int u, v;

    // Garante que a priority_queue funcione como Min-Heap ordenando pelo weight
    bool operator>(const m_edge& other) const {
        return weight > other.weight;
    }
};

w_list_Graph mst_Kruskal(w_list_Graph &G);


#endif