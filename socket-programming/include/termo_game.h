#ifndef TERMO_GAME_H
#define TERMO_GAME_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

typedef struct termo_game {
    char *word;
    int letter_map[26];
    int word_len;
    char **history;
    int num_tries;
    int max_tries;
} termo_game;

/**
 * Inicializa a struct do jogo, normalizando a palavra para minúsculas
 * e alocando memória para o histórico de tentativas.
 */
termo_game initialize_game(const char *word, int max_tries);

/**
 * Libera toda a memória dinâmica alocada pela struct do jogo.
 */
void destroy_game(termo_game *game);

/**
 * Avalia o palpite fornecido.
 * Retorna um array de inteiros alocado 
 *   0 = Errado 
 *   1 = Posição errada 
 *   2 = Posição correta 
 * Retorna NULL caso o tamanho seja inválido, o palpite contenha caracteres
 * não permitidos ou o número máximo de tentativas já tenha sido atingido (O tp não descreve isso
 * como problema, mas pra mim, essa condição deveria existir. No caso, colocarei número máximo de tentativas
 * igual ao tamanho máximo do inteiro).
 */
int *try_guess(termo_game *game, const char *guess);

/**
 * Converte o array de feedback numérico (0, 1, 2) em uma string legível:
 *   '_' = letra errada
 *   '*' = letra deslocada
 *   MAIÚSCULA = correta
 * Retorna uma string alocada dinamicamente terminada em '\0'.
 */
char *make_clue(const int *game_result, const char *guess);

#endif 