#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <exception>

template <typename T>
class graph{ //Unwheighted
private:
    std::vector<std::vector<int>> adj_matrix;
    std::unordered_map<T, int> key_map; // maps data to key in O(1)
    std::vector<T> data_vct;            // stores the data, for convinience
    std::vector<bool> is_active;
    bool is_directed;
    int key_idx;

public:
    graph(bool is_directed = false){
        key_idx = 0;
        is_directed = false;
    };

    int add_vertex(T &data){
        if(key_map.find(data) != key_map.end()){
            return key_map[data];
        }

        int new_key = key_map.size();

        // Binds the data to the key (the sequential number it is putt on the graph)
        key_map[data] = new_key;
        data_vct.push_back(data);
        is_active.push_back(true);

        // Adds a new column to each row
        for (auto& row : adj_matrix) {
            row.push_back(0);
        }

        // Adds a new row
        adj_matrix.push_back(std::vector<int>(new_key + 1, 0));

        return new_key;

    };

    bool remove_vertex(T &data){
        auto it = key_map.find(data);
        if (it == key_map.end()) {
            return false;
        }

        int idx = it->second;

        // Zeroes the adjacencies
        for (size_t i = 0; i < adj_matrix.size(); i++){
            adj_matrix[i][idx] = 0;
            adj_matrix[idx][i] = 0;
        }

        // Deactivates the vertex
        is_active[idx] = false;

        // Erases from the key_map so it won't get cought when the code verifies if the data is in the graph
        key_map.erase(it);
        return true;

    }
   
    bool add_edge(T &e1, T &e2){
        // Searches if e1 and e2 exist in the graph
        auto it1 = key_map.find(e1);
        auto it2 = key_map.find(e2);
        if (it1 == key_map.end() || it2 == key_map.end()) {
            return false;
        }
        int e1_idx = it1->second;
        int e2_idx = it2->second;

        adj_matrix[e1_idx][e2_idx] = 1;
        if(!is_directed){
            adj_matrix[e2_idx][e1_idx] = 1;
        }
        return true;
    };

    bool remove_edge(T &e1, T &e2){
        // Searches if e1 and e2 exist in the graph
        auto it1 = key_map.find(e1);
        auto it2 = key_map.find(e2);
        if (it1 == key_map.end() || it2 == key_map.end()) {
            return false;
        }
        int e1_idx = it1->second;
        int e2_idx = it2->second;

        adj_matrix[e1_idx][e2_idx] = 0;
        if(!is_directed){
            adj_matrix[e2_idx][e1_idx] = 0;
        }
        return true;
    }
    
    const void print(bool as_data = false){
        if(!as_data){
             // Skips deactivated vertices

            for (size_t i = 0; i < adj_matrix.size(); ++i) {
                if (!is_active[i]) continue; // Skips deactivated vertices

                std::cout << "Vertex " << i << ": ";

                for (size_t j = 0; j < adj_matrix[i].size(); ++j) {
                    if(adj_matrix[i][j] != 0)
                        std::cout << j << " ";
                }
                std::cout << "\n";
            }
        }else{
            for (size_t i = 0; i < adj_matrix.size(); ++i) {
                if (!is_active[i]) continue; // Skips deactivated vertices

                std::cout << "Vertex " << data_vct[i] << ": ";

                for (size_t j = 0; j < adj_matrix[i].size(); ++j) {
                    if(adj_matrix[i][j] != 0)
                        std::cout << data_vct[j] << ", "; //****  Have to make it prettier in the future, as it is printing with an extra ',' at the end
                }
                std::cout << "\n";
            }
        }
    };
};



#endif