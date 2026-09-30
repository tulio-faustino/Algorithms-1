CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -g -Iinclude
# Não funcionava quando havia colocado a flag c11, funcionou quando troquei pra gnu11
SRC_DIR = src
INC_DIR = include

all: client server

client: $(SRC_DIR)/client.c $(INC_DIR)/protocol.h
	$(CC) $(CFLAGS) -o client $(SRC_DIR)/client.c

server: $(SRC_DIR)/server.c $(SRC_DIR)/termo_game.c $(INC_DIR)/protocol.h $(INC_DIR)/termo_game.h
	$(CC) $(CFLAGS) -o server $(SRC_DIR)/server.c $(SRC_DIR)/termo_game.c

clean:
	rm -f client server

.PHONY: all clean