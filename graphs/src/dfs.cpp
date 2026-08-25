#include "../include/dfs.hpp"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
#include <exception>

dfs_vertex::dfs_vertex(int _key, Colour _colour, int _pi) {
    key = _key;
    colour = _colour;
    pi = _pi;
    d = 0;
    f = -1;
}

std::string colour_to_string(Colour c) {
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

void make_py_print(Graph &G, const std::vector<dfs_vertex> &v, const std::string &output_f) {
    if (G.size() != v.size()) {
        throw std::runtime_error("Graph dessynced to dfs_vertex");
    }

    nlohmann::json j;
    j["is_directed"] = G.has_direction();

    j["nodes"] = nlohmann::ordered_json::array();
    for (size_t u = 0; u < v.size(); u++){
        j["nodes"].push_back(v[u].key);
    }

    j["edges"] = nlohmann::ordered_json::array();
    for (size_t u = 0; u < v.size(); u++){
        j["edges"].push_back(G.edges(u));
    }

    j["nodes_print_data"] = nlohmann::ordered_json::array();
    for (size_t u = 0; u < v.size(); u++){
        j["nodes_print_data"].push_back({
            {"id", v[u].key},
            {"colour", colour_to_string(v[u].colour)}, // Converte o enum para string legível
            {"pi", v[u].pi},
            {"d", v[u].d},
            {"f", v[u].f}
        });
    }

    std::ofstream file(output_f);
    if (!file.is_open()){
        throw std::runtime_error("Could not open file for writing: " + output_f);
    }
    
    file << j.dump(4);
    file.close();
}