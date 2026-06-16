// Integrante 4 - Tom
// Responsável por: UI (Menu Principal) e Integração de todos os módulos

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "IO/io.h"
#include "TADLista/lista.h"
#include "TADHash/hash.h"
#include "TADPatricia/patricia.h"
#include "Pesquisa/pesquisa.h"

#define TAMANHO_HASH 97
#define MAX_LINHA 1024
#define CAMINHO_FABULAS "Fabulas"

// Calcula n_i (total de termos indexados) para cada documento
// a partir dos dados já inseridos na tabela hash.
int* calcular_n_i(TabelaHash* tabela, int N) {
    int* n_i = (int*) calloc(N, sizeof(int));
    if (n_i == NULL) return NULL;

    for (int i = 0; i < tabela->tamanho; i++) {
        ApontadorNoHash no = tabela->VetorListas[i];
        while (no != NULL) {
            TipoCelulaOcorrencia* cel = no->Ocorrencias.Primeiro->Prox;
            while (cel != NULL) {
                int doc = cel->Item.idDoc;
                if (doc >= 0 && doc < N) {
                    n_i[doc] += cel->Item.qtde;
                }
                cel = cel->Prox;
            }
            no = no->Prox;
        }
    }
    return n_i;
}

// MAIN - Menu Principal e Integração

