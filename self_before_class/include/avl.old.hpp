//
// Created by william on 2020/1/30.
//

#ifndef DATASTRUCTURE_AVL_HPP
#define DATASTRUCTURE_AVL_HPP

/* <Node Structure> */
template <class T>
struct Node {
    T value;
    Node *father;
    Node *lchild, *rchild;
    int height;

    Node();
    Node(T val, Node *fa, Node *lchild, Node *rchild);
    void contentCopyFrom(const Node<T> *other);
    void updateHeight();
    int balance() const;
    ~Node();
};

template <class T>
Node<T>::Node() {}

template <class T>
Node<T>::Node(T val, Node<T> *fa, Node<T> *lchild, Node<T> *rchild)
:value(val), father(fa), lchild(lchild), rchild(rchild), height(0)
{}

template <class T>
void
Node<T>::contentCopyFrom(const Node<T> *other) {
    /* explicit is better than implicit */
    this->value = other->value;
}

template <class T>
void
Node<T>::updateHeight() {
    /* update this->height */
    if (lchild) {
        if (rchild) height = (lchild->height<rchild->height ? rchild->height : lchild->height) + 1;
        else height = lchild->height + 1;
    }
    else {
        if (rchild) height = rchild->height + 1;
        else height = 0;
    }

    /* update iteration */
    auto iter = this;
    while (iter->father) {
        int max = 0;
        if (iter->father->lchild) max = iter->father->lchild->height;
        if (iter->father->rchild) max = iter->father->rchild->height<max ? max : iter->father->rchild->height;

        iter->father->height = max+1;
        iter = iter->father;
    }
}

template <class T>
int
Node<T>::balance() const {
    if (!lchild) {
        if (!rchild) return 0;
        else return -rchild->height - 1;
    }
    else {
        if (!rchild) return lchild->height + 1;
        else return lchild->height - rchild->height;
    }
}

template <class T>
Node<T>::~Node() {
    if (lchild)
        lchild->~Node<T>();
    if (rchild)
        rchild->~Node<T>();
}
/* </Node Structure> */

/* <AVL> */
template <class T>
class AVL {
public:
    AVL();
    explicit AVL(T val);
    ~AVL();

    int insert(T val);
    int remove(T val);
    int find(const T & val);
private:
    /* root pointer */
    Node<T> *rootptr;

    /* rotate */
    void RR(Node<T> *center);
    void LL(Node<T> *center);
    void RL(Node<T> *center);
    void LR(Node<T> *center);

    /* internal operations */
    Node<T> *internal_find(const T & val);
    void internal_remove(Node<T> *rm);
    void updateAVL(Node<T> *modified);
};

template <class T>
AVL<T>::AVL()
:rootptr(nullptr)
{}

template <class T>
AVL<T>::AVL(T val) {
    rootptr = new Node<T>(val, nullptr, nullptr, nullptr);
}

template <class T>
AVL<T>::~AVL() {
    if (rootptr) rootptr->~Node();
}

template <class T>
int
AVL<T>::insert(T val) {
    if (!rootptr) {
        rootptr = new Node<T>(val, nullptr, nullptr, nullptr);
        return 1;
    }

    auto fa = rootptr;
    do {
        if (fa->value < val) {
            if (fa->rchild) fa = fa->rchild;
            else break;
        }
        else if (fa->value > val) {
            if (fa->lchild) fa = fa->lchild;
            else break;
        }
        else return 0;
    } while (true);

    if (fa->value < val) {
        fa->rchild = new Node<T>(val, fa, nullptr, nullptr);
        updateAVL(fa->rchild);
    }
    else {
        fa->lchild = new Node<T>(val, fa, nullptr, nullptr);
        updateAVL(fa->lchild);
    }

    return 1;
}

template <class T>
int
AVL<T>::remove(T val) {
    if (!rootptr) return 0;

    /* find Node holding value: "val" */
    auto fa = rootptr;
    do {
        if (fa->value < val) {
            if (fa->rchild) fa = fa->rchild;
            /* Not found */
            else return 0;
        }
        else if (fa->value > val) {
            if (fa->lchild) fa = fa->lchild;
            /* Not found */
            else return 0;
        }
        /* equivalent: fa->value==val */
        else break;
    } while (true);

    internal_remove(fa);
    return 1;
}

template <class T>
int
AVL<T>::find(const T & val) {
    auto fa = rootptr;
    do {
        if (fa->value < val) {
            if (fa->rchild) fa = fa->rchild;
            else break;
        }
        else if (fa->value > val) {
            if (fa->lchild) fa = fa->lchild;
            else break;
        }
        else return 1;
    } while (true);
    return 0;
}

