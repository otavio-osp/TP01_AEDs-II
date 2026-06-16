// Integrante 1 - Otávio
// Responsável por: I/O, Leitura de Arquivos e Pré-processamento de Texto

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "io.h"


void ParaMinusculas(char* texto) {
    for (int i = 0; texto[i]; i++) {
        texto[i] = (char)tolower((unsigned char)texto[i]);
    }
}

void RemovePontuacao(char* texto) {
    char* src = texto;
    char* dst = texto;

    while (*src) {
        if (isalpha((unsigned char)*src) || *src == ' ' || *src == '\n') {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';
}

void PreprocessaPalavra(char* palavra) {
    ParaMinusculas(palavra);
    RemovePontuacao(palavra);
}

// Leitura do arquivo de entrada

int LerArquivoEntrada(const char* caminhoEntrada, Colecao* colecao) {
    FILE* fp = fopen(caminhoEntrada, "r");
    if (fp == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s'\n", caminhoEntrada);
        return 0;
    }

    if (fscanf(fp, "%d", &colecao->N) != 1) {
        fprintf(stderr, "Erro: formato invalido em '%s'\n", caminhoEntrada);
        fclose(fp);
        return 0;
    }

    if (colecao->N > MAX_DOCUMENTOS) {
        fprintf(stderr, "Erro: numero de documentos (%d) excede o maximo (%d)\n",
                colecao->N, MAX_DOCUMENTOS);
        fclose(fp);
        return 0;
    }

    for (int i = 0; i < colecao->N; i++) {
        if (fscanf(fp, "%99s", colecao->nomes[i]) != 1) {
            fprintf(stderr, "Erro: esperava %d nomes, leu apenas %d\n",
                    colecao->N, i);
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);

    // O idDoc de cada documento é o seu índice i (0 a N-1)
    return 1;
}

// Stop Words

int CarregaStopWords(const char* caminhoArquivo, StopWords* sw) {
    FILE* fp = fopen(caminhoArquivo, "r");
    if (fp == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s'\n", caminhoArquivo);
        sw->N = 0;
        return 0;
    }

    sw->N = 0;
    while (sw->N < MAX_STOPWORDS &&
           fscanf(fp, "%49s", sw->palavras[sw->N]) == 1) {
        // Garante que a stop word está em minúsculas e sem acentos
        PreprocessaPalavra(sw->palavras[sw->N]);
        sw->N++;
    }

    fclose(fp);
    return 1;
}

int EhStopWord(const StopWords* sw, const char* palavra) {
    for (int i = 0; i < sw->N; i++) {
        if (strcmp(sw->palavras[i], palavra) == 0) {
            return 1;
        }
    }
    return 0;
}

// Processamento de Documento

void ProcessaDocumentoHash(const char* caminhoPasta, const char* nomeArquivo,
                           int idDoc, const StopWords* sw,
                           TabelaHash* tabela, TipoPesos pesos) {

    // Monta o caminho completo: pasta/nomeArquivo
    char caminhoCompleto[MAX_NOME_ARQUIVO * 2];
    snprintf(caminhoCompleto, sizeof(caminhoCompleto), "%s/%s",
             caminhoPasta, nomeArquivo);

    FILE* fp = fopen(caminhoCompleto, "r");
    if (fp == NULL) {
        fprintf(stderr, "Aviso: nao foi possivel abrir '%s', pulando.\n",
                caminhoCompleto);
        return;
    }

    char palavra[MAX_PALAVRA];

    // Lê palavra por palavra do arquivo
    while (fscanf(fp, "%49s", palavra) == 1) {
        // Aplica o pipeline de pré-processamento
        PreprocessaPalavra(palavra);

        // Ignora palavras vazias (que ficaram assim após remoção de pontuação)
        if (palavra[0] == '\0') {
            continue;
        }

        // Filtra stop words
        if (EhStopWord(sw, palavra)) {
            continue;
        }

        // Palavra válida — insere diretamente na Tabela Hash
        InsereTabelaHash(tabela, palavra, idDoc, pesos);
    }

    fclose(fp);
}

void ProcessaColecaoHash(const char* caminhoPasta, const Colecao* colecao,
                         const StopWords* sw, TabelaHash* tabela, TipoPesos pesos) {

    for (int i = 0; i < colecao->N; i++) {
        // idDoc = i (mapeamento direto pelo índice)
        ProcessaDocumentoHash(caminhoPasta, colecao->nomes[i], i, sw, tabela, pesos);
    }
}

void ProcessaDocumentoPatricia(const char* caminhoPasta, const char* nomeArquivo,
                               int idDoc, const StopWords* sw,
                               TipoArvore* arvore) {

    // Monta o caminho completo: pasta/nomeArquivo
    char caminhoCompleto[MAX_NOME_ARQUIVO * 2];
    snprintf(caminhoCompleto, sizeof(caminhoCompleto), "%s/%s",
             caminhoPasta, nomeArquivo);

    FILE* fp = fopen(caminhoCompleto, "r");
    if (fp == NULL) {
        fprintf(stderr, "Aviso: nao foi possivel abrir '%s', pulando.\n",
                caminhoCompleto);
        return;
    }

    char palavra[MAX_PALAVRA];

    // Lê palavra por palavra do arquivo
    while (fscanf(fp, "%49s", palavra) == 1) {
        // Aplica o pipeline de pré-processamento
        PreprocessaPalavra(palavra);

        // Ignora palavras vazias (que ficaram assim após remoção de pontuação)
        if (palavra[0] == '\0') {
            continue;
        }

        // Filtra stop words
        if (EhStopWord(sw, palavra)) {
            continue;
        }

        // Palavra válida — insere diretamente na Árvore Patricia
        // TODO: Gabriel Implementar isso
        // InserePatricia(arvore, palavra, idDoc);
    }

    fclose(fp);
}

void ProcessaColecaoPatricia(const char* caminhoPasta, const Colecao* colecao,
                             const StopWords* sw, TipoArvore* arvore) {

    for (int i = 0; i < colecao->N; i++) {
        // idDoc = i (mapeamento direto pelo índice)
        ProcessaDocumentoPatricia(caminhoPasta, colecao->nomes[i], i, sw, arvore);
    }
}
