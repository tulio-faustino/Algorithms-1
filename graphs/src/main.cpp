#include <iostream>
#include <string>
#include "../include/graph.hpp"

int main() {
    graph<std::string> g(false);

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

    return 0;
}