#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/termo_game.h"

#define MAX_WORD_BUFFER 128

static void print_help(void) {
    printf("\nLegenda do feedback:\n");
    printf("  [LETRA MAIÚSCULA] : Posição correta (verde)\n");
    printf("  [*]               : Letra existe na palavra, mas em outra posição (amarelo)\n");
    printf("  [_]               : Letra não existe na palavra (cinza)\n\n");
}

int main(void) {
    const char *palavra_secreta = "sagaz";
    const int max_tentativas = 6;

    termo_game jogo = initialize_game(palavra_secreta, max_tentativas);

    printf("=========================================\n");
    printf("         BEM-VINDO AO TERMO EM C         \n");
    printf("=========================================\n");
    printf("Palavra com %d letras. Você tem %d tentativas.\n", jogo.word_len, jogo.max_tries);
    print_help();

    char input[MAX_WORD_BUFFER];
    int acertou = 0;

    while (jogo.num_tries < jogo.max_tries && !acertou) {
        printf("[Tentativa %d/%d] Digite seu palpite: ", jogo.num_tries + 1, jogo.max_tries);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nEntrada interrompida.\n");
            break;
        }

        // Remove quebra de linha (\n ou \r\n) do final
        input[strcspn(input, "\r\n")] = '\0';

        // Validação de feedback do jogo
        int *feedback = try_guess(&jogo, input);
        if (feedback == NULL) {
            printf(">> Entrada inválida! Digite exatamente %d letras sem acentos/símbolos.\n\n", jogo.word_len);
            continue;
        }

        char *clue = make_clue(feedback, input);

        printf("Feedback : %s\n", clue);

        // Verifica condição de vitória (todas as posições são 2)
        int acertos = 0;
        for (int i = 0; i < jogo.word_len; i++) {
            if (feedback[i] == 2) {
                acertos++;
            }
        }

        if (acertos == jogo.word_len) {
            acertou = 1;
        }

        // Libera os buffers temporários alocados na rodada
        free(feedback);
        free(clue);

        printf("-----------------------------------------\n");
    }

    if (acertou) {
        printf("\nParabéns! Você acertou a palavra '%s' em %d tentativas!\n", jogo.word, jogo.num_tries);
    } else {
        printf("\nFim de jogo! Você esgotou suas tentativas. A palavra correta era: '%s'\n", jogo.word);
    }

    destroy_game(&jogo);
    return 0;
}