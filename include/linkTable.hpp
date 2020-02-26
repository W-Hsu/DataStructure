//
// Created by william on 2020/2/20.
//

#ifndef DATASTRUCTURE_LINKTABLE_HPP
#define DATASTRUCTURE_LINKTABLE_HPP

template <class T>
struct linkTable_Node {
    T value;
    linkTable_Node *next;
    linkTable_Node *last;

    linkTable_Node() = default;
    linkTable_Node(T _value, linkTable_Node *_next, linkTable_Node *_last);
};

template <class T>
linkTable_Node<T>::linkTable_Node(T _value, linkTable_Node<T> *_next, linkTable_Node<T> *_last)
:value(_value), next(_next), last(_last)
{}

template <class T>
class linkTable {
private:
    linkTable_Node<T> *head;
    linkTable_Node<T> *tail;
    size_t size;
    linkTable_Node<T> *internal_find(T & value);
public:
    linkTable() = default;
    void insert_front(T & value);
    void insert_back(T & value);
    int remove(T & value);
};

template <class T>
linkTable_Node<T>* linkTable<T>::internal_find(T & value) {
    auto iter = head;
    do {
        if (iter->value == value) return iter;
    } while (iter->next);
    return nullptr;
}

template <class T>
void linkTable<T>::insert_front(T &value) {
    if (size==0) {
        head = tail = new linkTable_Node<T>(value, nullptr, nullptr);
    } else {
        auto old_head = head;
        head = new linkTable_Node<T>(value, old_head, nullptr);
        old_head->last = head;
    }
    size++;
}

template <class T>
void linkTable<T>::insert_back(T &value) {
    if (size==0) {
        head = tail = new linkTable_Node<T>(value, nullptr, nullptr);
    } else {
        auto old_tail = tail;
        tail = new linkTable_Node<T>(value, nullptr, old_tail);
        old_tail->last = tail;
    }
    size++;
}

template <class T>
int linkTable<T>::remove(T &value) {
    auto rm = internal_find(value);
    if (rm) {
        rm->last->next = rm->next;
        rm->next->last = rm->last;
        delete (rm);
        return 1;
    }
    else return 0;
}

#endif //DATASTRUCTURE_LINKTABLE_HPP
