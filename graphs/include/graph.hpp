#ifndef GRAPH_HPP
#define GRAPH_HPP

//#include <unordered_map>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <exception>
#include <stdexcept>
//#include <utility>

class Graph {
protected:
    bool directed; // 1 for true 0 for false
    int key_idx;

public:
    Graph(bool directed = false) : directed(directed), key_idx(0) {}
    virtual ~Graph() = default;

    virtual int add_vertex() = 0;
    virtual bool remove_vertex(int idx) = 0;
    virtual bool add_edge(int idx1, int idx2, int weight = 1) = 0;
    virtual bool remove_edge(int idx1, int idx2) = 0;
    virtual std::vector<int> edges(int idx) const = 0; // Have to change to pair to be able to have weighted list graphs.
                                                       // And add a adj_list to return the same as this

    bool is_directed() const {
        return directed;
    }

    size_t size() const {
        return static_cast<size_t>(key_idx);
    }
};

/// @brief Only topological Graph implementation.
// This Graph can be weighted but only in ints. It's implementation can and will be easily ported to weighted float graph in the future, if the course needs to
class matrix_Graph : public Graph{ 
private:
    std::vector<std::vector<int>> adj_matrix;

public:
    matrix_Graph(bool directed = false) : Graph(directed) {}

    int add_vertex() override{

        // Adds a new column to each existing row
        for (auto& row : adj_matrix) {
            row.push_back(0);
        }

        // Adds a new row
        adj_matrix.push_back(std::vector<int>(key_idx + 1, 0));

        return key_idx++;

    };

    // Only strips the vertex of all its adjacencies
    bool remove_vertex(int idx) override{
        
        // if (idx > key_idx)
        //     return false;
        
        // for (size_t i = 0; i < adj_matrix.size(); i++){
        //     adj_matrix[i][idx] = 0;
        //     adj_matrix[idx][i] = 0;
        // }

        // return true;

        return false;
        //Thinking if I'm going to have this function or not.
    }
   
    bool add_edge(int idx1, int idx2, int weight = 1) override{

        adj_matrix[idx1][idx2] = weight;
        if(!directed){
            adj_matrix[idx1][idx2] = weight;
        }
        return true;
    };

    bool remove_edge(int idx1, int idx2) override{

        if(idx1 > key_idx || idx2 > key_idx)
            return false;

        adj_matrix[idx1][idx2] = 0;
        if(!directed){
            adj_matrix[idx2][idx1] = 0;
        }
        return true;
    }

    std::vector<int> edges(int idx) const override{
        std::vector<int> sub_list;
        for (size_t i = 0; i < key_idx; i++){
            if(adj_matrix[idx][i] != 0){
                sub_list.push_back(i);
            } 
        }
        return sub_list;
    }

    std::vector<int> adj_row(int idx) const{
        return adj_matrix[idx];
    }

};

class list_Graph : public Graph{
private:
    std::vector<std::vector<int>> adj_list;

public:
    list_Graph(bool directed = false) : Graph(directed) {}

    int add_vertex() override{
        std::vector<int> nv_adj_list;
        adj_list.push_back(nv_adj_list);
        key_idx++;
        return key_idx - 1;
    };

    bool remove_vertex(int idx) override {return 0;};

    bool add_edge(int idx1, int idx2, int weight = 1) override{
        weight = 1; // Even if its suposed to be weighted. Gonna check how to do weighted list graphs later
        // Considering that there isn't a remove_vertex. Which is the case, for now.
        if (idx1 > key_idx || idx2 > key_idx){
            return false;
        }

        if (std::find(adj_list[idx1].begin(), adj_list[idx1].end(), idx2) == adj_list[idx1].end()) {
            adj_list[idx1].push_back(idx2);
        }
        if(!directed && idx1 != idx2){
            if (std::find(adj_list[idx2].begin(), adj_list[idx2].end(), idx1) == adj_list[idx2].end()) {
                adj_list[idx2].push_back(idx1);
            }
        }
        return true;
    }

    bool remove_edge(int idx1, int idx2) override{

        // Considering that there isn't a remove_vertex. Which is the case, for now.
        if (idx1 > key_idx || idx2 > key_idx){
            return false;
        }

        adj_list[idx1].erase(
            std::remove(adj_list[idx1].begin(), adj_list[idx1].end(), idx2),
            adj_list[idx1].end()
        );

        if (!directed && idx1 != idx2) {
            adj_list[idx2].erase(
                std::remove(adj_list[idx2].begin(), adj_list[idx2].end(), idx1),
                adj_list[idx2].end()
            );
        }
        return true;
    };

    std::vector<int> edges(int idx) const override {

        if (idx > key_idx){
            throw std::out_of_range("vertex index out of range");
        }

        return adj_list[idx];
    };
};

#endif