#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "../include/graph.hpp"
#include "../include/search_algorithms.hpp"


int main(int argc, char* argv[]) {

    std::string filename;
    if(argc > 1){
        filename = argv[1];
    }else{
        std::cout << "please type the input file path";
        std::cin >> filename;
    }

    std::ifstream file(filename);
    if(!file.is_open()){
        std::cout << "file not passed correctly";
        return 0;
    }
    int num_vertices;
    int graph_type;
    bool graph_direction;

    if(!(file >> graph_type >> graph_direction >> num_vertices)){
        std::cout << "header incorrect";
        return 0;
    } 

    Graph* g = nullptr;

    // Add a method to initialize the graph with the vertices already later
    if (graph_type == 0){
        g = new list_Graph(graph_direction);
    }else if (graph_type == 1){
        g = new matrix_Graph(graph_direction);
    }else{
        std::cout << "Type of graph passed wrong";
        return 0;
    }

    for(size_t i = 0; i < num_vertices; i++){
        g->add_vertex();
    }

    int v1, v2;
    while(file >> v1 >> v2){
        g->add_edge(v1, v2);
    }

    std::string operation;
    std::vector<s_vertex> searched_vertices;
    int search_starter;


    std::cout << "What operation do you want do to?\n";
    while (std::cin >> operation){
        
        if(operation == "bfs"){
            std::cin >> search_starter;
            searched_vertices = breadth_first_search(*g, search_starter);
            break;
        }else if(operation == "dfs"){
            searched_vertices = depth_first_search(*g);
            break;
        }else if(operation == "i_dfs"){
            //Indexed depth first search;
            return 0;
            break;
        }else{
            std::cout << "invalid operation. The correct ones are: \nbfs - breadth first search \ndfs - depth first search\n i_dfs - indexed depth first search\nDo you want to proceed with the program and try again? [Y] [N]\n";
            bool try_again;
            std::cin >> try_again;
            if(tolower(try_again) == 'n'){
                std::cout << "Programa encerrado";
                return 0;
            }else if(tolower(try_again) == 'y'){
                std::cout << "What operation do you want do to?\n";
                continue;
            }

        }
    }
    //print_adjacencies_with_colours(*g, Breadth_first_search(*g, 3));

    // Exporta para JSON e chama a visualização Python
    std::string json_path = "bin/graph_data.json";
    make_py_print(*g, searched_vertices, json_path);

    std::cout << "JSON gerado em " << json_path << ". Abrindo visualizador...\n";
    std::string command = "python3 scripts/plot_graph.py " + json_path;
    std::system(command.c_str());

    delete g;
    return 0;
}