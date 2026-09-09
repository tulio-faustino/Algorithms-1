#ifndef DFS_HPP
#define DFS_HPP

#include "../include/graph.hpp"
#include <vector>
#include <string>
#include "../include/json.hpp"

enum Colour {
    WHITE = 0,
    GREY = 1,
    BLACK = 2
};

struct s_vertex {
    int key;
    Colour colour; // 0 for white, 1 for grey and 2 for black
    int pi;        // refers to the last visited vertex, -1 for NIL
    int d;
    int f;

    s_vertex(int _key, Colour _colour = WHITE, int _pi = -1);
};

std::string colour_to_string(Colour c);

void dfs_visit(Graph &G, std::vector<s_vertex> &v, int idx, int &time);

std::vector<s_vertex> depth_first_search(Graph &G);

void print_adjacencies_with_colours(Graph &G, const std::vector<s_vertex> &v);

std::vector<s_vertex> breadth_first_search(Graph &G, int v_idx);

void print_adjacencies_with_colours(Graph &G, const std::vector<s_vertex> &v);

void make_py_print(Graph &G, const std::vector<s_vertex> &v, const std::string &output_f);

#endif // DFS
