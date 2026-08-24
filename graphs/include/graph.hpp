#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <list>
#include <vector>
#include <algorithm>
#include <iostream>


struct element{
    int key;
    // here, would go more stuff. I am just making it into an struct because in the future, for my dfs to work on other graph forms, 
    // I will make it so it knows keys, not direct ints, so I can modularize.
    element(int _key){
        key = _key;
    }
    int get_key() const {return key;};
};

class graph{
private:
    std::list<std::list<element>> adj;
    int num_vertices;
    std::list<std::list<element>>::iterator find_vertex(int key) {
        return std::find_if(adj.begin(), adj.end(), [key](const std::list<element>& sub) {
            return !sub.empty() && sub.front().get_key() == key;
        });
    }

public:
    graph();
    void add_element();
    //void remove_element(int key);
    void add_vertex(int e1, int e2);
    //void remove_vertex(int e1, int e2);
    //void print_adj(int key);
    const void print();

    //std::list<element> get_adj(int key);

};

#endif