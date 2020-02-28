//
// Created by william on 2020/2/3.
//

#include <algorithm>
#include <cstring>

#ifndef DATASTRUCTURE_BTREE_HPP
#define DATASTRUCTURE_BTREE_HPP

#define __constructor__
#define __destructor__

typedef int index_t;

template <class T, index_t order=6>
class bTree {
private:
    // bTree_Node class
    struct bTree_Node {
        index_t size;  // numbers of element
        T values[order+1];  // value
        bTree_Node *children[order+2];  // children pointers
        bTree_Node *father;  // easy to trace back

        bTree_Node() = default;
        bTree_Node(const T & val, bTree_Node *father, bTree_Node *lchild, bTree_Node *rchild);
        index_t withinNode_find(const T & val);
        void insert_at(index_t pos, const T & val, bTree_Node *child_pointer, bTree_Node *rchild_pointer);
    };  // class bTree_Node

    // stores internal searching result
    // first: Node where result is
    // second: result's position in Node
    // tip: if result do not matches, then it indicates the location to insert into (lower bound)
    typedef std::pair<bTree_Node*, index_t> result;

    bTree_Node *rootptr;
    const index_t min = order/2 + 1;

    result internal_find(const T & val);
    static T resolve(const result & r);
    void split(bTree_Node *sp);

public:
    bTree();
    void insert(const T & val);
    int find(const T & val);
};  // class bTree

template <class T, index_t order>
__constructor__
bTree<T, order>::bTree_Node::bTree_Node(const T & val, bTree<T, order>::bTree_Node *fa, bTree<T, order>::bTree_Node *lchild, bTree<T, order>::bTree_Node *rchild) {
    memset(values, 0, (order+1)*sizeof(T));
    memset(children, 0, (order+2)*sizeof(void*));
    children[0] = lchild, children[1] = rchild;
    values[0] = val;
    father = fa;
    size = 1;
}

template <class T, index_t order>
index_t
bTree<T, order>::bTree_Node::withinNode_find(const T & val) {
    T *result = std::lower_bound((T*)(values), (T*)(values)+size-1, val);

    if (val < *result) result--;  // lower_bound for 0 on {1, 1, 1, 1, 1}

    return result - (T*)(values);
}

template <class T, index_t order>
void
bTree<T, order>::bTree_Node::insert_at(index_t pos, const T & val, bTree_Node *lchild_pointer, bTree_Node *rchild_pointer) {
    children[size+1] = children[size];

    for (index_t i=size-1 ; i>pos ; i--) {
        values[i+1] = values[i];
        children[i+1] = children[i];
    }
    values[pos+1] = val;
    children[pos+1] = lchild_pointer;
    children[pos+2] = rchild_pointer;

    size++;
}

template <class T, index_t order>
typename bTree<T, order>::result
bTree<T, order>::internal_find(const T & val) {
    if (!rootptr) return std::make_pair<bTree_Node*, index_t>(nullptr, 0);

    auto iter = rootptr;
    index_t result = 0;
    while (true) {
        result = iter->withinNode_find(val);  // **open interval** feature of stl
        if (iter->values[result]==val || !iter->children[result+1]) break;  // found || no children to go
        else iter = iter->children[result+1];  // continue finding iteration
    }

    return std::make_pair(iter, result);
}

template <class T, index_t order>
T
bTree<T, order>::resolve(const bTree<T, order>::result & r) {
    return r.first->values[r.second];
}

template <class T, index_t order>
void
bTree<T, order>::split(bTree<T, order>::bTree_Node *sp) {
    if (!sp) return;  // (else)

    // make a new node (right child)
    auto fa = sp->father;
    auto rchild = new bTree_Node();

    T sendUp = sp->values[(order/2)];

    rchild->children[0] = sp->children[(order/2)+1];
    if (rchild->children[0]) rchild->children[0]->father = rchild;  // important - New father!
    for (index_t i=(order/2)+1, j=0 ; i<=order ; i++, j++) {
        rchild->values[j] = sp->values[i];
        rchild->size++;

        rchild->children[j+1] = sp->children[i+1];
        if (rchild->children[j+1]) rchild->children[j+1]->father = rchild;  // important - New father!
    }

    sp->size = order - rchild->size;
    /*
    sp->children[order/2] = 0;
    for (index_t i=(order/2)+1 ; i<order ; i++) {
        sp->children[i] = nullptr;
        sp->values[i] = 0;
    }
     */

    // make a new root
    if (!fa) rootptr = rchild->father = sp->father = new bTree_Node(sp->values[order/2], nullptr, sp, rchild);
    // insert into father
    else {
        rchild->father = fa;

        auto insert_pos = fa->withinNode_find(sendUp);
        fa->insert_at(insert_pos, sendUp, sp, rchild);

        if (fa->size>order) split(fa);
    }
}

template <class T, index_t order>
__constructor__
bTree<T, order>::bTree()
:rootptr(nullptr)
{}

template <class T, index_t order>
void
bTree<T, order>::insert(const T & val) {
    if (!rootptr) {
        rootptr = new bTree_Node(val, nullptr, nullptr, nullptr);
        return;
    }  // (else)

    auto res = internal_find(val);
    if (resolve(res)==val) return;  // (else)

    res.first->insert_at(res.second, val, nullptr, nullptr);

    if (res.first->size>order) split(res.first);
}

template <class T, index_t order>
int
bTree<T, order>::find(const T & val) {
    auto res = internal_find(val);

    if (res.second < res.first->size && resolve(res)==val) return 1;
    return 0;
}

#undef __constructor__
#undef __destructor__
#endif //DATASTRUCTURE_BTREE_HPP
