#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <exception>

// template <typename T>
// struct data_Graph{

// }

/// @brief Only topological Graph implementation.
// This Graph can be weighted but only in ints. It's implementation can and will be easily ported to weighted float graph in the future, if the course needs to
class Graph{ 
private:
    std::vector<std::vector<int>> adj_matrix;
    bool is_directed;
    int key_idx;

public:
    Graph(bool is_directed = false){
        key_idx = 0;
        is_directed = false;
    };

    int add_vertex(){

        // Adds a new column to each existing row
        for (auto& row : adj_matrix) {
            row.push_back(0);
        }

        // Adds a new row
        adj_matrix.push_back(std::vector<int>(key_idx + 1, 0));

        key_idx++;
        return key_idx;

    };

    // Only strips the vertex of all its adjacencies
    bool remove_vertex(int idx){
        
        if (idx > key_idx)
            return false;
        
        for (size_t i = 0; i < adj_matrix.size(); i++){
            adj_matrix[i][idx] = 0;
            adj_matrix[idx][i] = 0;
        }

        return true;

    }
   
    bool add_edge(int idx1, int idx2, int weight = 1){

        adj_matrix[idx1][idx2] = weight;
        if(!is_directed){
            adj_matrix[idx1][idx2] = weight;
        }
        return true;
    };

    bool remove_edge(int idx1, int idx2){

        if(idx1 > key_idx || idx2 > key_idx)
            return false;

        adj_matrix[idx1][idx2] = 0;
        if(!is_directed){
            adj_matrix[idx2][idx1] = 0;
        }
        return true;
    }
    
    std::vector<int> adj_row(int idx) const{
        return adj_matrix[idx];
    }

    std::vector<int> edges(int idx) const{
        std::vector<int> sub_list;
        for (size_t i = 0; i < key_idx; i++){
            if(adj_matrix[idx][i] != 0){
                sub_list.push_back(i);
            } 
        }
        return sub_list;
    }

    size_t size() const{
        return key_idx;
    }

};



#endif