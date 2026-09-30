#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "../include/protocol.h"
#include "../include/termo_game.h" 

#define BACKLOG 10

/*** PARA REFERÊNCIA
 struct addrinfo {
    int              ai_flags;     // Opções adicionais (ex: AI_PASSIVE)
    int              ai_family;    // Família: AF_INET (IPv4), AF_INET6 (IPv6) ou AF_UNSPEC
    int              ai_socktype;  // Tipo: SOCK_STREAM (TCP) ou SOCK_DGRAM (UDP)
    int              ai_protocol;  // Protocolo específico (0 deixa o SO escolher)
    size_t           ai_addrlen;   // Tamanho do endereço apontado por ai_addr
    struct sockaddr *ai_addr;      // Ponteiro para o endereço binário real
    char            *ai_canonname; // Nome canônico do host
    struct addrinfo *ai_next;      // Ponteiro para o próximo nó da lista encadeada
    };
  
 */

int main(int argc, char *argv[]) {
    if (argc != 4){
        fprintf(stderr, "Uso: %s <v4|v6> <porta> <palavra>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *proto_str = argv[1];
    const char *port_str  = argv[2];
    const char *secret    = argv[3];

    // Leitura do tipo de IP
    int ai_family;
    const char *modo_str;
    if (strcmp(proto_str, "v4") == 0){
        ai_family = AF_INET;
        modo_str = "IPv4";
    } else if (strcmp(proto_str, "v6") == 0){
        ai_family = AF_INET6;
        modo_str = "IPv6";
    } else{
        fprintf(stderr, "Protocolo invalido: utilize 'v4' ou 'v6'\n");
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = ai_family;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port_str, &hints, &res) != 0){
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    // Faz o bind na porta 
    int server_fd = -1;
    int yes = 1;
    for (p = res; p != NULL; p = p->ai_next){
        server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (server_fd == -1) continue;

        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        // Pra caso seja IPV6-Only
        if (p->ai_family == AF_INET6){
            int ipv6only = 1;
            setsockopt(server_fd, IPPROTO_IPV6, IPV6_V6ONLY, &ipv6only, sizeof(ipv6only));
        }

        if (bind(server_fd, p->ai_addr, p->ai_addrlen) == 0){
            break;
        }
        close(server_fd);
    }
    freeaddrinfo(res);

    if (p == NULL){
        fprintf(stderr, "Falha ao dar bind na porta %s\n", port_str);
        exit(EXIT_FAILURE);
    }

    // Setta o server pra escutar
    if (listen(server_fd, BACKLOG) == -1){
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor iniciado em modo %s na porta %s\n", modo_str, port_str);

    // Loop do programa
    while (1){
        struct sockaddr_storage client_addr;
        socklen_t addr_len = sizeof(client_addr);

        // Aceita a conexão
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd == -1){
            perror("accept");
            continue;
        }

        printf("Cliente Conectado\n");

        // Inicializa o jogo com o parâmetro passado em argv[3] (o secret)
        termo_game game = initialize_game(secret, -1);

        int attempts = 0;

        // Servidor envia MSG_START para comecar
        GameMessage msg;
        memset(&msg, 0, sizeof(msg));
        msg.type = MSG_START;
        if (send_all(client_fd, &msg, sizeof(msg)) == -1) {
            close(client_fd);
            printf("Cliente Desconectado\n");
            continue;
        }

        // Loop de rodadas da partida
        while (1) {
            if (recv_all(client_fd, &msg, sizeof(msg)) == -1) {
                break; // Conexao caiu ou erro
            }

            if (msg.type != MSG_GUESS) {
                break;
            }

            /* ========================================================
             * PROCESSAMENTO DO PALPITE DO CLIENTE
             * msg.guess[0..4] contem os 5 caracteres (em int/ASCII).
             * Converter os inteiros de msg.guess de volta para char.
             * Validar se sao letras de A-Z (ou a-z e fazer toupper).
             * ======================================================== */
            char guess_str[6] = {0};
            int valid = 1;
            for (int i = 0; i < 5; i++) {
                char c = (char)msg.guess[i];
                if (!isalpha(c)) {
                    valid = 0;
                    break;
                }
                guess_str[i] = (char)tolower(c);
            }

            if (!valid) {
                // Erro de formato
                memset(&msg, 0, sizeof(msg));
                msg.type = MSG_ERROR;
                msg.win_status = -1;
                send_all(client_fd, &msg, sizeof(msg));
                continue; // Nao penaliza attempts, aguarda nova tentativa
            }

            attempts++;
            int *resolution = try_guess(&game, guess_str);
            int is_victory = 1; 

            // Faço isso aqui com segurança pq se a string passada não tem 5 letras o programa ja deu pau bem antes.
            for(int i = 0; i < 5; i++){
                if (resolution[i] != 2){
                    is_victory = 0;
                    break;
                }
            }

            

            memset(&msg, 0, sizeof(msg));
            for(int i = 0; i < 5; i++){
                msg.feedback[i] = resolution[i];
            }
            msg.attempts = attempts;

            if (is_victory) {
                msg.type = MSG_WIN;
                msg.win_status = 1;
                send_all(client_fd, &msg, sizeof(msg));
                break; // Jogo encerrado
            } else {
                msg.type = MSG_FEEDBACK;
                msg.win_status = 0;
                send_all(client_fd, &msg, sizeof(msg));
            }
        }

        // Jogo finalizado
        destroy_game(&game);
        close(client_fd);
        printf("Cliente Desconectado\n");
    }

    close(server_fd);
    return 0;
}
