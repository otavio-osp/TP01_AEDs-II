#ifndef PATRICIA_H
#define PATRICIA_H

#include "../TADLista/lista.h"

// Contadores para análise de complexidade (para uso externo)
extern int comparacoes_patricia_insercao;
extern int comparacoes_patricia_busca; 

// Enum para identificar o tipo do nó
typedef enum {
    Interno,
    Externo
} TipoNo;

typedef unsigned int Index;

typedef struct NoPatricia* TipoArvore;

typedef struct NoPatricia {
    TipoNo nt;
    union {
        struct {
            Index index; // Índice do caractere divergente
            char char_comparacao; // Caractere usado para a ramificação (maior caractere)
            TipoArvore esq, dir;
        } NInterno;
        struct {
            char* Chave; // A palavra armazenada
            TipoListaOcorrencia Ocorrencias; // Lista de ocorrências
        } NExterno;
    } NO;
} NoPatricia;

// Inicializa a árvore Patricia
void InicializaPatricia(TipoArvore* t);

// Insere uma palavra na árvore (ou adiciona a ocorrência caso já exista)
void InserePatricia(TipoArvore* t, const char* chave, int idDoc);

// Pesquisa uma palavra na árvore e retorna sua lista de ocorrências (ou NULL se não encontrar)
TipoListaOcorrencia* PesquisaPatricia(TipoArvore t, const char* chave);

// Imprime a árvore (em ordem alfabética)
void ImprimePatricia(TipoArvore t);

// Libera toda a memória associada à árvore Patricia
void LiberaPatricia(TipoArvore t);

#endif
