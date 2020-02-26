//
// Created by william on 2020/2/3.
//

#include <algorithm>
#include <cstring>

#ifndef DATASTRUCTURE_BTREE_HPP
#define DATASTRUCTURE_BTREE_HPP

#define __constructor__
#define __destructor__

template <class T, size_t order=6>
class bTree {
private:
    // node class
    struct bTree_Node {
        size_t size;  // numbers of element
        T values[order+1];  // value
        bTree_Node *children[order+2];  // children pointers
        bTree_Node *father;  // easy to trace back

        bTree_Node() = default;
        bTree_Node(const T & val, bTree_Node *father, bTree_Node *lchild, bTree_Node *rchild);
        T *withinNode_find(const T & val);
        void insert_at(size_t pos, const T & val, bTree_Node *child_pointer, bTree_Node *rchild_pointer);
    };  // class bTree_Node

    // stores internal searching result
    // first: Node where result is
    // second: result's position in Node
    // tip: if result do not matches, then refers to the location to insert into (lower bound)
    typedef std::pair<bTree_Node*, size_t> result;

    bTree_Node *rootptr;
    const size_t min = order/2 + 1;

    result internal_find(const T & val);
    static T resolve(const result & r);
    void split(bTree_Node *sp);

public:
    bTree();
    void insert(const T & val);
    int find(const T & val);
};  // class bTree

template <class T, size_t order>
__constructor__
 bTree<T, order>::bTree_Node::bTree_Node(const T & val, bTree<T, order>::bTree_Node *fa, bTree<T, order>::bTree_Node *lchild, bTree<T, order>::bTree_Node *rchild) {
     memset(values, 0, (order+1)*sizeof(T));
     memset(children, 0, (order+2)*sizeof(void*));
     children[0] = lchild, children[1] = rchild;
     values[0] = val;
     father = fa;
     size = 1;
 }

template <class T, size_t order>
T*
 bTree<T, order>::bTree_Node::withinNode_find(const T & val) {
    return std::lower_bound((T*)(values), values+size, val) - 1;
 }

template <class T, size_t order>
void
 bTree<T, order>::bTree_Node::insert_at(size_t pos, const T & val, bTree_Node *lchild_pointer, bTree_Node *rchild_pointer) {
     children[size] = children[size+1];

     for (size_t i=size-1 ; i>pos ; i--) {
         values[i+1] = values[i];
         children[i+1] = children[i];
     }
     values[pos+1] = val;
     children[pos+1] = lchild_pointer;
     children[pos+2] = rchild_pointer;

     size++;
 }

template <class T, size_t order>
typename bTree<T, order>::result
 bTree<T, order>::internal_find(const T & val) {
    if (!rootptr) return std::make_pair(nullptr, 0);

    auto iter = rootptr;
    T *result = nullptr;
    size_t pos = 0;
    while (true) {
        result = iter->withinNode_find(val);  // **open interval** feature of stl
        pos = result - iter->values;
        if (*result==val) break;
        else if (iter->children[pos+1]) iter = iter->children[pos+1];
        else break;
    }

    return std::make_pair(iter, pos);
 }

template <class T, size_t order>
T
 bTree<T, order>::resolve(const bTree<T, order>::result & r) {
     return r.first->values[r.second];
 }

template <class T, size_t order>
void
 bTree<T, order>::split(bTree<T, order>::bTree_Node *sp) {
    if (!sp) return;  // (else)

    // make a new node (right child)
    auto fa = sp->father;
    auto rchild = new bTree_Node();

    rchild->children[0] = sp->children[(order/2)+1];
    for (size_t i=(order/2)+1, j=0 ; i<=order ; i++, j++) {
        rchild->values[j] = sp->values[i];
        rchild->children[j+1] = sp->children[i+1];

        if (rchild->children[j+1]) rchild->children[j+1]->father = rchild;  // important - New father!

        rchild->size++;
    }

    sp->size = order - rchild->size;
    /*
    sp->children[order/2] = 0;
    for (size_t i=(order/2)+1 ; i<order ; i++) {
        sp->children[i] = nullptr;
        sp->values[i] = 0;
    }
     */

    // make a new root
    if (!fa) rootptr = rchild->father = sp->father = new bTree_Node(sp->values[order/2], nullptr, sp, rchild);
    // insert into father
    else {
        rchild->father = fa;
        auto insert_pos = fa->withinNode_find(sp->values[order/2]);
        fa->insert_at(insert_pos-fa->values, sp->values[order/2], sp, rchild);
        if (fa->size>order) split(fa);
    }
}

template <class T, size_t order>
__constructor__
 bTree<T, order>::bTree()
:rootptr(nullptr)
{}

template <class T, size_t order>
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

template <class T, size_t order>
int
 bTree<T, order>::find(const T & val) {
     auto res = internal_find(val);
    auto target = std::make_pair(res.first, res.second+1);

    if (res.second+1 < res.first->size) {
         if (resolve(target)) return 1;
     }
     std::cout << resolve(target) << std::endl;
     return 0;
 }


#undef __constructor__
#undef __destructor__
#endif //DATASTRUCTURE_BTREE_HPP
