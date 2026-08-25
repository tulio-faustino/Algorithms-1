#include <iostream>
#include <string>
#include "../include/graph.hpp"
#include "../include/dfs.hpp"

int main() {
    // Cria um grafo não-direcionado puramente topológico
    Graph g(false);

    // Adiciona 5 vértices sequencialmente (gerando os índices 0, 1, 2, 3, 4)
    int v0 = g.add_vertex();
    int v1 = g.add_vertex();
    int v2 = g.add_vertex();
    int v3 = g.add_vertex();
    int v4 = g.add_vertex();

    // Conexões:
    // (0)---(1)---(2)
    //  |     |
    // (3)   (4)
    g.add_edge(v0, v1);
    g.add_edge(v0, v3);
    g.add_edge(v1, v2);
    g.add_edge(v1, v4);

    // Executa a busca em profundidade
    std::vector<dfs_vertex> dfs_res = depth_first_search(g);

    // Imprime as adjacências com as cores e tempos
    std::cout << "=== RESULTADO DO DFS ===" << std::endl;
    print_adjacencies_with_colours(g, dfs_res);

    return 0;
}