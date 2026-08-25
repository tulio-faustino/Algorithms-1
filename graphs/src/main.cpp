#include <iostream>
#include <cstdlib>
#include <string>
#include "../include/graph.hpp"
#include "../include/dfs.hpp"

int main() {
    // Grafo não-direcionado com 12 vértices
    Graph g(false);

    // Cria os vértices de 0 a 11
    int v[12];
    for (int i = 0; i < 12; ++i) {
        v[i] = g.add_vertex();
    }

    // --- COMPONENTE 1: Ciclo com corda e nó folha (0, 1, 2, 3, 4, 5) ---
    //  (0)---(1)
    //   | \   |
    //   |  \  |
    //  (3)---(2)---(5)
    //   |
    //  (4)
    g.add_edge(v[0], v[1]);
    g.add_edge(v[1], v[2]);
    g.add_edge(v[2], v[3]);
    g.add_edge(v[3], v[0]); // Ciclo 0-1-2-3-0
    g.add_edge(v[0], v[2]); // Corda interna
    g.add_edge(v[3], v[4]); // Folha
    g.add_edge(v[2], v[5]); // Folha

    // --- COMPONENTE 2: Subgrafo triangular ramificado (6, 7, 8, 9, 10) ---
    //        (6)
    //       /   \
    //     (7)---(8)
    //     /       \
    //   (9)       (10)
    g.add_edge(v[0], v[7]);
    g.add_edge(v[6], v[7]);
    g.add_edge(v[7], v[8]);
    g.add_edge(v[8], v[6]); // Triângulo 6-7-8
    g.add_edge(v[7], v[9]);
    g.add_edge(v[8], v[10]);

    // --- COMPONENTE 3: Vértice isolado (11) ---
    // (11) - Não adiciona nenhuma aresta

    g.add_edge(v[8], v[11]);
    // Executa a DFS
    std::vector<dfs_vertex> dfs_res = depth_first_search(g);

    // Exibe no terminal
    std::cout << "=== DFS EM GRAFO DESCONEXO (12 VÉRTICES) ===\n\n";
    print_adjacencies_with_colours(g, dfs_res);

    // Exporta para JSON e chama a visualização Python
    std::string json_path = "bin/graph_data.json";
    make_py_print(g, dfs_res, json_path);

    std::cout << "JSON gerado em " << json_path << ". Abrindo visualizador...\n";
    std::string command = "python3 scripts/plot_graph.py " + json_path;
    std::system(command.c_str());

    return 0;
}