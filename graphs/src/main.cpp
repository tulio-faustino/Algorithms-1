#include "../include/graph.hpp"

#include <iostream>
#include "graph.hpp" // Ajuste para o nome do seu header

int main() {
    graph g;

    // 1. Cria 4 vértices (assumindo chaves geradas: 0, 1, 2, 3)
    g.add_element(); // Chave 0
    g.add_element(); // Chave 1
    g.add_element(); // Chave 2
    g.add_element(); // Chave 3

    // 2. Conecta os vértices (arestas)
    // 0 -- 1
    // 0 -- 2
    // 1 -- 2
    // 2 -- 3
    g.add_vertex(0, 1);
    g.add_vertex(0, 2);
    g.add_vertex(1, 2);
    g.add_vertex(2, 3);

    // 3. Testa a exibição
    std::cout << "--- Teste de Impressao do Grafo ---\n";
    g.print();

    return 0;
}