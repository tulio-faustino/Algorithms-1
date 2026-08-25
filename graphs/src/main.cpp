#include <iostream>
#include <string>
#include "../include/graph.hpp"
#include "../include/dfs.hpp"


/*int main() {
    Graph<std::string> g(false);

    std::string v1 = "Belo Horizonte";
    std::string v2 = "Nova Era";
    std::string v3 = "Vitoria";
    std::string v4 = "Rio de Janeiro";
    std::string v5 = "Sao Paulo";
    std::string v6 = "Brasilia";
    std::string v7 = "Salvador";

    g.add_vertex(v1);
    g.add_vertex(v2);
    g.add_vertex(v3);
    g.add_vertex(v4);
    g.add_vertex(v5);
    g.add_vertex(v6);
    g.add_vertex(v7);

    g.add_edge(v1, v2);
    g.add_edge(v2, v3);
    g.add_edge(v1, v4);
    g.add_edge(v1, v5);
    g.add_edge(v4, v5);
    g.add_edge(v1, v6);
    g.add_edge(v6, v7);
    g.add_edge(v3, v7);

    std::cout << "--- Grafo Original ---\n";
    g.print(true);

    g.remove_edge(v1, v4);
    g.remove_edge(v6, v7);
    g.remove_edge(v2, v3);

    std::cout << "\n--- Apos Remover Arestas ---\n";
    g.print(true);

    g.remove_vertex(v1);
    g.remove_vertex(v7);

    std::cout << "\n--- Apos Remover Vertices (Belo Horizonte e Salvador) ---\n";
    g.print(true);

    bool status_edge = g.add_edge(v1, v2);
    std::cout << "\nTentativa de adicionar aresta em vertice removido: " 
              << (status_edge ? "Sucesso" : "Falhou (Correto)") << "\n";

    return 0;
}*/

int main() {
    // Grafo não-direcionado de inteiros
    Graph<int> g;

    // Valores para os vértices (usando variáveis já que a interface pede referências T&)
    int val0 = 10, val1 = 20, val2 = 30, val3 = 40, val4 = 50;

    g.add_vertex(val0); // idx 0
    g.add_vertex(val1); // idx 1
    g.add_vertex(val2); // idx 2
    g.add_vertex(val3); // idx 3
    g.add_vertex(val4); // idx 4

    // Conexões:
    // (10)---(20)---(30)
    //   |      |
    // (40)    (50)
    g.add_edge(val0, val1);
    g.add_edge(val0, val3);
    g.add_edge(val1, val2);
    g.add_edge(val1, val4);

    // Executa a busca em profundidade
    std::vector<dfs_vertex> dfs_res = depth_first_search(g);

    // Imprime o grafo com os dados, tempos e cores
    print_adjacencies_with_colours(g, dfs_res);

    return 0;
}