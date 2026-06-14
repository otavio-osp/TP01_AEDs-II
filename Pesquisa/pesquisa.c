// Integrante 4 - Tom
// Responsável por: Motor de Busca (TF-IDF), Busca e Ordenação

#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <string.h>
#include "pesquisa.h"

// ============================================================================
// Cálculo do TF-IDF
// ============================================================================

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

// ============================================================================
// Função de comparação para qsort (declarada ANTES de ser usada)
// ============================================================================

int comparar_relevancia(const void *a, const void *b) {
    DocumentoRelevancia *docA = (DocumentoRelevancia *)a;
    DocumentoRelevancia *docB = (DocumentoRelevancia *)b;

    // Ordem decrescente (maior relevância no topo).
    if (docA->relevancia < docB->relevancia) return 1;
    if (docA->relevancia > docB->relevancia) return -1;
    
    // Desempate por idDoc (menor id primeiro para estabilidade)
    if (docA->idDoc < docB->idDoc) return -1;
    if (docA->idDoc > docB->idDoc) return 1;

    return 0;
}

// ============================================================================
// Função auxiliar de ordenação
// ============================================================================

void ordenar_resultados_por_relevancia(DocumentoRelevancia* resultados, int tamanho) {
    qsort(resultados, tamanho, sizeof(DocumentoRelevancia), comparar_relevancia);
}

// ============================================================================
// Função Principal de Pesquisa e Ranqueamento
// ============================================================================

void realizar_busca(char* consulta, TabelaHash* tabela, TipoPesos pesos, int N, int* n_i, char** nomes_arquivos) {
    // Inicializa o vetor de resultados e o somatório de pesos para cada documento.
    DocumentoRelevancia* resultados = (DocumentoRelevancia*) malloc(N * sizeof(DocumentoRelevancia));
    float* somatorio_pesos = (float*) calloc(N, sizeof(float));

    if (resultados == NULL || somatorio_pesos == NULL) {
        printf("Erro: Falha na alocacao de memoria para a busca.\n");
        free(resultados);
        free(somatorio_pesos);
        return;
    }

    for (int i = 0; i < N; i++) {
        resultados[i].idDoc = i + 1; // idDocs começam em 1
        resultados[i].relevancia = 0.0;
    }

    // Conta quantos termos de busca foram informados
    int num_termos_busca = 0;

    // Separar a string de consulta em palavras individuais (termos de busca).
    char* termo = strtok(consulta, " ");
    
    while (termo != NULL) {
        num_termos_busca++;
        
        // INTEGRAÇÃO COM A TABELA HASH
        // Chama a pesquisa da Hash passando a tabela, o termo e os pesos.
        TipoListaOcorrencia* lista_da_palavra = PesquisaTabelaHash(tabela, termo, pesos); 

        // Se o resultado não for NULL, a palavra existe em algum documento.
        if (lista_da_palavra != NULL) {
            
            // Obtém o d_j (número de documentos que contém o termo)
            int dj = lista_da_palavra->Tamanho;
            
            // Percorre a lista encadeada de ocorrências (pula o nó cabeça)
            TipoCelulaOcorrencia *celula = lista_da_palavra->Primeiro->Prox;
            
            while (celula != NULL) {
                int id_do_documento = celula->Item.idDoc;
                int f_ji = celula->Item.qtde; // Ocorrências no documento.
                
                // Calcula o peso usando a função matemática (w_j,i).
                float peso = calcular_peso_w(f_ji, N, dj);
                
                // Soma esse peso no acumulador do respectivo documento.
                // (id_do_documento - 1 porque o índice do vetor começa em 0).
                if (id_do_documento >= 1 && id_do_documento <= N) {
                    somatorio_pesos[id_do_documento - 1] += peso;
                }
                
                // Avança para o próximo documento da lista.
                celula = celula->Prox;
            }
        }
 
        // Avança para o próximo termo da consulta.
        termo = strtok(NULL, " ");
    }

    // Verifica se algum termo foi informado
    if (num_termos_busca == 0) {
        printf("\nNenhum termo de busca informado.\n");
        free(resultados);
        free(somatorio_pesos);
        return;
    }

    // Após iterar por todas as palavras da consulta, calcular a Relevância Final r(i).
    for (int i = 0; i < N; i++) {
        resultados[i].relevancia = calcular_relevancia_r(n_i[i], somatorio_pesos[i]);
    }

    // Ordena os resultados para mostrar os mais relevantes primeiro.
    ordenar_resultados_por_relevancia(resultados, N);

    // ========================================================================
    // Impressão dos resultados
    // ========================================================================
    printf("\n");
    printf("========================================\n");
    printf("       RESULTADOS DA BUSCA\n");
    printf("========================================\n");

    int encontrou_algum = 0;
    int posicao = 1;

    for (int i = 0; i < N; i++) {
        if (resultados[i].relevancia > 0) {
            int idx = resultados[i].idDoc - 1;
            printf(" %d. [Doc %d] %-20s  Relevancia: %.4f\n", 
                   posicao, 
                   resultados[i].idDoc, 
                   (nomes_arquivos != NULL) ? nomes_arquivos[idx] : "(sem nome)",
                   resultados[i].relevancia);
            encontrou_algum = 1;
            posicao++;
        }
    }
    
    if (!encontrou_algum) {
        printf(" Nenhum documento relevante encontrado para a busca.\n");
    }

    printf("========================================\n");
    printf(" Termos pesquisados: %d\n", num_termos_busca);
    printf(" Comparacoes na pesquisa (Hash): %d\n", tabela->comparacoes_pesquisa);
    printf("========================================\n\n");

    // Libera memória alocada
    free(resultados);
    free(somatorio_pesos);
}