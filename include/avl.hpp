//
// Created by william on 2020/1/30.
//

#ifndef DATASTRUCTURE_AVL_HPP
#define DATASTRUCTURE_AVL_HPP

/* <Node Structure> */
template <class T>
struct AVL_Node {
    T value;
    AVL_Node *father;
    AVL_Node *lchild, *rchild;
    int height;

    AVL_Node();
    AVL_Node(T val, AVL_Node *fa, AVL_Node *lchild, AVL_Node *rchild);
    void contentCopyFrom(const AVL_Node<T> *other);
    void updateHeight();
    int balance() const;
    ~AVL_Node();
};

template <class T>
AVL_Node<T>::AVL_Node() = default;

template <class T>
AVL_Node<T>::AVL_Node(T val, AVL_Node<T> *fa, AVL_Node<T> *lchild, AVL_Node<T> *rchild)
:value(val), father(fa), lchild(lchild), rchild(rchild), height(0)
{}

template <class T>
void
AVL_Node<T>::contentCopyFrom(const AVL_Node<T> *other) {
    /* explicit is better than implicit */
    this->value = other->value;
}

template <class T>
void
AVL_Node<T>::updateHeight() {
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
AVL_Node<T>::balance() const {
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
AVL_Node<T>::~AVL_Node() {
    if (lchild)
        lchild->~AVL_Node<T>();
    if (rchild)
        rchild->~AVL_Node<T>();
}
/* </Node Structure> */

/* <AVL> */
template <class T>
class AVL {
public:
    AVL();
    ~AVL();

    int insert(T val);
    int remove(T val);
    int find(const T & val);
private:
    /* root pointer */
    AVL_Node<T> *rootptr;

    /* rotate */
    void RR(AVL_Node<T> *center);
    void LL(AVL_Node<T> *center);
    void RL(AVL_Node<T> *center);
    void LR(AVL_Node<T> *center);

    /* internal operations */
    AVL_Node<T> *internal_find(const T & val);
    void internal_remove(AVL_Node<T> *rm);
    void updateAVL(AVL_Node<T> *modified, bool del=false);
};

template <class T>
AVL<T>::AVL()
:rootptr(nullptr)
{}

template <class T>
AVL<T>::~AVL() {
    if (rootptr) rootptr->~AVL_Node();
}

template <class T>
int
AVL<T>::insert(T val) {
    if (!rootptr) {
        rootptr = new AVL_Node<T>(val, nullptr, nullptr, nullptr);
        return 1;
    }

    AVL_Node<T> *add = internal_find(val);

    /* add to right subtree */
    if (add->value<val) {
        add->rchild = new AVL_Node<T>(val, add, nullptr, nullptr);
        updateAVL(add->rchild);
    }
    /* add to left subtree */
    else if (val<add->value) {
        add->lchild = new AVL_Node<T>(val, add, nullptr, nullptr);
        updateAVL(add->lchild);
    }
    /* equal */
    else {
        return 0;
    }

    return 1;
}

template <class T>
int
AVL<T>::remove(T val) {
    if (!rootptr) return 0;

    /* find Node holding value: "val" */
    AVL_Node<T> *rm = internal_find(val);

    if (rm->value==val) {
        internal_remove(rm);
        return 1;
    }
    else return 0;
}

template <class T>
int
AVL<T>::find(const T & val) {
    if (!rootptr) return 0;

    AVL_Node<T> *result = internal_find(val);
    if (result->value==val) return 1;
    else return 0;
}

template <class T>
void
AVL<T>::RR(AVL_Node<T> *center) {
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
AVL<T>::LL(AVL_Node<T> *center) {
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
AVL<T>::RL(AVL_Node<T> *center) {
    RR(center);
    LL(center);
}

template <class T>
void
AVL<T>::LR(AVL_Node<T> *center) {
    LL(center);
    RR(center);
}

template <class T>
AVL_Node<T>*
AVL<T>::internal_find(const T & val) {
    if (!rootptr) return nullptr;

    AVL_Node<T> *iter = rootptr;
    do {
        if (val<iter->value) {
            if (iter->lchild) iter = iter->lchild;
            else return iter;
        }
        else if (iter->value<val) {
            if (iter->rchild) iter = iter->rchild;
            else return iter;
        }
        else return iter;
    } while(true);
}

template <class T>
void
AVL<T>::internal_remove(AVL_Node<T> *rm) {
    /* rm has 2 children: process until rm has 1 children */
    if (rm->lchild && rm->rchild) {
        /* find the smallest Node in right-subtree, it's going to replace *rm */
        AVL_Node<T> *iter = rm->rchild;
        while (iter->lchild) iter = iter->lchild;
        /* convert the deletion problem into 1-child-node(or leaf) deletion */
        rm->contentCopyFrom(iter);
        rm = iter;
    }

    AVL_Node<T> *audit = rm->father;
    /* Consider! */
    AVL_Node<T> *upd = rm->rchild ? rm->rchild : rm->lchild;
    /* upd is tree branch */
    if (upd) {
        upd->father = audit;
        /* rm is root */
        if (audit==nullptr) rootptr = upd;
        /* rm is audit's left child */
        else if (rm == audit->lchild) audit->lchild = upd;
        /* rm is audit's right child */
        else audit->rchild = upd;
    }
    /* rm is leaf */
    /* Have to update(rotate)? Yes!
     *      3
     *     / \
     *    2   4 <- delete 4, then need LL
     *   /
     *  1
     */
    else {
        /* audit==nullptr: only root remain */
        if (audit) {
            upd = audit;
            if (audit->lchild == rm) {
                audit->lchild = nullptr;
                if (audit->rchild) upd = audit->rchild;
            } else {
                audit->rchild = nullptr;
                if (audit->lchild) upd = audit->lchild;
            }
        }
    }

    rm->rchild = nullptr;
    rm->lchild = nullptr;
    /* Have to concern about lchild? Yes! consider:
     *      3
     *     / \...
     *    2 <- delete 2, then 2's lchild should be set to nullptr
     *   /
     *  1
     */
    delete rm;
    if (upd) updateAVL(upd, true);
}

template <class T>
void
AVL<T>::updateAVL(AVL_Node<T> *modified, bool del) {
    /* update height */
    if (modified->lchild) modified->lchild->updateHeight();
    if (modified->rchild) modified->rchild->updateHeight();
    modified->updateHeight();

    /* find the lowest imbalance point */
    while (modified->father && -2<modified->father->balance() && modified->father->balance()<2)
        modified = modified->father;
    /* no rotation when reach root */
    if (!modified->father) return;

    if (del) {
        if (modified->father->lchild==modified && modified->father->rchild) modified = modified->father->rchild;
        else if (modified->father->rchild==modified && modified->father->lchild) modified = modified->father->lchild;
    }

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
