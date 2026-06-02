// Integrante 4 - Tom
// Responsável por: UI (Menu Principal) e Integração de todos os módulos

#include <stdio.h>
#include <stdlib.h>

#include "TADLista/lista.h"
#include "TADHash/hash.h"

int main() {
    printf("--- Teste de Validacao da Tabela HASH (Padrao Simplificado) ---\n");
    
    int tamanho = 7; 
    
    TabelaHash tabela;
    InicializaTabelaHash(&tabela, tamanho); 
    
    // Gera os pesos aleatórios que a função de Hashing Universal exige
    TipoPesos pesos;
    GeraPesos(pesos);
    
    // Inserindo palavras fictícias e associando a IDs de documento
    InsereTabelaHash(&tabela, "casa", 1, pesos);
    InsereTabelaHash(&tabela, "apartamento", 1, pesos);
    InsereTabelaHash(&tabela, "casa", 1, pesos);
    InsereTabelaHash(&tabela, "casa", 2, pesos);
    InsereTabelaHash(&tabela, "ninguem", 1, pesos);
    InsereTabelaHash(&tabela, "ninguem", 2, pesos);
    InsereTabelaHash(&tabela, "todos", 2, pesos);
    
    // Teste de Impressão (Deve imprimir as palavras em ordem alfabética)
    printf("Imprimindo a Tabela Hash ordenada:\n");
    ImprimeTabelaHashOrdenada(&tabela);
    
    // Teste de estatísticas
    printf("\nEstatisticas de Complexidade:\n");
    printf("- Comparacoes na Insercao: %d\n", tabela.comparacoes_insercao);
    
    // Libera a memória
    LiberaTabelaHash(&tabela);
    
    printf("--- Fim do Teste ---\n");
    return 0;
}