template <class T>
void
AVL<T>::RR(Node<T> *center) {
    /* root can't be rotation center */
    if (center==rootptr) return;

    /* audit: nearest unconcerned upper node */
    auto audit = center->father->father;
    auto newLeft = center->father;

    /* update top layer */
    /* `audit==nullptr` indicates rotation at the top */
    if (audit==nullptr) {
        rootptr = center;
        center->father = nullptr;
    }
    /* else: connect the rotated subtree to audit's lchild or rchild */
    else if (audit->lchild==center->father) {
        audit->lchild = center;
        center->father = audit;
    }
    else {
        audit->rchild = center;
        center->father = audit;
    }

    /* update original father */
    newLeft->rchild = center->lchild;
    if (center->lchild) center->lchild->father = newLeft;

    center->lchild = newLeft;
    newLeft->father = center;

    /* update height */
    if (center->lchild) center->lchild->updateHeight();
    if (center->rchild) center->rchild->updateHeight();
    if (!center->lchild && !center->rchild) center->updateHeight();
}

template <class T>
void
AVL<T>::LL(Node<T> *center) {
    if (center==rootptr) return;

    auto audit = center->father->father;
    auto newRight = center->father;

    /* update top layer */
    if (audit==nullptr) {
        rootptr = center;
        center->father = nullptr;
    }
    else if (audit->lchild==center->father) {
        audit->lchild = center;
        center->father = audit;
    }
    else {
        audit->rchild = center;
        center->father = audit;
    }

    /* update original father */
    newRight->lchild = center->rchild;
    if (center->rchild) center->rchild->father = newRight;

    center->rchild = newRight;
    newRight->father = center;

    /* update height */
    if (center->lchild) center->lchild->updateHeight();
    if (center->rchild) center->rchild->updateHeight();
    if (!center->lchild && !center->rchild) center->updateHeight();
}

template <class T>
void
AVL<T>::RL(Node<T> *center) {
    RR(center);
    LL(center);
}

template <class T>
void
AVL<T>::LR(Node<T> *center) {
    LL(center);
    RR(center);
}

template <class T>
Node<T>*
AVL<T>::internal_find(const T & val) {
    if (!rootptr) return nullptr;

    Node<T> *iter = rootptr;
    do {
        if (val<iter->value) {
            if (iter->rchild) iter = iter->rchild;
            else return iter;
        }
        else if (iter->value<val) {
            if (iter->lchild) iter = iter->lchild;
            else return iter;
        }
        else return iter;
    } while(true);
}

template <class T>
void
AVL<T>::internal_remove(Node<T> *rm) {
    /* rm has 2 children: process until rm has 1 children */
    if (rm->lchild && rm->rchild) {
        Node<T> *iter = rm;
        /* find the smallest Node in right-subtree */
        while (iter->lchild) iter = iter->lchild;
        /* convert the deletion problem into 1-child-node(or leaf) deletion */
        rm->contentCopyFrom(iter);
        rm = iter;
    }

    Node<T> *audit = rm->father;
    Node<T> *upd = rm->rchild;
    if (upd) {
        /* rm is root */
        if (audit==nullptr) rootptr = rm->rchild;
        /* rm is father's left child */
        else if (rm == audit->lchild) rm->father->lchild = rm->rchild;
        /* rm is father's right child */
        else audit->rchild = rm->rchild;

        rm->rchild->father = rm->father;
    }
    /* rm is leaf */
    else {
        upd = audit;
        if (audit->rchild) upd = audit->rchild;

        if (audit->lchild==rm) audit->lchild = nullptr;
        else audit->rchild = nullptr;
    }

    rm->rchild = nullptr;
    rm->lchild = nullptr;
    delete rm;
    updateAVL(upd);
}

template <class T>
void
AVL<T>::updateAVL(Node<T> *modified) {
    /* update height */
    if (modified->lchild) modified->lchild->updateHeight();
    if (modified->rchild) modified->rchild->updateHeight();
    modified->updateHeight();

    /* find the lowest imbalance point */
    while (modified->father && -2<modified->father->balance() && modified->father->balance()<2)
        modified = modified->father;
    /* no rotation when reach root */
    if (!modified->father) return;

    /* situations - rotate once */
    if (modified->balance()<0) {
        if (modified->father->balance()<0) RR(modified);
        else RL(modified->rchild);
    }
    else {
        if (modified->father->balance()<0) LR(modified->lchild);
        else LL(modified);
    }

    /* update height */
    if (modified->lchild) modified->lchild->updateHeight();
    if (modified->rchild) modified->rchild->updateHeight();
    modified->updateHeight();
}
/* </AVL> */

#endif //DATASTRUCTURE_AVL_HPP
