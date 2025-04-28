// Função auxiliar: encontrar o menor nó
Node* minimo(Node* no) {
    while (no->esq != NULL)
        no = no->esq;
    return no;
}

// Função auxiliar: transplanta um nó no lugar de outro
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

// Correção após remoção
Node* corrigirRemocao(Node* raiz, Node* x) {
    while (x != raiz && (x == NULL || x->cor == BLACK)) {
        if (x == x->pai->esq) {
            Node* w = x->pai->dir;
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                raiz = rotacaoEsquerda(raiz, x->pai);
                w = x->pai->dir;
            }
            if ((w->esq == NULL || w->esq->cor == BLACK) && (w->dir == NULL || w->dir->cor == BLACK)) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->dir == NULL || w->dir->cor == BLACK) {
                    if (w->esq != NULL)
                        w->esq->cor = BLACK;
                    w->cor = RED;
                    raiz = rotacaoDireita(raiz, w);
                    w = x->pai->dir;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                if (w->dir != NULL)
                    w->dir->cor = BLACK;
                raiz = rotacaoEsquerda(raiz, x->pai);
                x = raiz;
            }
        } else {
            Node* w = x->pai->esq;
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                raiz = rotacaoDireita(raiz, x->pai);
                w = x->pai->esq;
            }
            if ((w->esq == NULL || w->esq->cor == BLACK) && (w->dir == NULL || w->dir->cor == BLACK)) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->esq == NULL || w->esq->cor == BLACK) {
                    if (w->dir != NULL)
                        w->dir->cor = BLACK;
                    w->cor = RED;
                    raiz = rotacaoEsquerda(raiz, w);
                    w = x->pai->esq;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                if (w->esq != NULL)
                    w->esq->cor = BLACK;
                raiz = rotacaoDireita(raiz, x->pai);
                x = raiz;
            }
        }
    }
    if (x != NULL)
        x->cor = BLACK;
    return raiz;
}

// Busca de um nó pelo valor
Node* buscar(Node* raiz, int valor) {
    if (raiz == NULL || raiz->valor == valor)
        return raiz;
    if (valor < raiz->valor)
        return buscar(raiz->esq, valor);
    else
        return buscar(raiz->dir, valor);
}

// Função de remoção
Node* remover(Node* raiz, int valor) {
    Node* z = buscar(raiz, valor);
    if (z == NULL)
        return raiz; // Nó não encontrado

    Node* y = z;
    Node* x;
    Color cor_original = y->cor;

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
            if (x != NULL)
                x->pai = y;
        } else {
            transplantar(&raiz, y, y->dir);
            y->dir = z->dir;
            if (y->dir != NULL)
                y->dir->pai = y;
        }
        transplantar(&raiz, z, y);
        y->esq = z->esq;
        if (y->esq != NULL)
            y->esq->pai = y;
        y->cor = z->cor;
    }

    free(z);

    if (cor_original == BLACK) {
        raiz = corrigirRemocao(raiz, x);
    }

    return raiz;
}
