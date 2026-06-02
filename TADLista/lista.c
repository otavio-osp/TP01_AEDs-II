// Integrante 2 - Pedro
// Responsável por: Estruturas Base (Lista Encadeada de ocorrências para hash)
// Baseado no livro "Projeto de Algoritmos" (Ziviani) - Capítulo 3

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

void FazListaOcorrenciaVazia(TipoListaOcorrencia* Lista) {
    Lista->Primeiro = (TipoApontadorOcorrencia) malloc(sizeof(TipoCelulaOcorrencia)); // Célula cabeça
    Lista->Primeiro->Prox = NULL;
    Lista->Ultimo = Lista->Primeiro;
    Lista->Tamanho = 0;
}

void InsereOcorrencia(TipoListaOcorrencia* Lista, int idDoc) {
    TipoApontadorOcorrencia atual = Lista->Primeiro->Prox;
    TipoApontadorOcorrencia anterior = Lista->Primeiro;
    
    // Procura se o idDoc já existe para apenas incrementar
    while (atual != NULL && atual->Item.idDoc < idDoc) {
        anterior = atual;
        atual = atual->Prox;
    }
    
    // Se o documento já existe na lista, apenas incrementa a contagem (qtde)
    if (atual != NULL && atual->Item.idDoc == idDoc) {
        atual->Item.qtde++;
        return;
    }
    
    // Se não encontrou ou é um novo idDoc maior, insere na posição correta para manter ordenado
    TipoApontadorOcorrencia nova = (TipoApontadorOcorrencia) malloc(sizeof(TipoCelulaOcorrencia));
    nova->Item.idDoc = idDoc;
    nova->Item.qtde = 1;
    nova->Prox = atual;
    anterior->Prox = nova;
    
    // Se inseriu no final, atualiza o ponteiro 'Ultimo'
    if (atual == NULL) {
        Lista->Ultimo = nova;
    }
    
    Lista->Tamanho++;
}

void LiberaListaOcorrencia(TipoListaOcorrencia* Lista) {
    TipoApontadorOcorrencia atual = Lista->Primeiro;
    TipoApontadorOcorrencia prox;
    while (atual != NULL) {
        prox = atual->Prox;
        free(atual);
        atual = prox;
    }
    Lista->Primeiro = NULL;
    Lista->Ultimo = NULL;
    Lista->Tamanho = 0;
}

void ImprimeListaOcorrencia(TipoListaOcorrencia* Lista) {
    TipoApontadorOcorrencia atual = Lista->Primeiro->Prox;
    while (atual != NULL) {
        printf("<%d, %d> ", atual->Item.qtde, atual->Item.idDoc);
        atual = atual->Prox;
    }
    printf("\n");
}
