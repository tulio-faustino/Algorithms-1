#ifndef DFS_HPP
#define DFS_HPP

#include "../include/graph.hpp"
#include <iostream>
#include <vector>
#include <string>

enum Colour {
    WHITE = 0,
    GREY = 1,
    BLACK = 2
};

struct dfs_vertex {
    int key;
    Colour colour; // 0 for white, 1 for grey and 2 for black
    int pi;        // refers to the last visited vertex, -1 for NIL
    int d;
    int f;

    dfs_vertex(int _key, Colour _colour, int _pi) {
        key = _key;
        colour = _colour;
        pi = _pi;
        d = 0;
        f = -1;
    }
};

inline std::string colour_to_string(Colour c) {
    switch (c) {
        case WHITE: return "BRANCO";
        case GREY:  return "CINZA";
        case BLACK: return "PRETO";
        default:    return "DESCONHECIDO";
    }
}

void dfs_visit(Graph &G, std::vector<dfs_vertex> &v, int idx, int &time) {
    time++;
    v[idx].d = time;
    v[idx].colour = GREY;

    std::vector<int> adj_v = G.edges(idx);
    for (size_t i = 0; i < adj_v.size(); i++) {
        int u = adj_v[i];
        if (v[u].colour == WHITE) {
            v[u].pi = idx;
            dfs_visit(G, v, u, time);
        }
    }

    time++;
    v[idx].f = time;
    v[idx].colour = BLACK;
}

std::vector<dfs_vertex> depth_first_search(Graph &G) {
    std::vector<dfs_vertex> vertices;
    for (size_t i = 0; i < G.size(); i++) {
        vertices.push_back(dfs_vertex(i, WHITE, -1));
    }

    int time = 0;
    int flag_connected = 0;

    for (size_t i = 0; i < vertices.size(); i++) {
        if (vertices[i].colour == WHITE) {
            dfs_visit(G, vertices, i, time);
            flag_connected++; // If bigger than 1, the graph is not connected
        }
    }

    return vertices;
}

void print_adjacencies_with_colours(Graph &G, const std::vector<dfs_vertex> &v) {
    for (size_t u = 0; u < G.size(); ++u) {
        std::cout << "Vértice " << u 
                  << " [" << colour_to_string(v[u].colour) 
                  << ", d: " << v[u].d << ", f: " << v[u].f << "]:\n";

        std::vector<int> adj = G.edges(u);
        if (adj.empty()) {
            std::cout << "  -> (sem adjacências)\n";
        } else {
            for (int neighbor : adj) {
                std::cout << "  -> Vizinho " << neighbor 
                          << " [" << colour_to_string(v[neighbor].colour) << "]\n";
            }
        }
    }
}

#endif // DFS_HPP