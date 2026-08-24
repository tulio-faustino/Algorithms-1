CC = g++
CFLAGS = -g -std=c++20 -Iinclude
TARGET = bin/teste.out
SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp, obj/%.o, $(SRCS))
DIRS := bin obj

all: $(DIRS) $(TARGET)

$(TARGET): $(OBJS) | $(DIRS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

obj/%.o: src/%.cpp | $(DIRS)
	$(CC) $(CFLAGS) -c $< -o $@

$(DIRS):
	mkdir -p $@

# Executa o binário final gerado
.PHONY: run
run: all
	./$(TARGET)

.PHONY: clean
clean:
	rm -rf obj bin

.PHONY: prepare
prepare: clean all