# Integrante 4 - Tom
# Responsável por: Empacotamento e automatização da compilação

CC = gcc
CFLAGS = -Wall -Wextra -g -I.
LDFLAGS = -lm

# ============= Executável principal =============
OBJS = main.o IO/io.o TADLista/lista.o TADHash/hash.o TADPatricia/patricia.o Pesquisa/pesquisa.o
TARGET = tp01_busca

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ============= Teste isolado da Hash =============
TEST_HASH_OBJS = main_hash.o TADLista/lista.o TADHash/hash.o
TEST_HASH_TARGET = test_hash

test_hash: $(TEST_HASH_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_HASH_TARGET) $(TEST_HASH_OBJS) $(LDFLAGS)

# ============= Comandos de execução =============
run: $(TARGET)
	./$(TARGET)

run_test_hash: test_hash
	./$(TEST_HASH_TARGET)

# ============= Limpeza =============
clean:
	rm -f *.o IO/*.o TADLista/*.o TADHash/*.o TADPatricia/*.o Pesquisa/*.o $(TARGET) $(TEST_HASH_TARGET)

.PHONY: all clean run test_hash run_test_hash