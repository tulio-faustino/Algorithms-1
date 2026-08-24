#include "../include/graph.hpp"

graph::graph(){
    num_vertices = 0;
};

void graph::add_element(){
    element elmnt = element(num_vertices);
    std::list<element> elmnt_adj;
    elmnt_adj.push_back(elmnt);
    adj.push_back(elmnt_adj);
    num_vertices++;
}

//void graph::remove_element(int key){

//};

void graph::add_vertex(int e1, int e2){
    
    auto it1 = find_vertex(e1);
    auto it2 = find_vertex(e2);

    if (it1 == adj.end() || it2 == adj.end()) {
        return; // Sai do método add_vertex caso algum não exista
    }

    element elmnt2 = it2->front();
    element elmnt1 = it1->front();
    
    it1->push_back(elmnt2);
    it2->push_back(elmnt1);
}

const void graph::print(){
    
    if(adj.empty()){
        return;
    }

    for (const auto& sub_lista : adj) {
        if (!sub_lista.empty()) {
            std::cout << "Vertice: " << sub_lista.front().get_key() << " -> ";

            // Loop aninhado para os vizinhos (pulando o primeiro)
            for (auto it = std::next(sub_lista.begin()); it != sub_lista.end(); ++it) {
                std::cout << it->get_key() << " ";
            }
            std::cout << "\n";
        }
    }
}