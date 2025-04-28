#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int valor;
    Color cor;
    struct Node *esq, *dir, *pai;
} Node;

// Criação de um novo nó
Node* criarNo(int valor) {
    Node* novo = (Node*)malloc(sizeof(Node));
    novo->valor = valor;
    novo->cor = RED;
    novo->esq = novo->dir = novo->pai = NULL;
    return novo;
}

// Rotação à esquerda
Node* rotacaoEsquerda(Node* raiz, Node* x) {
    Node* y = x->dir;
    x->dir = y->esq;
    if (y->esq != NULL)
        y->esq->pai = x;
    y->pai = x->pai;

    if (x->pai == NULL)
        raiz = y;
    else if (x == x->pai->esq)
        x->pai->esq = y;
    else
        x->pai->dir = y;

    y->esq = x;
    x->pai = y;
    return raiz;
}

// Rotação à direita
Node* rotacaoDireita(Node* raiz, Node* y) {
    Node* x = y->esq;
    y->esq = x->dir;
    if (x->dir != NULL)
        x->dir->pai = y;
    x->pai = y->pai;

    if (y->pai == NULL)
        raiz = x;
    else if (y == y->pai->dir)
        y->pai->dir = x;
    else
        y->pai->esq = x;

    x->dir = y;
    y->pai = x;
    return raiz;
}

// Inserção binária
Node* inserirBST(Node* raiz, Node* novo) {
    if (raiz == NULL)
        return novo;

    if (novo->valor < raiz->valor) {
        raiz->esq = inserirBST(raiz->esq, novo);
        raiz->esq->pai = raiz;
    } else if (novo->valor > raiz->valor) {
        raiz->dir = inserirBST(raiz->dir, novo);
        raiz->dir->pai = raiz;
    }
    return raiz;
}

// Correção após inserção
Node* corrigirInsercao(Node* raiz, Node* no) {
    Node* tio;

    while (no != raiz && no->pai->cor == RED) {
        if (no->pai == no->pai->pai->esq) {
            tio = no->pai->pai->dir;
            if (tio != NULL && tio->cor == RED) {
                no->pai->cor = BLACK;
                tio->cor = BLACK;
                no->pai->pai->cor = RED;
                no = no->pai->pai;
            } else {
                if (no == no->pai->dir) {
                    no = no->pai;
                    raiz = rotacaoEsquerda(raiz, no);
                }
                no->pai->cor = BLACK;
                no->pai->pai->cor = RED;
                raiz = rotacaoDireita(raiz, no->pai->pai);
            }
        } else {
            tio = no->pai->pai->esq;
            if (tio != NULL && tio->cor == RED) {
                no->pai->cor = BLACK;
                tio->cor = BLACK;
                no->pai->pai->cor = RED;
                no = no->pai->pai;
            } else {
                if (no == no->pai->esq) {
                    no = no->pai;
                    raiz = rotacaoDireita(raiz, no);
                }
                no->pai->cor = BLACK;
                no->pai->pai->cor = RED;
                raiz = rotacaoEsquerda(raiz, no->pai->pai);
            }
        }
    }
    raiz->cor = BLACK;
    return raiz;
}

// Inserção completa
Node* inserir(Node* raiz, int valor) {
    Node* novo = criarNo(valor);
    raiz = inserirBST(raiz, novo);
    return corrigirInsercao(raiz, novo);
}

// Impressão em ordem
void emOrdem(Node* raiz) {
    if (raiz == NULL) return;
    emOrdem(raiz->esq);
    printf("%d (%s) ", raiz->valor, raiz->cor == RED ? "R" : "B");
    emOrdem(raiz->dir);
}

// Impressão visual (como árvore "deitada")
void imprime_RubroNegra(Node* raiz, int nivel) {
    if (raiz != NULL) {
        imprime_RubroNegra(raiz->dir, nivel + 1);
        for (int i = 0; i < nivel; i++)
            printf("    ");
        printf("%d (%s)\n", raiz->valor, raiz->cor == RED ? "R" : "B");
        imprime_RubroNegra(raiz->esq, nivel + 1);
    }
}

// Função principal de teste
int main() {
    Node* raiz = NULL;

    raiz = inserir(raiz, 10);
    raiz = inserir(raiz, 20);
    raiz = inserir(raiz, 30);
    raiz = inserir(raiz, 15);
    raiz = inserir(raiz, 25);
    raiz = inserir(raiz, 5);

    printf("Árvore Rubro-Negra em ordem:\n");
    emOrdem(raiz);
    printf("\n\nÁrvore Rubro-Negra formatada:\n");
    imprime_RubroNegra(raiz, 0);

    return 0;
}
