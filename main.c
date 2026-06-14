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
#define MAX_PALAVRA 100
#define MAX_STOPWORDS 300
#define CAMINHO_FABULAS "Fabulas/"

// ============================================================================
// Funções auxiliares de pré-processamento (integração enquanto módulo IO não 
// está pronto — quando o Otávio implementar, migrar para IO/io.c)
// ============================================================================

// Converte uma string para minúsculas (ASCII)
void converter_para_minuscula(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Remove acentuação simples substituindo caracteres acentuados por seus equivalentes ASCII.
// Trata codificação UTF-8 (sequências de 2 bytes comuns em português).
void remover_acentuacao(char* str) {
    char buffer[MAX_LINHA];
    int j = 0;

    for (int i = 0; str[i] != '\0' && j < MAX_LINHA - 1; ) {
        unsigned char c = (unsigned char)str[i];

        // Caractere ASCII normal
        if (c < 0x80) {
            buffer[j++] = str[i++];
        }
        // Sequência UTF-8 de 2 bytes (0xC0-0xDF)
        else if (c >= 0xC0 && c <= 0xDF && str[i+1] != '\0') {
            unsigned char c2 = (unsigned char)str[i+1];
            unsigned int codepoint = ((c & 0x1F) << 6) | (c2 & 0x3F);

            switch (codepoint) {
                // a com acentos
                case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4:
                    buffer[j++] = 'a'; break;
                case 0xC0: case 0xC1: case 0xC2: case 0xC3: case 0xC4:
                    buffer[j++] = 'a'; break;
                // e com acentos
                case 0xE8: case 0xE9: case 0xEA: case 0xEB:
                    buffer[j++] = 'e'; break;
                case 0xC8: case 0xC9: case 0xCA: case 0xCB:
                    buffer[j++] = 'e'; break;
                // i com acentos
                case 0xEC: case 0xED: case 0xEE: case 0xEF:
                    buffer[j++] = 'i'; break;
                case 0xCC: case 0xCD: case 0xCE: case 0xCF:
                    buffer[j++] = 'i'; break;
                // o com acentos
                case 0xF2: case 0xF3: case 0xF4: case 0xF5: case 0xF6:
                    buffer[j++] = 'o'; break;
                case 0xD2: case 0xD3: case 0xD4: case 0xD5: case 0xD6:
                    buffer[j++] = 'o'; break;
                // u com acentos
                case 0xF9: case 0xFA: case 0xFB: case 0xFC:
                    buffer[j++] = 'u'; break;
                case 0xD9: case 0xDA: case 0xDB: case 0xDC:
                    buffer[j++] = 'u'; break;
                // c cedilha
                case 0xE7:
                    buffer[j++] = 'c'; break;
                case 0xC7:
                    buffer[j++] = 'c'; break;
                default:
                    // Caractere desconhecido: pula
                    break;
            }
            i += 2;
        }
        // Sequência UTF-8 de 3+ bytes: pula
        else {
            int bytes = 1;
            if (c >= 0xE0 && c <= 0xEF) bytes = 3;
            else if (c >= 0xF0 && c <= 0xF7) bytes = 4;
            i += bytes;
        }
    }
    buffer[j] = '\0';
    strcpy(str, buffer);
}

// Verifica se um caractere é uma letra (a-z, A-Z)
int eh_letra(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Carrega o arquivo de stop words em um vetor de strings.
// Retorna a quantidade de stop words carregadas.
int carregar_stopwords(const char* caminho, char stopwords[][MAX_PALAVRA]) {
    FILE* arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        printf("Aviso: Arquivo de stop words '%s' nao encontrado. Continuando sem filtro.\n", caminho);
        return 0;
    }

    int count = 0;
    char linha[MAX_PALAVRA];
    while (fgets(linha, MAX_PALAVRA, arquivo) != NULL && count < MAX_STOPWORDS) {
        // Remove \n e \r
        linha[strcspn(linha, "\r\n")] = '\0';
        converter_para_minuscula(linha);
        if (strlen(linha) > 0) {
            strcpy(stopwords[count], linha);
            count++;
        }
    }
    fclose(arquivo);
    return count;
}

// Verifica se uma palavra é uma stop word
int eh_stopword(const char* palavra, char stopwords[][MAX_PALAVRA], int num_stopwords) {
    for (int i = 0; i < num_stopwords; i++) {
        if (strcmp(palavra, stopwords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// ============================================================================
// Função de leitura do arquivo de entrada
// ============================================================================

// Lê o arquivo entrada.txt e retorna o número N de documentos e os nomes dos arquivos.
int ler_arquivo_entrada(const char* caminho, int* N, char*** nomes_arquivos) {
    FILE* arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo '%s'.\n", caminho);
        return 0;
    }

    // Lê N (número de documentos)
    if (fscanf(arquivo, "%d", N) != 1 || *N <= 0) {
        printf("Erro: Formato invalido no arquivo de entrada.\n");
        fclose(arquivo);
        return 0;
    }
    // Consome o \n restante
    fgetc(arquivo);

    // Aloca vetor de nomes
    *nomes_arquivos = (char**) malloc((*N) * sizeof(char*));
    if (*nomes_arquivos == NULL) {
        printf("Erro: Falha na alocacao de memoria.\n");
        fclose(arquivo);
        return 0;
    }

    char linha[MAX_LINHA];
    int lidos = 0;
    while (lidos < *N && fgets(linha, MAX_LINHA, arquivo) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        if (strlen(linha) > 0) {
            (*nomes_arquivos)[lidos] = strdup(linha);
            lidos++;
        }
    }

    fclose(arquivo);

    if (lidos < *N) {
        printf("Aviso: Esperava %d nomes de arquivo, mas encontrou apenas %d.\n", *N, lidos);
        *N = lidos;
    }

    return 1;
}

// ============================================================================
// Função de construção dos índices invertidos
// ============================================================================

// Lê cada fábula, pré-processa o texto e insere as palavras na tabela hash.
// Preenche o vetor n_i com a contagem de termos por documento.
int construir_indices(char** nomes_arquivos, int N, TabelaHash* tabela, TipoPesos pesos, int** n_i) {
    // Carrega stop words
    char stopwords[MAX_STOPWORDS][MAX_PALAVRA];
    int num_stopwords = carregar_stopwords("stopwords.txt", stopwords);
    printf("Stop words carregadas: %d\n", num_stopwords);

    // Aloca o vetor n_i (quantidade de termos distintos por documento)
    *n_i = (int*) calloc(N, sizeof(int));
    if (*n_i == NULL) {
        printf("Erro: Falha na alocacao de memoria para n_i.\n");
        return 0;
    }

    for (int doc = 0; doc < N; doc++) {
        // Monta o caminho completo do arquivo
        char caminho_completo[MAX_LINHA];
        snprintf(caminho_completo, MAX_LINHA, "%s%s", CAMINHO_FABULAS, nomes_arquivos[doc]);

        FILE* arquivo = fopen(caminho_completo, "r");
        if (arquivo == NULL) {
            printf("Aviso: Nao foi possivel abrir '%s'. Pulando documento %d.\n", 
                   caminho_completo, doc + 1);
            continue;
        }

        char linha[MAX_LINHA];
        int termos_no_doc = 0;

        while (fgets(linha, MAX_LINHA, arquivo) != NULL) {
            // Pré-processamento: remove acentuação e converte para minúsculas
            remover_acentuacao(linha);
            converter_para_minuscula(linha);

            // Extrai palavras da linha (somente caracteres alfabéticos)
            int i = 0;
            while (linha[i] != '\0') {
                // Pula não-letras
                if (!eh_letra(linha[i])) {
                    i++;
                    continue;
                }

                // Lê uma palavra
                char palavra[MAX_PALAVRA];
                int j = 0;
                while (eh_letra(linha[i]) && j < MAX_PALAVRA - 1) {
                    palavra[j++] = linha[i++];
                }
                palavra[j] = '\0';

                // Ignora palavras muito curtas (1 caractere) e stop words
                if (strlen(palavra) <= 1) continue;
                if (eh_stopword(palavra, stopwords, num_stopwords)) continue;

                // Insere na tabela hash: palavra + id do documento (1-indexed)
                InsereTabelaHash(tabela, palavra, doc + 1, pesos);
                termos_no_doc++;
            }
        }

        fclose(arquivo);
        (*n_i)[doc] = termos_no_doc;
        printf("  Doc %02d: %-20s -> %d termos indexados\n", doc + 1, nomes_arquivos[doc], termos_no_doc);
    }

    return 1;
}

// ============================================================================
// MAIN - Menu Principal e Integração
// ============================================================================

int main() {
    int opcao;

    // Variáveis de controle de arquivos e coleção
    int N = 0;                     // Número de documentos
    char** nomes_arquivos = NULL;  // Vetor para armazenar os nomes das fábulas
    int* n_i = NULL;               // Vetor que guarda a quantidade de termos de cada documento i

    // Flags de estado do sistema
    int entrada_carregada = 0;
    int indices_construidos = 0;

    // Inicialização das Estruturas de Dados.
    TabelaHash tabela;
    InicializaTabelaHash(&tabela, TAMANHO_HASH);
    
    TipoPesos pesos;
    srand((unsigned int)time(NULL));  // Seed para geração de pesos aleatórios
    GeraPesos(pesos); // Pesos necessários para o Hashing Universal do Pedro

    TipoArvore raiz = NULL; // Inicialização da árvore PATRICIA (stub)

    // Variáveis para medição de desempenho
    clock_t t_inicio, t_fim;
    double tempo_gasto;

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
        getchar(); // Limpa o buffer do teclado

        switch(opcao) {
            case 1:
                printf("\n--- Opcao 1: Receber arquivo de entrada ---\n");

                // Libera dados anteriores se já foram carregados
                if (nomes_arquivos != NULL) {
                    for (int i = 0; i < N; i++) {
                        free(nomes_arquivos[i]);
                    }
                    free(nomes_arquivos);
                    nomes_arquivos = NULL;
                }
                N = 0;
                entrada_carregada = 0;
                indices_construidos = 0;

                // Lê o arquivo de entrada
                char caminho_entrada[MAX_LINHA];
                snprintf(caminho_entrada, MAX_LINHA, "%sentrada.txt", CAMINHO_FABULAS);

                if (ler_arquivo_entrada(caminho_entrada, &N, &nomes_arquivos)) {
                    printf("\nArquivo de entrada lido com sucesso!\n");
                    printf("Numero de documentos: %d\n", N);
                    printf("Arquivos encontrados:\n");
                    for (int i = 0; i < N; i++) {
                        printf("  %02d. %s\n", i + 1, nomes_arquivos[i]);
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
                if (construir_indices(nomes_arquivos, N, &tabela, pesos, &n_i)) {
                    t_fim = clock();
                    tempo_gasto = ((double)(t_fim - t_inicio)) / CLOCKS_PER_SEC;

                    printf("\nIndices construidos com sucesso!\n");
                    printf("Tempo de construcao (Hash): %.4f segundos\n", tempo_gasto);
                    printf("Comparacoes na insercao: %d\n", tabela.comparacoes_insercao);
                    indices_construidos = 1;
                }
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

                // Pré-processa a consulta (mesmas regras dos documentos)
                remover_acentuacao(consulta);
                converter_para_minuscula(consulta);

                if (strlen(consulta) == 0) {
                    printf("Nenhum termo informado.\n");
                    break;
                }

                t_inicio = clock();
                realizar_busca(consulta, &tabela, pesos, N, n_i, nomes_arquivos);
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

    // ========================================================================
    // Liberação de memória
    // ========================================================================
    LiberaTabelaHash(&tabela);

    // Quando o Gabriel implementar: LiberaPatricia(raiz);
    (void)raiz; // Suprime warning de variável não utilizada

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