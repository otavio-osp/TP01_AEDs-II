// Integrante 4 - Tom
// Responsável por: UI (Menu Principal) e Integração de todos os módulos


#include <stdio.h>
#include <stdlib.h>
#include "IO/io.h"
#include "TADLista/lista.h"
#include "TADHash/hash.h"
#include "TADPatricia/patricia.h"
#include "Pesquisa/pesquisa.h"

int main() {
    int opcao;

    // 1. AQUI: Declare e inicialize as suas variáveis e as estruturas 
    // de dados (como a inicialização da Hash e da raiz da Patricia)

    do {
        printf("         SISTEMA DE BUSCA DE FABULAS - MENU\n");
        printf("1. Receber arquivo de entrada\n");
        printf("2. Construir indices invertidos (PATRICIA e HASH)\n");
        printf("3. Imprimir indices invertidos\n");
        printf("4. Realizar busca por termo(s)\n");
        printf("0. Sair\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);

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
                printf("\nSaindo do sistema... Ate logo!\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while(opcao != 0);

    // 2. Criar funções para liberar a memória alocada dinamicamente (free) das listas, hash e árvore.

    return 0;
}