/*#include "../include/graph.hpp"
#include <string>

 Pseudocode for DFS

DFS(G)
1   for each vertex u in G.V
2       u.color = WHITE
3       u.pi = NIL
4   time = 0
5   for each vertex u in G.V
6       if u.color == WHITE
7           DFS-VISIT(G, u)


DFS-VISIT(G, u)
1   time = time + 1
2   u.d = time
3   u.color = GRAY
4   for each vertex v in G.Adj[u]
5       if v.color == WHITE
6           v.pi = u
7           DFS-VISIT(G, v)
8   time = time + 1
9   u.f = time
10  u.color = BLACK

//

enum Colour {
    WHITE = 0,
    GREY = 1,
    BLACK = 2
};

struct dfs_vertex{
    int key;
    Colour colour; // 0 for white, 1 for grey and 2 for black
    int pi; // refers to the last visited vertex, -1 for NIL
    int d;
    int f;

    dfs_vertex(int _key, Colour _colour, int _pi){
        key = _key;
        colour = _colour;
        pi = _pi;
        d = 0;
        f = -1;
    }
};

template <typename T>
void dfs_visit(Graph<T> &G, std::vector<dfs_vertex> &v, int idx, int& time){
    time++;
    v[idx].d = *time;
    v[idx].colour = GRAY;
    std::vector<int> adj_v = G.edges(idx);
    for(size_t = i; i < adj_v.size(); i++){
        if(v[adj_v[i]].colour == WHITE){
            v[adj_v[i]].pi = idx;
            dfs_visit(G, v, i, time)
        }
    }
    time++;
    v[idx].f = *time;
    v[idx].colour = BLACK;

}

template <typename T>
void depth_first_search(Graph<T> &G){
    std::vector<dfs_vertex> vertices;
    for(size_t i = 0; i < G.size(); i++){
        vertices.push_back(dfs_vertex(i, WHITE, -1))
    }

    int time = 0;

    int flag_connected = 0;
    for(size_t i = 0; i < vertices.size(); i++){
        if (vertices[i].colour == WHITE){
            dfs_visit(G, vertices, i, time);
            flag_connected++; // If bigger then 1, the graph is not connected
        }
    }

};

// AI generated print function:
std::string colour_to_string(Colour c) {
    switch (c) {
        case WHITE: return "BRANCO";
        case GREY:  return "CINZA";
        case BLACK: return "PRETO";
        default:    return "DESCONHECIDO";
    }
}

// Função para imprimir cada vértice, seus vizinhos e as cores correspondentes
template <typename T>
void print_adjacencies_with_colours(Graph<T> &G, const std::vector<dfs_vertex> &v) {
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
*/