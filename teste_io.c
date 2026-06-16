// Integrante 1 - Otávio
// Responsável por: Teste do Módulo IO e Pré-processamento

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "IO/io.h"
#include "TADHash/hash.h"
#include "TADPatricia/patricia.h"

int main() {
    printf("--- Teste do Modulo IO (Leitor e Pre-processamento) ---\n\n");

    srand((unsigned)time(NULL));

    // Carrega a lista de stop words
    StopWords sw;
    if (!CarregaStopWords("stopwords.txt", &sw)) {
        fprintf(stderr, "Erro ao carregar stop words.\n");
        return 1;
    }

    // Le o arquivo de entrada
    Colecao colecao;
    if (!LerArquivoEntrada("Fabulas/entrada.txt", &colecao)) {
        fprintf(stderr, "Erro ao ler arquivo de entrada.\n");
        return 1;
    }

    // Mapeamento idDoc -> nome do arquivo
    printf("Mapeamento idDoc -> Documento:\n");
    for (int i = 0; i < colecao.N; i++) {
        printf("  idDoc %d -> %s\n", i, colecao.nomes[i]);
    }

    // Inicializa a tabela Hash
    TabelaHash tabela;
    InicializaTabelaHash(&tabela, 127);

    TipoPesos pesos;
    GeraPesos(pesos);

    // Inicializa a arvore Patricia
    TipoArvore raiz = NULL;

    printf("\nProcessando a colecao de documentos...\n");
    ProcessaColecaoHash("Fabulas", &colecao, &sw, &tabela, pesos);
    ProcessaColecaoPatricia("Fabulas", &colecao, &sw, &raiz);

    // Impressão da Tabela Hash ordenada
    printf("\nImprimindo Tabela Hash ordenada:\n");
    ImprimeTabelaHashOrdenada(&tabela);

    // Teste de busca por termos na Hash
    printf("\nRealizando buscas na Hash:\n");
    const char* termos[] = {"raposa", "uvas", "leao", "rato"};
    int nTermos = 4;

    for (int i = 0; i < nTermos; i++) {
        TipoListaOcorrencia* lista = PesquisaTabelaHash(&tabela, termos[i], pesos);
        if (lista != NULL) {
            printf("  '%s' encontrada em %d doc(s): ", termos[i], lista->Tamanho);
            ImprimeListaOcorrencia(lista);
        } else {
            printf("  '%s' nao encontrada.\n", termos[i]);
        }
    }

    // Estatísticas da Tabela Hash
    printf("\nEstatisticas da Tabela Hash:\n");
    printf("- Documentos processados: %d\n", colecao.N);
    printf("- Stop words carregadas: %d\n", sw.N);
    printf("- Comparacoes na insercao: %d\n", tabela.comparacoes_insercao);
    printf("- Comparacoes na pesquisa: %d\n", tabela.comparacoes_pesquisa);

    // Libera a memória alocada
    LiberaTabelaHash(&tabela);

    printf("\n--- Fim do Teste ---\n");
    return 0;
}
