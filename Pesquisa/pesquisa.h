// Integrante 4 - Tom
// Responsável por: Motor de Busca (TF-IDF), Busca e Ordenação

#ifndef PESQUISA_H
#define PESQUISA_H

#include "../TADLista/lista.h"     
#include "../TADHash/hash.h"       
#include "../TADPatricia/patricia.h" 

// Estrutura auxiliar para armazenar a pontuação de relevância r(i) de um documento. 
// Será usada para ordenar os resultados no final.

typedef struct {
    int idDoc;
    float relevancia;
} DocumentoRelevancia;


// Funçãp principal de pesquisa (chamada no caso 4 do main).


void realizar_busca(char* consulta, TabelaHash tabela, TipoArvore raiz, int N, int* n_i);
// Recebe os termos de busca, pesquisa nos índices e imprime os documentos em ordem de relevância.

// (Cálculo do TF-IDF)

// Calcula o peso (w_j,i) de um termo em um documento específico.
// Fórmula: w_j,i = f_j,i * (log2(N) / d_j)
float calcular_peso_w(int f_ji, int N, int d_j);


// Calcula a relevância final r(i) de um documento para a consulta inteira.
// Fórmula: r(i) = (1 / n_i) * somatorio(w_j,i)
float calcular_relevancia_r(int n_i, float somatorio_pesos);

//Funções Auxiliares

// Ordena o vetor de DocumentoRelevancia de forma DECRESCENTE (maior relevância primeiro).
void ordenar_resultados_por_relevancia(DocumentoRelevancia* resultados, int tamanho);

#endif
