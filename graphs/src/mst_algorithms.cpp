#include "../include/mst_algorithms.hpp"
#include <queue>

w_list_Graph mst_Kruskal(w_list_Graph &G){
    std::priority_queue<m_edge, std::vector<m_edge>, std::greater<m_edge>> edges_heap; // Edges to be ordered;
    // Adds and orders the edges (O(E*log(E)))
    for (int i = 0; i < G.size(); i++){
        const auto& e_adjList = G.w_edges(i);

        for (int j = 0; j < e_adjList.size(); j++) {
            m_edge c_edge;
            c_edge.u = i;
            c_edge.v = e_adjList[j].first;
            c_edge.weight = e_adjList[j].second;
            
            edges_heap.push(c_edge);
        }
    }

    
}