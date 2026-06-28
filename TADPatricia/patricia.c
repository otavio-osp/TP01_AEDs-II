#include "patricia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comparacoes_patricia_insercao = 0;
int comparacoes_patricia_busca = 0;

void InicializaPatricia(TipoArvore* t) {
    *t = NULL;
}

// Retorna o caractere na posição i. Trata strings mais curtas que i retornando '\0'
static inline char charAt(const char* s, int len, Index i) {
    if (i >= (Index)len) return '\0';
    return s[i];
}

// Função auxiliar para criar um nó interno
static TipoArvore CriaNoInterno(Index index, char char_comp, TipoArvore esq, TipoArvore dir) {
    TipoArvore p = (TipoArvore)malloc(sizeof(NoPatricia));
    if (p == NULL) {
        perror("Erro ao alocar no interno");
        exit(EXIT_FAILURE);
    }
    p->nt = Interno;
    p->NO.NInterno.index = index;
    p->NO.NInterno.char_comparacao = char_comp;
    p->NO.NInterno.esq = esq;
    p->NO.NInterno.dir = dir;
    return p;
}

// Função auxiliar para criar um nó externo
static TipoArvore CriaNoExterno(const char* chave, int idDoc) {
    TipoArvore p = (TipoArvore)malloc(sizeof(NoPatricia));
    if (p == NULL) {
        perror("Erro ao alocar no externo");
        exit(EXIT_FAILURE);
    }
    p->nt = Externo;
    p->NO.NExterno.Chave = strdup(chave);
    FazListaOcorrenciaVazia(&(p->NO.NExterno.Ocorrencias));
    InsereOcorrencia(&(p->NO.NExterno.Ocorrencias), idDoc);
    return p;
}

TipoListaOcorrencia* PesquisaPatricia(TipoArvore t, const char* chave) {
    if (t == NULL) return NULL;
    
    int len = strlen(chave);
    TipoArvore p = t;
    
    // Desce na árvore comparando os caracteres nos índices de divergência
    while (p->nt == Interno) {
        comparacoes_patricia_busca++;
        char c = charAt(chave, len, p->NO.NInterno.index);
        
        // Regra de roteamento: < vai para a esquerda, >= vai para a direita
        if (c < p->NO.NInterno.char_comparacao) {
            p = p->NO.NInterno.esq;
        } else {
            p = p->NO.NInterno.dir;
        }
    }
    
    // Chegou em um nó externo, verifica se a palavra é exatamente a buscada
    comparacoes_patricia_busca++; 
    if (strcmp(chave, p->NO.NExterno.Chave) == 0) {
        return &(p->NO.NExterno.Ocorrencias);
    }
    
    return NULL; // Palavra não encontrada
}

void InserePatricia(TipoArvore* t, const char* chave, int idDoc) {
    // 1. Árvore vazia: insere primeiro nó externo
    if (*t == NULL) {
        *t = CriaNoExterno(chave, idDoc);
        return;
    }
    
    int lenK = strlen(chave);
    TipoArvore p = *t;
    
    // 2. Busca o nó folha mais próximo onde a palavra deveria estar
    while (p->nt == Interno) {
        comparacoes_patricia_insercao++;
        char c = charAt(chave, lenK, p->NO.NInterno.index);
        if (c < p->NO.NInterno.char_comparacao) {
            p = p->NO.NInterno.esq;
        } else {
            p = p->NO.NInterno.dir;
        }
    }
    
    // 3. Verifica se a palavra já existe
    comparacoes_patricia_insercao++;
    if (strcmp(chave, p->NO.NExterno.Chave) == 0) {
        // Palavra já existe, apenas adiciona a ocorrência
        InsereOcorrencia(&(p->NO.NExterno.Ocorrencias), idDoc);
        return;
    }
    
    // 4. Encontra o primeiro índice 'i' onde a nova palavra difere da palavra encontrada na folha
    Index i = 0;
    int lenL = strlen(p->NO.NExterno.Chave);
    while (charAt(chave, lenK, i) == charAt(p->NO.NExterno.Chave, lenL, i)) {
        comparacoes_patricia_insercao++;
        i++;
    }
    comparacoes_patricia_insercao++; // A última comparação que deu diferente
    
    // 5. Determina o caractere de comparação para o novo nó interno
    char charK = charAt(chave, lenK, i);
    char charL = charAt(p->NO.NExterno.Chave, lenL, i);
    char char_comp = (charK > charL) ? charK : charL; // Guarda o maior caractere
    
    // 6. Cria o novo nó externo
    TipoArvore novoExterno = CriaNoExterno(chave, idDoc);
    
    // 7. Busca novamente a posição para inserir o novo nó interno.
    // Paramos apenas quando encontrarmos um nó folha OU um nó interno com índice > i.
    // Isso é vital porque podemos ter vários nós com o MESMO índice i (BST de caracteres).
    TipoArvore pai = NULL;
    TipoArvore atual = *t;
    
    while (atual->nt == Interno && atual->NO.NInterno.index <= i) {
        pai = atual;
        char c = charAt(chave, lenK, atual->NO.NInterno.index);
        if (c < atual->NO.NInterno.char_comparacao) {
            atual = atual->NO.NInterno.esq;
        } else {
            atual = atual->NO.NInterno.dir;
        }
    }
    
    // 8. Insere o novo nó interno acima do nó "atual" que paramos.
    TipoArvore novoInterno = CriaNoInterno(i, char_comp, NULL, NULL);
    
    // Define os filhos do novo nó interno
    if (charK < char_comp) {
        novoInterno->NO.NInterno.esq = novoExterno;
        novoInterno->NO.NInterno.dir = atual;
    } else {
        novoInterno->NO.NInterno.esq = atual;
        novoInterno->NO.NInterno.dir = novoExterno;
    }
    
    // Conecta o pai ao novo nó interno
    if (pai == NULL) {
        *t = novoInterno; // Nova raiz
    } else {
        char c = charAt(chave, lenK, pai->NO.NInterno.index);
        if (c < pai->NO.NInterno.char_comparacao) {
            pai->NO.NInterno.esq = novoInterno;
        } else {
            pai->NO.NInterno.dir = novoInterno;
        }
    }
}

void ImprimePatricia(TipoArvore t) {
    if (t == NULL) return;
    
    if (t->nt == Interno) {
        ImprimePatricia(t->NO.NInterno.esq);
        ImprimePatricia(t->NO.NInterno.dir);
    } else {
        printf("%s ", t->NO.NExterno.Chave);
        ImprimeListaOcorrencia(&(t->NO.NExterno.Ocorrencias));
    }
}

void LiberaPatricia(TipoArvore t) {
    if (t == NULL) return;
    
    if (t->nt == Interno) {
        LiberaPatricia(t->NO.NInterno.esq);
        LiberaPatricia(t->NO.NInterno.dir);
    } else {
        if (t->NO.NExterno.Chave != NULL) {
            free(t->NO.NExterno.Chave);
        }
        LiberaListaOcorrencia(&(t->NO.NExterno.Ocorrencias));
    }
    free(t);
}
