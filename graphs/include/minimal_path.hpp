#ifndef MINIMAL_PATH_HPP
#define MINIMAL_PATH_HPP

// Minimal Path Algorithms
// Content: Dijkstra's algorithm for single source shortest paths

#include "graph.hpp"
#include <vector>
#include <string>
#include <limits>

inline constexpr double INF = std::numeric_limits<double>::infinity();

struct d_vertex{ //Vertex struct used for dijkstra algs
    int key;
    double dist; // Shortest distance found so far, INF if unreachable
    int pi;      // Refers to the predecessor vertex, -1 for NIL
    bool done;   // Already removed from the priority queue

    d_vertex(int _key, double _dist = INF, int _pi = -1);
};

// Works with any Graph that implements w_edges (w_list_Graph for now)
std::vector<d_vertex> dijkstra(Graph &G, int src);

// Rebuilds the path src -> dest from the dijkstra result. Empty if unreachable
std::vector<int> dijkstra_path(const std::vector<d_vertex> &v, int src, int dest);

// Prints the smallest path from src to each destination on the screen
void print_dijkstra_paths(const std::vector<d_vertex> &v, int src, const std::vector<int> &dests);

// Exports a weighted graph plus the dijkstra result to JSON, for scripts/plot_graph.py
void make_py_print_w(Graph &G, const std::vector<d_vertex> &v, int src,
                     const std::vector<int> &dests, const std::string &output_f);

#endif
