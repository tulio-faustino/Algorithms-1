#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>

#define MSG_SIZE 128

// ---- Protocolo definido na especificação do TP

typedef enum {
    MSG_START,    // Servidor solicita a palavra de acesso
    MSG_GUESS,    // Cliente envia o palpite de 5 letras
    MSG_FEEDBACK, // Servidor retorna as dicas (0, 1 ou 2)
    MSG_WIN,      // Servidor informa que a palavra foi descoberta
    MSG_ERROR,    // Servidor reporta erro de formato/tamanho
    MSG_EXIT      // Encerramento da conexao
} MessageType;

typedef struct {
    int type;                  // Tipo da mensagem (MessageType)
    int guess[5];              // Vetor com as 5 letras enviadas pelo cliente
    int feedback[5];           // Dicas retornadas pelo servidor (2, 1 ou 0)
    int attempts;              // Contador de tentativas realizadas
    int win_status;            // 1 para vitoria, 0 para em jogo, -1 para erro
    char message[MSG_SIZE];    // Mensagem de texto para logs
} GameMessage;

// ---- Funcoes auxiliares para garantir e facilitar o enviar e receber do pacote completo.
static inline int send_all(int sockfd, const void *buf, size_t len) {
    size_t total = 0;
    const char *p = (const char *)buf;
    while (total < len) {
        ssize_t n = send(sockfd, p + total, len - total, 0);
        if (n <= 0) return -1;
        total += n;
    }
    return 0;
}

static inline int recv_all(int sockfd, void *buf, size_t len) {
    size_t total = 0;
    char *p = (char *)buf;
    while (total < len) {
        ssize_t n = recv(sockfd, p + total, len - total, 0);
        if (n <= 0) return -1;
        total += n;
    }
    return 0;
}

#endif