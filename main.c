// Integrante 4 - Tom
// Responsável por: UI (Menu Principal) e Integração de todos os módulos


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "IO/io.h"
#include "TADLista/lista.h"
#include "TADHash/hash.h"
#include "TADPatricia/patricia.h"
#include "Pesquisa/pesquisa.h"

#define TAMANHO_HASH 97

int main() {
    int opcao;

    // Variáveis de controle de arquivos e coleção
    int N = 0;                     // Número de documentos
    char** nomes_arquivos = NULL;  // Vetor para armazenar os nomes das fábulas
    int* n_i = NULL;               // Vetor que guarda a quantidade de termos de cada documento i

    // Inicialização das Estruturas de Dados.
    TabelaHash tabela;
    InicializaTabelaHash(&tabela, TAMANHO_HASH);
    
    TipoPesos pesos;
    GeraPesos(pesos); // Pesos necessários para o Hashing Universal do Pedro

    TipoArvore raiz = NULL; // Inicialização da árvore PATRICIA

    // Variáveis para medição de desempenho (Métricas do Relatório)
    struct timeval inicio, fim;
    double tempo_hash, tempo_patricia;

    do {
        printf("         SISTEMA DE BUSCA DE FABULAS - MENU\n");
        printf("1. Receber arquivo de entrada\n");
        printf("2. Construir indices invertidos (PATRICIA e HASH)\n");
        printf("3. Imprimir indices invertidos\n");
        printf("4. Realizar busca por termo(s)\n");
        printf("0. Sair\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch(opcao) {
            case 1:
                printf("\n--- Opcao 1: Receber arquivo de entrada ---\n");
                // Chamar a função (IO/io.h)
                // Ex: ler_arquivo_entrada("entrada.txt");
                break;

            case 2:
                printf("\n--- Opcao 2: Construir indices invertidos ---\n");
                // Chamar as funções para ler os textos e popular a HASH e a PATRICIA
                break;

            case 3:
                printf("\n--- Opcao 3: Imprimir indices invertidos ---\n");
                // Chamar as funções de impressão do TADHash e TADPatricia
                // Ex: imprime_hash(tabela); imprime_patricia(raiz);
                break;

            case 4:
                printf("\n--- Opcao 4: Realizar busca por termo(s) ---\n");
                // Codigo Pesquisa/pesquisa.h:
                // - Receber o(s) termo(s) de busca do usuário.
                // - Calcular a relevância (TF-IDF) de cada documento.
                // - Retornar os textos ordenados pela relevância.
                break;

            case 0:
                printf("\nSaindo do sistema e liberando memoria...\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while(opcao != 0);

    // Função para liberar a memória alocada dinamicamente (free) das listas, hash e árvore.
    LiberaTabelaHash(&tabela);
    
    // Certifique-se que o Gabriel implemente a liberação dos nós da árvore dele
    // LiberaPatricia(raiz); 

    if (n_i != NULL) free(n_i);
    
    if (nomes_arquivos != NULL) {
        for (int i = 0; i < N; i++) {
            free(nomes_arquivos[i]);
        }
        free(nomes_arquivos);
    }

    printf("Ate logo!\n");
    return 0;
}