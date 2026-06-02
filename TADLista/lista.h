// Integrante 2 - Pedro
// Responsável por: Estruturas Base (Lista Encadeada de ocorrências para hash)
// Baseado no livro "Projeto de Algoritmos" (Ziviani) - Capítulo 3

#ifndef LISTA_H
#define LISTA_H

// Estrutura que guarda os dados de uma ocorrência (no Ziviani chama-se TipoItem)
typedef struct {
    int idDoc;
    int qtde;
} TipoItemOcorrencia;

typedef struct TipoCelulaOcorrencia* TipoApontadorOcorrencia;

// Célula da lista encadeada
typedef struct TipoCelulaOcorrencia {
    TipoItemOcorrencia Item;
    TipoApontadorOcorrencia Prox;
} TipoCelulaOcorrencia;

// Lista de Ocorrências (mantém ponteiro para início e fim para facilitar inserção)
typedef struct {
    TipoApontadorOcorrencia Primeiro, Ultimo;
    int Tamanho;
} TipoListaOcorrencia;

void FazListaOcorrenciaVazia(TipoListaOcorrencia* Lista);
void InsereOcorrencia(TipoListaOcorrencia* Lista, int idDoc);
void LiberaListaOcorrencia(TipoListaOcorrencia* Lista);
void ImprimeListaOcorrencia(TipoListaOcorrencia* Lista);

#endif
