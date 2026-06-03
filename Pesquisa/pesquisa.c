// Integrante 4 - Tom
// Responsável por: Motor de Busca (TF-IDF), Busca e Ordenação

#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include "pesquisa.h"
#include <string.h>

// Cálculo do TF-IDF

float calcular_peso_w(int f_ji, int N, int d_j) {
    // Se o termo não aparece em nenhum documento, o peso é 0 para evitar divisão por zero.
    if (d_j == 0) {
        return 0.0;
    }
    
    // Fórmula do trabalho: w_j,i = f_j,i * (log2(N) / d_j)
    float log_n = log2((double)N); 
    float peso = (float)f_ji * (log_n / (float)d_j);
    
    return peso;
}

float calcular_relevancia_r(int n_i, float somatorio_pesos) {
    // Se o documento não tem termos (n_i = 0), a relevância é 0
    if (n_i == 0) {
        return 0.0;
    }
    
    // Fórmula do trabalho: r(i) = (1 / n_i) * somatorio(w_j,i)
    float relevancia = (1.0 / (float)n_i) * somatorio_pesos;
    
    return relevancia;
}


// Função Principal de Pesquisa e Ranqueamento

void realizar_busca(char* consulta, TabelaHash* tabela, TipoPesos pesos, int N, int* n_i); {
    // Inicializa o vetor de resultados e o somatório de pesos para cada documento.
    DocumentoRelevancia resultados[N];
    float somatorio_pesos[N];
    for(int i = 0; i < N; i++) {
        resultados[i].idDoc = i + 1; // Assumindo que os idDocs comecem em 1
        resultados[i].relevancia = 0.0;
        somatorio_pesos[i] = 0.0;
    }

    // Separar a string de consulta em palavras individuais (termos de busca).
    // O delimitador " " (espaço) separa as palavras.
    char* termo = strtok(consulta, " ");
    
    while(termo != NULL) {
        
        // INTEGRAÇÃO REAL COM A TABELA HASH

        // Chama a pesquisa da Hash passando a tabela (por referência), o termo e os pesos.

        TipoListaOcorrencia* lista_da_palavra = PesquisaTabelaHash(tabela, termo, pesos); 

        //  Se o resultado não for NULL, a palavra existe em algum documento.
        if (lista_da_palavra != NULL) {
            
            // Obtém o d_j (número de documentos que contém o termo)
            int dj = lista_da_palavra->Tamanho;
            
            // Percorre a lista encadeada padronizada
            TipoCelula *celula = lista_da_palavra->Primeiro->Prox; // Pula o nó cabeça
            
            while(celula != NULL) {
                int id_do_documento = celula->Item.idDoc;
                int f_ji = celula->Item.qtde; // Ocorrências no documento.
                
                // Calcula o peso usando a função matemática (w_j,i).
                float peso = calcular_peso_w(f_ji, N, dj);
                
                // Soma esse peso no acumulador do respectivo documento.
                // (id_do_documento - 1 porque o índice do vetor começa em 0).
                somatorio_pesos[id_do_documento - 1] += peso;
                
                // Avança para o próximo documento da lista.
                celula = celula->Prox;
            }
        }
 
        // Avança para o próximo termo da consulta.
        termo = strtok(NULL, " ");
    }

    // Após iterar por todas as palavras da consulta, calcular a Relevância Final r(i).
    for(int i = 0; i < N; i++) {
        resultados[i].relevancia = calcular_relevancia_r(n_i[i], somatorio_pesos[i]);
    }

    // Ordena os resultados para mostrar os mais relevantes primeiro.
    ordenar_resultados_por_relevancia(resultados, N);

    // Imprime os textos ordenados.
    printf("\n  RESULTADOS DA BUSCA  \n");
    int encontrou_algum = 0;
    for(int i = 0; i < N; i++) {
        if(resultados[i].relevancia > 0) {
            printf("Documento ID %d - Relevancia: %.4f\n", resultados[i].idDoc, resultados[i].relevancia);
            encontrou_algum = 1;
        }
    }
    
    if(!encontrou_algum) {
        printf("Nenhum documento relevante encontrado para a busca.\n");
    }
}

// FUNÇÃO AUXILIAR DE ORDENAÇÃO

void ordenar_resultados_por_relevancia(DocumentoRelevancia* resultados, int tamanho) {
    // qsort(vetor, tamanho, tamanho_do_item, funcao_de_comparacao)
    qsort(resultados, tamanho, sizeof(DocumentoRelevancia), comparar_relevancia);
}

int comparar_relevancia(const void *a, const void *b) {
    DocumentoRelevancia *docA = (DocumentoRelevancia *)a;
    DocumentoRelevancia *docB = (DocumentoRelevancia *)b;

    // ordem decrescente (maior relevância no topo).
    if (docA->relevancia < docB->relevancia) return 1;
    if (docA->relevancia > docB->relevancia) return -1;
    
    return 0; // Se forem iguais
}