#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "protocol.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <endereco_ip> <porta>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_ip   = argv[1];
    const char *server_port = argv[2];

    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Aceita conectar em IPv4 ou IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    if (getaddrinfo(server_ip, server_port, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    // Conecta na porta do servidor
    int client_fd = -1;
    for (p = res; p != NULL; p = p->ai_next) {
        client_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (client_fd == -1) continue;

        if (connect(client_fd, p->ai_addr, p->ai_addrlen) == 0) {
            break;
        }
        close(client_fd);
    }
    freeaddrinfo(res);

    if (p == NULL) {
        fprintf(stderr, "Falha na conexao com o servidor %s:%s\n", server_ip, server_port);
        exit(EXIT_FAILURE);
    }

    // Aguarda MSG_START do servidor
    GameMessage msg;
    if (recv_all(client_fd, &msg, sizeof(msg)) == -1 || msg.type != MSG_START) {
        fprintf(stderr, "Erro ao iniciar sessao com o servidor.\n");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // Começo do game loop
    char input_buf[128];
    while (1) {
        printf("Insira seu palpite:\n> ");
        if (!fgets(input_buf, sizeof(input_buf), stdin)) {
            break;
        }

        // Remove quebra de linha (\n ou \r\n)
        input_buf[strcspn(input_buf, "\r\n")] = '\0';

        // Validacao local basica (se o tamanho for errado)
        int len = strlen(input_buf);
        if (len != 5) {
            printf("ERRO: Insira uma sequência de 5 caracteres de A a Z!\n");
            continue;
        }

        memset(&msg, 0, sizeof(msg));
        msg.type = MSG_GUESS;
        for (int i = 0; i < 5; i++) {
            msg.guess[i] = (int)input_buf[i];
        }
        // Envia ao server a mensagem
        if (send_all(client_fd, &msg, sizeof(msg)) == -1) {
            break;
        }

        // Recebe do server a mensagem de feedback
        if (recv_all(client_fd, &msg, sizeof(msg)) == -1) {
            break;
        }

        // Mensagem de erro de formato/tamanho (tamanho não chegaria até aqui, mas ...)
        if (msg.type == MSG_ERROR || msg.win_status == -1) {
            printf("ERRO: Insira uma sequência de 5 caracteres de A a Z!\n");
            continue;
        }

        if (msg.type == MSG_FEEDBACK) {
            // Imprime dica padronizada: "Dica: <simbolos>"
            printf("Dica: ");
            for (int i = 0; i < 5; i++) {
                if (msg.feedback[i] == 2) {
                    printf("%c", (char)toupper((unsigned char)input_buf[i]));
                } else if (msg.feedback[i] == 1) {
                    printf("*");
                } else {
                    printf("_");
                }
            }
            printf("\n");
            printf("Tentativas realizadas: %d\n", msg.attempts);
        } else if (msg.type == MSG_WIN || msg.win_status == 1) {
            printf("Parabéns! Você venceu!\n");
            break;
        }
    }

    close(client_fd);
    return 0;
}
