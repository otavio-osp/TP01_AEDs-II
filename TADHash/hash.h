// Integrante 2 - Pedro
// Responsável por: Módulo TAD HASH
// Baseado no livro "Projeto de Algoritmos" (Ziviani) - Capítulo 5 - Hashing Universal

#ifndef HASH_H
#define HASH_H

#include "../TADLista/lista.h"

// Define um tipo de vetor de pesos (usado na função de transformação Hashing Universal)
#define MAX_TAM_PALAVRA 50
typedef int TipoPesos[MAX_TAM_PALAVRA];

// Célula da Tabela Hash com encadeamento (Endereçamento Fechado)
typedef struct NoHash {
    char* Chave; // A palavra lida
    TipoListaOcorrencia Ocorrencias;
    struct NoHash* Prox;
} NoHash;

typedef NoHash* ApontadorNoHash;

typedef struct {
    ApontadorNoHash* VetorListas; // Vetor de Listas Encadeadas
    int tamanho;                  
    
    // Contadores para análise de complexidade
    int comparacoes_insercao;
    int comparacoes_pesquisa;
} TabelaHash;


// Gera os pesos aleatórios que serão usados pelo Hashing Universal 
void GeraPesos(TipoPesos pesos);

// Inicializa a Tabela Hash com o tamanho especificado
void InicializaTabelaHash(TabelaHash* tabela, int tamanho);

// Insere uma chave e seu idDoc na Tabela, usando o vetor de pesos
void InsereTabelaHash(TabelaHash* tabela, const char* chave, int idDoc, TipoPesos pesos);

// Retorna um ponteiro para a lista de ocorrências. Se não encontrar, retorna NULL.
TipoListaOcorrencia* PesquisaTabelaHash(TabelaHash* tabela, const char* chave, TipoPesos pesos);

// Imprime os itens armazenados na tabela Hash em ordem alfabética.
void ImprimeTabelaHashOrdenada(TabelaHash* tabela);

// Libera toda a memória alocada
void LiberaTabelaHash(TabelaHash* tabela);

#endif
