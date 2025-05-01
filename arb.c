#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int valor;
    Color cor;
    struct Node *esq, *dir, *pai;
} Node;

Node* criarNo(int valor) {
    Node* novo = (Node*)malloc(sizeof(Node));
    novo->valor = valor;
    novo->cor = RED;
    novo->esq = novo->dir = novo->pai = NULL;
    return novo;
}

// Rotação a esquerda
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

// Rotação a direita
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

Node* inserir(Node* raiz, int valor) {
    Node* novo = criarNo(valor);
    raiz = inserirBST(raiz, novo);
    return corrigirInsercao(raiz, novo);
}

void emOrdem(Node* raiz) {
    if (raiz == NULL) return;
    emOrdem(raiz->esq);
    printf("%d (%s) ", raiz->valor, raiz->cor == RED ? "R" : "B");
    emOrdem(raiz->dir);
}

Node* NULL_LEAF;

Node* criarFolhaNula() {
    Node* folha = (Node*)malloc(sizeof(Node));
    folha->cor = BLACK;
    folha->valor = 0;
    folha->esq = folha->dir = folha->pai = NULL;
    return folha;
}

Node* buscar(Node* raiz, int valor) {
    if (raiz == NULL || raiz->valor == valor)
        return raiz;
    if (valor < raiz->valor)
        return buscar(raiz->esq, valor);
    else
        return buscar(raiz->dir, valor);
}

void transplantar(Node** raiz, Node* u, Node* v) {
    if (u->pai == NULL)
        *raiz = v;
    else if (u == u->pai->esq)
        u->pai->esq = v;
    else
        u->pai->dir = v;

    if (v != NULL)
        v->pai = u->pai;
}

Node* minimo(Node* no) {
    while (no->esq != NULL)
        no = no->esq;
    return no;
}

Node* corrigirRemocao(Node* raiz, Node* x) {
    while (x != raiz && (x == NULL || x->cor == BLACK)) {
        if (x == x->pai->esq) {
            Node* w = x->pai->dir; // irmao de x

            // Caso 1: irmao vermelho
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                raiz = rotacaoEsquerda(raiz, x->pai);
                w = x->pai->dir;
            }

            // Caso 2: irmao e filhos pretos
            if ((w->esq == NULL || w->esq->cor == BLACK) &&
                (w->dir == NULL || w->dir->cor == BLACK)) {
                w->cor = RED;
                x = x->pai;
            } else {
                // Caso 3: irmao preto, filho esquerdo vermelho
                if (w->dir == NULL || w->dir->cor == BLACK) {
                    if (w->esq != NULL && w->esq->cor == RED) w->esq->cor = BLACK;
                    w->cor = RED;
                    raiz = rotacaoDireita(raiz, w);
                    w = x->pai->dir;
                }

                // Caso 4: irmao preto, filho direito vermelho
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                if (w->dir != NULL && w->dir->cor == RED) w->dir->cor = BLACK;
                raiz = rotacaoEsquerda(raiz, x->pai);
                x = raiz;
            }
        } else {
            // Mesma logica, mas espelhada para x ser filho da direita
            Node* w = x->pai->esq;

            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                raiz = rotacaoDireita(raiz, x->pai);
                w = x->pai->esq;
            }

            if ((w->esq == NULL || w->esq->cor == BLACK) &&
                (w->dir == NULL || w->dir->cor == BLACK)) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->esq == NULL || w->esq->cor == BLACK) {
                    if (w->dir != NULL && w->dir->cor == RED) w->dir->cor = BLACK;
                    w->cor = RED;
                    raiz = rotacaoEsquerda(raiz, w);
                    w = x->pai->esq;
                }

                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                if (w->esq != NULL && w->esq->cor == RED) w->esq->cor = BLACK;
                raiz = rotacaoDireita(raiz, x->pai);
                x = raiz;
            }
        }
    }

    if (x != NULL)
        x->cor = BLACK;

    return raiz;
}

Node* remover(Node* raiz, int valor) {
    Node* z = buscar(raiz, valor);  // No a ser removido
    if (z == NULL) return raiz;     // Nao encontrado

    Node *y = z;                    // No que sera efetivamente removido
    Node *x;                        // No que tomara o lugar (ou NULL_LEAF)
    Color cor_original = y->cor;    // Precisamos saber a cor original

    if (z->esq == NULL) {
        x = z->dir;
        transplantar(&raiz, z, z->dir);
    } else if (z->dir == NULL) {
        x = z->esq;
        transplantar(&raiz, z, z->esq);
    } else {
        y = minimo(z->dir);
        cor_original = y->cor;
        x = y->dir;

        if (y->pai == z) {
            if (x != NULL) x->pai = y;
        } else {
            transplantar(&raiz, y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }

        transplantar(&raiz, z, y);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }

    free(z);

    if (cor_original == BLACK) {
        raiz = corrigirRemocao(raiz, x);
    }

    return raiz;
}


int main() {
    Node* raiz = NULL;

    // Inserções
    int valores[] = {10, 5, 20, 15, 25, 3, 8};
    int n = sizeof(valores)/sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        raiz = inserir(raiz, valores[i]);
    }

    printf("Arvore apos insercoes (em ordem):\n");
    emOrdem(raiz);
    printf("\n");

    // Remoção
    printf("Removendo o valor 10...\n");
    raiz = remover(raiz, 10);

    printf("Arvore apos remocao (em ordem):\n");
    emOrdem(raiz);
    printf("\n");

    return 0;
}

/*

int main() {
    Node* raiz = NULL;
    int valores[] = {10, 20, 30, 15, 25, 5, 1};
    int n = sizeof(valores)/sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        raiz = inserir(raiz, valores[i]);
    }

    printf("Árvore em ordem:\n");
    emOrdem(raiz);
    printf("\n");

    return 0;
}

*/

