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

    // member variables
    bTree_Node *rootptr;
    const index_t min = order/2;

    // private member functions
    result internal_find(const T & val);
    static int resolve(const result & r, const T & val);
    void split(bTree_Node *sp);

    void merge(bTree_Node l_sibling, bTree_Node r_sibling);

public:
    bTree();
    void insert(const T & val);
    int find(const T & val);
    void remove(const T & val);
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
int
bTree<T, order>::resolve(const bTree<T, order>::result & r, const T & val) {
    if (r.second==-1 || r.first->values[r.second]!=val) return 0;  // (else)
    return 1;
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
    /*
     * insert procedures:
     *  1. whole b tree is empty
     *      - make new root
     *  2. b tree is not empty
     *      - insert on leaf
     *      2.1. if a leaf max out after insertion
     *          - split the leaf recursively
     */
    if (!rootptr) {
        rootptr = new bTree_Node(val, nullptr, nullptr, nullptr);
        return;
    }  // (else)

    auto res = internal_find(val);
    if (resolve(res, val)) return;  // (else)

    res.first->insert_at(res.second, val, nullptr, nullptr);

    if (res.first->size>order) split(res.first);
}

template <class T, index_t order>
int
bTree<T, order>::find(const T & val) {
    auto res = internal_find(val);

    if (res.second < res.first->size && resolve(res, val)) return 1;
    return 0;
}

template <class T, index_t order>
void
bTree<T, order>::remove(const T & val) {
    /*
     * remove procedures:
     *  1. remove a value on branch
     *      - find the greatest(smallest) value on left(right) subtree, replace the to-be-removed value
     *      - continue on 2.: remove the value on the leaf
     *  2. remove a value on leaf
     *      - remove the value and update size of the node
     *      2.1. if the leaf node is root
     *          2.1.1. if root is empty
     *              - reset rootptr to nullptr
     *          2.1.2. else (if the root is not empty)
     *              - pass || do nothing
     *      2.2. else (if the leaf node isn't root)
     *          2.2.1. if the node size >= `bTree<T, order>::min`
     *              - pass || do nothing
     *          2.2.2. else (if the node size < `bTree<T, order>::min`)
     *              2.2.2.1. if more than one sibling's size > `bTree<T, order>::min`
     *                  - lend a value from sibling: sibling->father, father->this
     *              2.2.2.2. if both siblings's size is less than `bTree<T, order>::min`
     *                  - merge the node with its sibling
     */

    result res = internal_find(val);
    result orig = res;
    if (resolve(res, val)) return;  // value not exist, (else)

    auto get_lchild = [res] { return res.first->children[res.second]; };
    auto get_rchild = [res] { return res.first->children[res.second + 1]; };
    auto get_sibling = [res](char lr) {
        if (!res.first->father || (lr != 'l' && lr != 'r')) return nullptr;

        bTree_Node *fa = res.first->father;
        index_t pos = fa->withinNode_find(res.first->values[res.second]);

        if (lr == 'l') {
            if (pos < 0) return nullptr;
            else return fa->children[pos];
        } else /* if (lr=='r') */ {
            if (fa->size < pos + 2) return nullptr;
            else return fa->children[pos + 2];
        }
    };
    auto get_size = [res] { return res.first->size; };

    if (get_lchild) {
        if (get_lchild->size < get_rchild->size) {
            res = std::make_pair(get_rchild, 0);
            while (get_lchild) res = std::make_pair(get_lchild, 0);
        } else {
            res = std::make_pair(get_lchild, get_lchild->size - 1);
            while (get_lchild) res = std::make_pair(get_rchild, get_rchild->size - 1);
        }

        orig.first->values[orig.second] = res.first->values[res.second];
    }

    // deletion
    // no need to care for children
    // because this operation is always on leaf
    for (index_t i = res.second; i < res.first->size - 1; i++) res.first->values[i] = res.first->values[i + 1];
    res.first->size--;
    res.second--;

    if (get_size >= min) return;  // (else)

    bTree_Node *modify = get_sibling('r');
    bTree_Node *ls = get_sibling('l');
    if (!modify || modify->size < ls->size) modify = ls;

    if (modify->size>min) {
        // specify left or right sibling to modify
        if (modify==ls) {
            // TODO: modify insertion, complete father->withinNode_find in previous finding.
            res.first->insert_at(res.second, res.first->father->values[res.first->father->withinNode_find(val)], );
        }
        else {

        }
    }
    else {
        __constructor__
    }
}

#undef __constructor__
#undef __destructor__
#endif //DATASTRUCTURE_BTREE_HPP
