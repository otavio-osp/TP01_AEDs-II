#ifndef PATRICIA_H
#define PATRICIA_H

#include "../TADLista/lista.h"

// contadores globais pro relatorio de complexidade
extern int comparacoes_patricia_insercao;
extern int comparacoes_patricia_busca; 

// diferencia se o no eh interno ou folha
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
            Index index; // posicao do char q deu diferenca
            char char_comparacao; // guarda o char pra fazer a regra do < ou >=
            TipoArvore esq, dir;
        } NInterno;
        struct {
            char* Chave; // a palavra msm (so fica na folha)
            TipoListaOcorrencia Ocorrencias; // lista de doc IDs
        } NExterno;
    } NO;
} NoPatricia;

// inicializa a arvore vazia
void InicializaPatricia(TipoArvore* t);

// insere a palavra ou so adiciona o idDoc se ela ja existir
void InserePatricia(TipoArvore* t, const char* chave, int idDoc);

// busca a palavra e devolve a lista de docs (null se nao achar)
TipoListaOcorrencia* PesquisaPatricia(TipoArvore t, const char* chave);

// imprime as palavras em ordem alfabetica
void ImprimePatricia(TipoArvore t);

// da free em tudo pra n dar leak no valgrind
void LiberaPatricia(TipoArvore t);

#endif
