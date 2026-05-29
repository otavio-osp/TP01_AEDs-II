# Integrante 4 - Tom
# Responsável por: Empacotamento e automatização da compilação

CC = gcc
CFLAGS = -Wall -Wextra -g -I.

OBJS = main.o IO/io.o TADLista/lista.o TADHash/hash.o TADPatricia/patricia.o Pesquisa/pesquisa.o
TARGET = tp01_busca

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o IO/*.o TADLista/*.o TADHash/*.o TADPatricia/*.o Pesquisa/*.o $(TARGET)
