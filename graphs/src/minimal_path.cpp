#include "../include/minimal_path.hpp"
#include "../include/json.hpp"
#include <queue>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdexcept>

d_vertex::d_vertex(int _key, double _dist, int _pi) {
    key = _key;
    dist = _dist;
    pi = _pi;
    done = false;
}

std::vector<d_vertex> dijkstra(Graph &G, int src){
    if(src < 0 || static_cast<size_t>(src) >= G.size())
        throw std::out_of_range("source index out of range");

    std::vector<d_vertex> vertices;
    for(size_t i = 0; i < G.size(); i++){
        vertices.push_back(d_vertex(i));
    }
    vertices[src].dist = 0;

    // Min-heap of (distance, vertex), ordered by the smallest distance
    using q_item = std::pair<double, int>;
    std::priority_queue<q_item, std::vector<q_item>, std::greater<q_item>> heap;
    heap.push({0.0, src});

    while(!heap.empty()){
        int u = heap.top().second;
        heap.pop();

        if(vertices[u].done) continue; // Stale entry from an older relaxation
        vertices[u].done = true;

        // Relaxes all edges leaving u (O(E*log(V)) with the heap)
        for(const auto &[v, w] : G.w_edges(u)){
            if(vertices[u].dist + w < vertices[v].dist){
                vertices[v].dist = vertices[u].dist + w;
                vertices[v].pi = u;
                heap.push({vertices[v].dist, v});
            }
        }
    }

    return vertices;
}

std::vector<int> dijkstra_path(const std::vector<d_vertex> &v, int src, int dest){
    if(dest < 0 || static_cast<size_t>(dest) >= v.size())
        throw std::out_of_range("destination index out of range");

    std::vector<int> path;
    if(v[dest].dist == INF)
        return path; // Unreachable vertex, empty path

    // Walks back through the predecessors until the source
    for(int current = dest; current != -1; current = v[current].pi){
        path.push_back(current);
        if(current == src) break;
    }

    if(path.back() != src)
        return {}; // dest is not connected to src

    std::reverse(path.begin(), path.end());
    return path;
}

void print_dijkstra_paths(const std::vector<d_vertex> &v, int src, const std::vector<int> &dests){
    std::cout << "Origem " << src << " (Dijkstra)\n";

    for(int dest : dests){
        std::vector<int> path = dijkstra_path(v, src, dest);

        std::cout << "  Menor caminho " << src << " -> " << dest << ": ";
        if(path.empty()){
            std::cout << "inalcançável\n";
            continue;
        }

        for(size_t i = 0; i < path.size(); i++){
            if(i != 0) std::cout << " -> ";
            std::cout << path[i];
        }
        std::cout << " (custo: " << v[dest].dist << ")\n";
    }
}

void make_py_print_w(Graph &G, const std::vector<d_vertex> &v, int src,
                     const std::vector<int> &dests, const std::string &output_f){
    if(G.size() != v.size()){
        throw std::runtime_error("Graph dessynced to d_vertex");
    }

    nlohmann::json j;
    j["is_directed"] = G.is_directed();

    j["nodes"] = nlohmann::ordered_json::array();
    for(size_t u = 0; u < v.size(); u++){
        j["nodes"].push_back(v[u].key);
    }

    // Adjacencies and their weights, kept in the same order
    j["edges"] = nlohmann::ordered_json::array();
    j["weights"] = nlohmann::ordered_json::array();
    for(size_t u = 0; u < v.size(); u++){
        nlohmann::json e_list = nlohmann::json::array();
        nlohmann::json w_list = nlohmann::json::array();
        for(const auto &[neighbor, weight] : G.w_edges(u)){
            e_list.push_back(neighbor);
            w_list.push_back(weight);
        }
        j["edges"].push_back(e_list);
        j["weights"].push_back(w_list);
    }

    j["nodes_print_data"] = nlohmann::ordered_json::array();
    for(size_t u = 0; u < v.size(); u++){
        j["nodes_print_data"].push_back({
            {"id", v[u].key},
            {"colour", v[u].done ? "PRETO" : "BRANCO"}, // Preto = vertice fechado pelo dijkstra
            {"pi", v[u].pi},
            {"dist", v[u].dist}
        });
    }

    // Paths asked in print_dijkstra_paths, highlighted by the plot script
    j["shortest_paths"] = nlohmann::ordered_json::array();
    for(int dest : dests){
        j["shortest_paths"].push_back({
            {"src", src},
            {"dest", dest},
            {"dist", v[dest].dist},
            {"path", dijkstra_path(v, src, dest)}
        });
    }

    std::ofstream file(output_f);
    if(!file.is_open()){
        throw std::runtime_error("Could not open file for writing: " + output_f);
    }

    file << j.dump(4);
    file.close();
}
