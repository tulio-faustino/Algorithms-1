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
    bool is_directed;
    int key_idx;

public:
    graph(bool is_directed = false){
        key_idx = 0;
        is_directed = false;
    };

    int add_vertex(T &_data){
        if(key_map.find(_data) != key_map.end()){
            return key_map[_data];
        }

        int new_key = key_map.size();

        // Binds the data to the key (the sequential number it is putt on the graph)
        key_map[_data] = new_key;
        data_vct.push_back(_data);

        // Adds a new column to each row
        for (auto& row : adj_matrix) {
            row.push_back(0);
        }

        // Adds a new row
        adj_matrix.push_back(std::vector<int>(new_key + 1, 0));

        return new_key;

    };
   
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
            for (size_t i = 0; i < adj_matrix.size(); ++i) {
            std::cout << "Vertice " << i << ": ";

            for (size_t j = 0; j < adj_matrix[i].size(); ++j) {
                if(adj_matrix[i][j] != 0)
                    std::cout << j << " ";
            }
            std::cout << "\n";
            }
        }else{
            for (size_t i = 0; i < adj_matrix.size(); ++i) {
            std::cout << "Vertice " << data_vct[i] << ": ";

            for (size_t j = 0; j < adj_matrix[i].size(); ++j) {
                if(adj_matrix[i][j] != 0)
                    std::cout << data_vct[j] << ", ";
            }
            std::cout << "\n";
            }
        }
    };
};



#endif