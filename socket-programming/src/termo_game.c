#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "../include/termo_game.h"

#define HISTORY_SIZE 5

termo_game initialize_game(const char *word, int max_tries){
    termo_game game;

    game.max_tries = max_tries;
    game.num_tries = 0;

    // Pega os dados da palavra e registra-os na struct termo_game
    game.word_len = strlen(word); 
    game.word = (char *)malloc((game.word_len + 1) * sizeof(char));

    if(game.word == NULL) {
        exit(-1);
    }

    for(int i = 0; i < game.word_len; i++){
        game.word[i] = tolower((unsigned char)word[i]);
    }
    game.word[game.word_len] = '\0';

    // Aloca no game.history o suficiente, o tamanho máximo do histórico
    game.history = (char **)malloc(HISTORY_SIZE * sizeof(char *));
    if(game.history == NULL) {
        free(game.word);
        exit(-1);
    }

    // Aloca a memória de cada uma das strings de histórico
    for(int i = 0; i < HISTORY_SIZE; i++){
        game.history[i] = (char *)malloc((game.word_len + 1) * sizeof(char));
        if(game.history[i] == NULL) {
            // Em caso de erro, desfaz as alocações anteriores e encerra o programa.
            for(int j = 0; j < i; j++) free(game.history[j]);
            free(game.history);
            free(game.word);
            exit(-1);
        }
        game.history[i][0] = '\0'; // Inicializa como string vazia
    }

    // Inicializa o mapa de caracteres com 0 em tudo
    for(int i = 0; i < 26; i++){
        game.letter_map[i] = 0;
    }

    // Conta as letras da palavra do jogo
    for(int i = 0; i < game.word_len; i++){
        // game.word[i] - 'a' retorna a posição de 0 a 26 da letra, tipo, 'b' - 'a' = 1
        if(game.word[i] >= 'a' && game.word[i] <= 'z')
            game.letter_map[game.word[i] - 'a']++;
    }

    return game;
}

void destroy_game(termo_game *game) {
    if (game == NULL) return;

    //printf("[DEBUG] Liberando word...\n");
    if (game->word != NULL) {
        free(game->word);
        game->word = NULL;
    }

    //printf("[DEBUG] Liberando history (history_size = %d)...\n", HISTORY_SIZE);
    if (game->history != NULL) {
        for (int i = 0; i < HISTORY_SIZE; i++) {
            //printf("[DEBUG] Liberando history[%d] = %p\n", i, (void*)game->history[i]);
            if (game->history[i] != NULL) {
                free(game->history[i]);
                game->history[i] = NULL;
            }
        }
        //printf("[DEBUG] Liberando vetor history...\n");
        free(game->history);
        game->history = NULL;
    }
    //printf("[DEBUG] Destruído com sucesso!\n");
}

int *try_guess(termo_game *game, const char *guess){

    if((int)strlen(guess) != game->word_len){
        return NULL;
        // Há de ser tratado na main
    }

    // Insere, de forma circular no histórico de tentativas (guarda até 5)
    int idx = game->num_tries % HISTORY_SIZE;
    for (int i = 0; i < game->word_len; i++) {
        game->history[idx][i] = (char)tolower((unsigned char)guess[i]);
    }
    game->history[idx][game->word_len] = '\0'; // Finaliza a string na posição correta
    game->num_tries++;
    
    int map_copy[26];
    for(int i = 0; i < 26; i++){
        map_copy[i] = game->letter_map[i];
    }

    int *result = (int*)malloc((game->word_len) * sizeof(int));

    // Primeira passada para verificar se a letra está na posição certa;
    for(int i = 0; i < game->word_len; i++){
        char c = (char)tolower((unsigned char)guess[i]);

        if(c >= 'a' && c <= 'z') // Acho interessante deixar pra colocar _ só no a a z pra deixar poder ter termo com espaço
            result[i] = 0;
        else
            return NULL; // Entrada Inválida

        if(c == game->word[i]) {
            result[i] = 2;
            map_copy[c - 'a']--;
        }
    }
    
    // Segunda passada pra pegar as letras que não estão na posição certa mas existem
    for (int i = 0; i < game->word_len; i++){
        char c = (char)tolower((unsigned char)guess[i]);
        if( c >= 'a' && c <= 'z'){
            if(map_copy[c - 'a'] > 0 && result[i] == 0){
                result[i] = 1;
                map_copy[c - 'a']--;
            }
        }
    }

    return result;
} 

char *make_clue(const int *game_result, const char *guess){
    char *clue = (char*)malloc((strlen(guess) + 1)*sizeof(char));
    clue[strlen(guess)] = '\0';
    for(int i = 0; i < (int)strlen(guess); i++){
        switch (game_result[i]){
            case 0:
                clue[i] = '_';
                break;
            case 1:
                clue[i] = '*';
                break;
            case 2:
                clue[i] = toupper((unsigned char)guess[i]);
                break;
            default:
                //Não é pra existir game_result sem ser com 0 1 ou 2 em todas posições
                return NULL;
        }
    }
    return clue;
}