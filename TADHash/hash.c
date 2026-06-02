// Integrante 2 - Pedro
// Responsável por: Módulo TAD HASH
// Baseado no livro "Projeto de Algoritmos" (Ziviani) - Capítulo 5 - Hashing Universal

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hash.h"

int FuncaoHash(const char* chave, TipoPesos pesos, int tamanho) {
    int soma = 0;
    int comp = strlen(chave);
    for (int i = 0; i < comp; i++) {
        soma += (int)chave[i] * pesos[i];
    }
    return (soma % tamanho);
}

void GeraPesos(TipoPesos pesos) {
    for (int i = 0; i < MAX_TAM_PALAVRA; i++) {
        pesos[i] = (rand() % 10000) + 1; 
    }
}

void InicializaTabelaHash(TabelaHash* tabela, int tamanho) {
    tabela->tamanho = tamanho;
    tabela->comparacoes_insercao = 0;
    tabela->comparacoes_pesquisa = 0;
    tabela->VetorListas = (ApontadorNoHash*) calloc(tamanho, sizeof(ApontadorNoHash));
}

void InsereTabelaHash(TabelaHash* tabela, const char* chave, int idDoc, TipoPesos pesos) {
    
    int indice = FuncaoHash(chave, pesos, tabela->tamanho);
    
    ApontadorNoHash atual = tabela->VetorListas[indice];
    
    // Vê se a palavra já existe
    while (atual != NULL) {
        tabela->comparacoes_insercao++; 
        if (strcmp(atual->Chave, chave) == 0) {// Se a palavra existe aumenta a ocorrencia
            InsereOcorrencia(&atual->Ocorrencias, idDoc);
            return;
        }
        atual = atual->Prox;
    }
    
    // Se não existe vamos inserir
    ApontadorNoHash novoNo = (ApontadorNoHash) malloc(sizeof(NoHash));
    novoNo->Chave = strdup(chave); 
    
    // Cria uma nova ocorrencia
    FazListaOcorrenciaVazia(&novoNo->Ocorrencias);
    InsereOcorrencia(&novoNo->Ocorrencias, idDoc);
    
    // Insere a nova palavra na lista
    novoNo->Prox = tabela->VetorListas[indice];
    tabela->VetorListas[indice] = novoNo;
}

TipoListaOcorrencia* PesquisaTabelaHash(TabelaHash* tabela, const char* chave, TipoPesos pesos) {
    
    int indice = FuncaoHash(chave, pesos, tabela->tamanho);
    
    ApontadorNoHash atual = tabela->VetorListas[indice];
    
    // Vê se a palavra existe
    while (atual != NULL) {
        tabela->comparacoes_pesquisa++; 
        if (strcmp(atual->Chave, chave) == 0) { // se achou retorna a lista de ocorrências
            return &atual->Ocorrencias;
        }
        atual = atual->Prox;
    }
    
    // se não achar retorna NULL
    return NULL;
}

int comparaPalavras(const void* a, const void* b) {
    ApontadorNoHash noA = *(ApontadorNoHash*)a;
    ApontadorNoHash noB = *(ApontadorNoHash*)b;
    return strcmp(noA->Chave, noB->Chave);
}

void ImprimeTabelaHashOrdenada(TabelaHash* tabela) {
    // Conta quantas palavras existem na Tabela
    int count = 0;
    for (int i = 0; i < tabela->tamanho; i++) {
        ApontadorNoHash atual = tabela->VetorListas[i];
        while (atual != NULL) {
            count++;
            atual = atual->Prox;
        }
    }
    
    // Verifica se a tabela esta vazia
    if (count == 0) return;
    
    // Cria um vetor temporario que caiba todas as palavras
    ApontadorNoHash* vetorPalavras = (ApontadorNoHash*) malloc(count * sizeof(ApontadorNoHash));
    
    int j = 0;
    
    // Armazenar as palavras em um vetor para ordenar
    for (int i = 0; i < tabela->tamanho; i++) {
        ApontadorNoHash atual = tabela->VetorListas[i];
        while (atual != NULL) {
            vetorPalavras[j++] = atual;
            atual = atual->Prox;
        }
    }
    
    qsort(vetorPalavras, count, sizeof(ApontadorNoHash), comparaPalavras);
    
    // printa as palavras
    for (int i = 0; i < count; i++) {
        printf("%s ", vetorPalavras[i]->Chave);
        ImprimeListaOcorrencia(&vetorPalavras[i]->Ocorrencias);
    }
    
    free(vetorPalavras);
}

void LiberaTabelaHash(TabelaHash* tabela) {
    for (int i = 0; i < tabela->tamanho; i++) {
        ApontadorNoHash atual = tabela->VetorListas[i];
        ApontadorNoHash prox;
        while (atual != NULL) {
            prox = atual->Prox;
            LiberaListaOcorrencia(&atual->Ocorrencias);
            free(atual->Chave);
            free(atual);
            atual = prox;
        }
    }
    free(tabela->VetorListas);
    tabela->tamanho = 0;
}
