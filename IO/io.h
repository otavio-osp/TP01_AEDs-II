// Integrante 1 - Otávio
// Responsável por: I/O, Leitura de Arquivos e Pré-processamento de Texto

#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "../TADHash/hash.h"
#include "../TADPatricia/patricia.h"

// Constantes
#define MAX_NOME_ARQUIVO 100
#define MAX_DOCUMENTOS   100
#define MAX_PALAVRA      50
#define MAX_STOPWORDS    300

// Estrutura: Coleção de Documentos
// Mapeia cada documento a um idDoc único (0-indexado)
typedef struct {
    char nomes[MAX_DOCUMENTOS][MAX_NOME_ARQUIVO]; // nomes dos arquivos
    int  N;                                        // quantidade de documentos
} Colecao;

// Estrutura: Tabela de Stop Words
typedef struct {
    char palavras[MAX_STOPWORDS][MAX_PALAVRA];
    int  N;
} StopWords;

// Funções de Leitura

// Lê o arquivo entrada.txt e preenche a coleção com os nomes
// dos documentos (e seus IDs implícitos pelo índice).
// Retorna 1 em sucesso, 0 em falha.
int LerArquivoEntrada(const char* caminhoEntrada, Colecao* colecao);

// Funções de Pré-processamento

// Converte toda a string para minúsculas (ASCII).
void ParaMinusculas(char* texto);

// Remove caracteres de pontuação da string, mantendo apenas
// letras e espaços. Modifica a string in-place.
void RemovePontuacao(char* texto);

// Aplica todo o pipeline de limpeza (minúsculas, pontuação) sobre uma palavra isolada.
void PreprocessaPalavra(char* palavra);

// Funções de Stop Words

// Carrega a lista de stop words
// Retorna 1 em sucesso, 0 em falha.
int CarregaStopWords(const char* caminhoArquivo, StopWords* sw);

// Verifica se uma palavra é stop word.
// Retorna 1 se for stop word, 0 caso contrário.
int EhStopWord(const StopWords* sw, const char* palavra);

// Funções Principais de Processamento

void ProcessaDocumentoHash(const char* caminhoPasta, const char* nomeArquivo,
                           int idDoc, const StopWords* sw,
                           TabelaHash* tabela, TipoPesos pesos);

void ProcessaColecaoHash(const char* caminhoPasta, const Colecao* colecao,
                         const StopWords* sw, TabelaHash* tabela, TipoPesos pesos);

void ProcessaDocumentoPatricia(const char* caminhoPasta, const char* nomeArquivo,
                               int idDoc, const StopWords* sw,
                               TipoArvore* arvore);

void ProcessaColecaoPatricia(const char* caminhoPasta, const Colecao* colecao,
                             const StopWords* sw, TipoArvore* arvore);

#endif