int main() {
    int opcao;

    // Estruturas do módulo IO
    Colecao colecao;
    colecao.N = 0;
    StopWords sw;
    sw.N = 0;

    int* n_i = NULL;

    // Flags de estado do sistema
    int entrada_carregada = 0;
    int indices_construidos = 0;

    // Inicialização das Estruturas de Dados
    TabelaHash tabela;
    InicializaTabelaHash(&tabela, TAMANHO_HASH);

    TipoPesos pesos;
    srand((unsigned int)time(NULL));
    GeraPesos(pesos);

    TipoArvore raiz = NULL; // Stub da árvore PATRICIA

    // Variáveis para medição de desempenho
    clock_t t_inicio, t_fim;
    double tempo_gasto;

    // Carrega stop words uma vez no início
    if (!CarregaStopWords("stopwords.txt", &sw)) {
        printf("Aviso: Nao foi possivel carregar stop words. Continuando sem filtro.\n");
    } else {
        printf("Stop words carregadas: %d\n", sw.N);
    }

    do {
        printf("\n");
        printf("============================================\n");
        printf("   SISTEMA DE BUSCA DE FABULAS - MENU\n");
        printf("============================================\n");
        printf(" 1. Receber arquivo de entrada\n");
        printf(" 2. Construir indices invertidos (HASH)\n");
        printf(" 3. Imprimir indices invertidos\n");
        printf(" 4. Realizar busca por termo(s)\n");
        printf(" 0. Sair\n");
        printf("--------------------------------------------\n");
        printf(" Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1:
                printf("\n--- Opcao 1: Receber arquivo de entrada ---\n");

                // Reseta estado
                colecao.N = 0;
                entrada_carregada = 0;
                indices_construidos = 0;

                if (LerArquivoEntrada("Fabulas/entrada.txt", &colecao)) {
                    printf("\nArquivo de entrada lido com sucesso!\n");
                    printf("Numero de documentos: %d\n", colecao.N);
                    printf("Arquivos encontrados:\n");
                    for (int i = 0; i < colecao.N; i++) {
                        printf("  %02d. %s\n", i + 1, colecao.nomes[i]);
                    }
                    entrada_carregada = 1;
                }
                break;

            case 2:
                printf("\n--- Opcao 2: Construir indices invertidos ---\n");

                if (!entrada_carregada) {
                    printf("Erro: Execute a opcao 1 primeiro para carregar o arquivo de entrada.\n");
                    break;
                }

                // Reinicializa a tabela hash se já foi usada
                if (indices_construidos) {
                    LiberaTabelaHash(&tabela);
                    InicializaTabelaHash(&tabela, TAMANHO_HASH);
                    if (n_i != NULL) { free(n_i); n_i = NULL; }
                }

                t_inicio = clock();

                printf("Construindo indices invertidos (Hash)...\n\n");
                ProcessaColecaoHash(CAMINHO_FABULAS, &colecao, &sw, &tabela, pesos);

                // Calcula n_i a partir da hash já populada
                n_i = calcular_n_i(&tabela, colecao.N);
                if (n_i == NULL) {
                    printf("Erro: Falha na alocacao de memoria para n_i.\n");
                    break;
                }

                // Imprime resumo por documento
                for (int i = 0; i < colecao.N; i++) {
                    printf("  Doc %02d: %-20s -> %d termos indexados\n", i + 1, colecao.nomes[i], n_i[i]);
                }

                t_fim = clock();
                tempo_gasto = ((double)(t_fim - t_inicio)) / CLOCKS_PER_SEC;

                printf("\nIndices construidos com sucesso!\n");
                printf("Tempo de construcao (Hash): %.4f segundos\n", tempo_gasto);
                printf("Comparacoes na insercao: %d\n", tabela.comparacoes_insercao);
                indices_construidos = 1;
                break;

            case 3:
                printf("\n--- Opcao 3: Imprimir indices invertidos ---\n");

                if (!indices_construidos) {
                    printf("Erro: Execute a opcao 2 primeiro para construir os indices.\n");
                    break;
                }

                printf("\n=== INDICE INVERTIDO (TABELA HASH) ===\n");
                printf("(Formato: palavra <qtde, idDoc>)\n\n");
                ImprimeTabelaHashOrdenada(&tabela);
                printf("\n=== FIM DO INDICE ===\n");
                break;

            case 4:
                printf("\n--- Opcao 4: Realizar busca por termo(s) ---\n");

                if (!indices_construidos) {
                    printf("Erro: Execute a opcao 2 primeiro para construir os indices.\n");
                    break;
                }

                // Zera o contador de comparações para medir a pesquisa atual
                tabela.comparacoes_pesquisa = 0;

                char consulta[MAX_LINHA];
                printf("Digite os termos de busca (separados por espaco): ");
                fgets(consulta, MAX_LINHA, stdin);
                consulta[strcspn(consulta, "\r\n")] = '\0';

                // Pré-processa a consulta com o mesmo pipeline dos documentos
                // (aplica palavra por palavra pois PreprocessaPalavra é para uma palavra)
                {
                    char consulta_limpa[MAX_LINHA] = "";
                    char copia[MAX_LINHA];
                    strcpy(copia, consulta);
                    char* tok = strtok(copia, " ");
                    while (tok != NULL) {
                        PreprocessaPalavra(tok);
                        if (tok[0] != '\0') {
                            if (strlen(consulta_limpa) > 0) strcat(consulta_limpa, " ");
                            strcat(consulta_limpa, tok);
                        }
                        tok = strtok(NULL, " ");
                    }
                    strcpy(consulta, consulta_limpa);
                }

                if (strlen(consulta) == 0) {
                    printf("Nenhum termo informado.\n");
                    break;
                }

                t_inicio = clock();
                realizar_busca(consulta, &tabela, pesos, &colecao, n_i);
                t_fim = clock();
                tempo_gasto = ((double)(t_fim - t_inicio)) / CLOCKS_PER_SEC;
                printf("Tempo da busca: %.4f segundos\n", tempo_gasto);
                break;

            case 0:
                printf("\nSaindo do sistema e liberando memoria...\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while(opcao != 0);

    // Liberação de memória
    LiberaTabelaHash(&tabela);

    // Quando o Gabriel implementar: LiberaPatricia(raiz);
    (void)raiz;

    if (n_i != NULL) free(n_i);

    printf("Ate logo!\n");
    return 0;
}