/* created by whsu */
/* 2020.03.26 */
/* Data Structure homework 2.3 */

#include <iostream>

typedef int Element_t;

struct Node {
    Element_t data;
    Node *next;
};

struct Link_List {
    Node head;
};

void delete_range(Link_List & l, Element_t x, Element_t y) {
    if (x>y) {
        Element_t tmp = x;
        x = y;
        y = tmp;
    }

    Node *iter = &(l.head);
    while (iter->next) {
        if (x<iter->next->data && iter->next->data<y) {
            Node *d = iter->next;
            iter->next = iter->next->next;
            delete d;
        } else {
            iter = iter->next;
        }
    }
}

int main() {
    Link_List l;
    auto iter = &(l.head);
    for (int i=0 ; i<100 ; i++) {
        iter->next = new Node;
        iter->next->data = i;
        iter = iter->next;
    }
    iter->next = nullptr;

    delete_range(l, 50, 70);

    iter = &(l.head);
    while (iter->next) {
        iter = iter->next;
        std::cout << iter->data << " -> ";
    }
    std::cout << "*nullptr";

    iter = &(l.head);
    while (iter->next) {
        auto d = iter;
        iter = iter->next;
        if (d != &(l.head)) delete d; 
    }
}
