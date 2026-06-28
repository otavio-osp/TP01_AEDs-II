#include "patricia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comparacoes_patricia_insercao = 0;
int comparacoes_patricia_busca = 0;

void InicializaPatricia(TipoArvore* t) {
    *t = NULL;
}

// retorna o char na posicao i. se a palavra for menor, retorna '\0'
static inline char charAt(const char* s, size_t len, Index i) {
    if (i >= len) return '\0';
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
    
    size_t len = strlen(chave);
    TipoArvore p = t;
    
    // desce na arvore testando os caracteres nos indices
    while (p->nt == Interno) {
        comparacoes_patricia_busca++;
        char c = charAt(chave, len, p->NO.NInterno.index);
        
        // < vai pra esquerda, >= vai pra direita
        if (c < p->NO.NInterno.char_comparacao) {
            p = p->NO.NInterno.esq;
        } else {
            p = p->NO.NInterno.dir;
        }
    }
    
    // achou uma folha, confere se eh a palavra certa
    comparacoes_patricia_busca++; 
    if (strcmp(chave, p->NO.NExterno.Chave) == 0) {
        return &(p->NO.NExterno.Ocorrencias);
    }
    
    return NULL; // nao achou
}

// funcao auxiliar recursiva do ziviani para encaixar o no interno
static TipoArvore InsereEntre(const char* chave, int idDoc, TipoArvore t, Index i, char char_comp) {
    size_t len = strlen(chave);
    
    // se for folha ou o indice do no interno for maior q o i, encaixa aqui
    if (t->nt == Externo || t->NO.NInterno.index > i) {
        TipoArvore novoInterno = CriaNoInterno(i, char_comp, NULL, NULL);
        TipoArvore novoExterno = CriaNoExterno(chave, idDoc);
        
        char charK = charAt(chave, len, i);
        if (charK < char_comp) {
            novoInterno->NO.NInterno.esq = novoExterno;
            novoInterno->NO.NInterno.dir = t;
        } else {
            novoInterno->NO.NInterno.esq = t;
            novoInterno->NO.NInterno.dir = novoExterno;
        }
        return novoInterno;
    } else {
        // senao, continua descendo com as regras de comparacao
        char c = charAt(chave, len, t->NO.NInterno.index);
        if (c < t->NO.NInterno.char_comparacao) {
            t->NO.NInterno.esq = InsereEntre(chave, idDoc, t->NO.NInterno.esq, i, char_comp);
        } else {
            t->NO.NInterno.dir = InsereEntre(chave, idDoc, t->NO.NInterno.dir, i, char_comp);
        }
        return t;
    }
}

void InserePatricia(TipoArvore* t, const char* chave, int idDoc) {
    // arvore vazia, cria a primeira folha
    if (*t == NULL) {
        *t = CriaNoExterno(chave, idDoc);
        return;
    }
    
    size_t lenK = strlen(chave);
    TipoArvore p = *t;
    
    // desce ate a folha mais parecida
    while (p->nt == Interno) {
        comparacoes_patricia_insercao++;
        char c = charAt(chave, lenK, p->NO.NInterno.index);
        if (c < p->NO.NInterno.char_comparacao) {
            p = p->NO.NInterno.esq;
        } else {
            p = p->NO.NInterno.dir;
        }
    }
    
    // checa se a palavra ja ta la
    comparacoes_patricia_insercao++;
    if (strcmp(chave, p->NO.NExterno.Chave) == 0) {
        // so adiciona a ocorrencia do documento
        InsereOcorrencia(&(p->NO.NExterno.Ocorrencias), idDoc);
        return;
    }
    
    // procura qual o indice do char diferente
    Index i = 0;
    size_t lenL = strlen(p->NO.NExterno.Chave);
    while (charAt(chave, lenK, i) == charAt(p->NO.NExterno.Chave, lenL, i)) {
        comparacoes_patricia_insercao++;
        i++;
    }
    comparacoes_patricia_insercao++; // ultima comparacao que falhou
    
    // pega o maior char para usar de regra no no interno
    char charK = charAt(chave, lenK, i);
    char charL = charAt(p->NO.NExterno.Chave, lenL, i);
    char char_comp = (charK > charL) ? charK : charL;
    
    // faz a insercao real chamando a recursiva do ziviani
    *t = InsereEntre(chave, idDoc, *t, i, char_comp);
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
