#include <iostream>
#include <fstream>
#include <algorithm>

// element_type shall be initialized with operator< and operator <=

typedef int element_type;
typedef size_t index_t;

// template <class element_type>
struct Node {
	element_type value;
	Node *next;
	
	Node(const element_type & val, Node *next_ptr = nullptr);
};

Node::Node(const element_type & val, Node *next_ptr) {
	this->value = val;
	this->next = next_ptr;
}

class Link_list {
private:
	Node *head, *tail;
	index_t size;
public:
	Link_list();
	~Link_list();

	void insert(const element_type & val, bool greater = false);
	index_t remove(const element_type & val);
	index_t find (const element_type & val);
	void read_from_file(const char *path);
	index_t length() const;
	void print() const;

    void self_reverse();
};

Link_list::Link_list(): head(nullptr), tail(nullptr), size(0)
{}

Link_list::~Link_list() {
	Node *iter = head;
	while (iter) {
		Node *next = iter->next;
		delete iter;
		iter = next;
	}
}

void
Link_list::insert(const element_type & val, bool greater) {
	if (!head) {
		head = tail = new Node(val, nullptr);
	}
	else if (val<head->value) {
		Node *new_head = new Node(val, head);
		head = new_head;
	}
	else {
		Node *iter = head;
		while (iter->next && (iter->next->value <= val)) iter = iter->next;
		
		Node *successor = iter->next;
		iter->next = new Node(val, successor);
		
		if (successor==nullptr) tail = iter->next;
		// think: extensible
	}
	
	size++;
}

index_t
Link_list::remove(const element_type & val) {
	if (!head) return 0;
	index_t count = 0;
	
	if (head->value==val) {
		if (head==tail) {
			delete head;
			head = tail = nullptr;
		}
		else {
			Node *new_head = head->next;
			delete head;
			head = new_head;
		}
		count = 1;
	}
	else {
		Node *iter = head;
		count = 2;
		
		while (iter->next && iter->next->value != val) {
			iter = iter->next;
			count++;
		}
		
		if (iter->next) {
			Node *rm = iter->next;
			iter->next = iter->next->next;
			delete rm;
			
			if (!iter->next) tail = iter;
		}
		else count = 0;
	}
	
    if (count) size--;
	return count;
}

index_t
Link_list::find(const element_type & val) {
	if (!head) return 0;
	
	index_t count = 1;
	
	Node *iter = head;
	while (iter && iter->value!=val) {
		iter = iter->next;
		count++;
	}
	
	if (iter) return count;
	else return 0;
}

void
Link_list::read_from_file(const char *path) {
	std::ifstream fin(path, std::ifstream::in|std::ifstream::binary);
	if (!fin) return;
	element_type *buf = new element_type;
	
	while(true) {
		fin.read((char *)(buf), sizeof(element_type));

		if (fin) {
			this->insert(*buf);
		}
		else {
			break;
		}
	}
	
	delete buf;
	fin.close();
}

index_t
Link_list::length() const {
	return this->size;
}

void
Link_list::print() const {
	if (!head) return;
	std::cout << head->value;
	
	Node *iter = head->next;
	
	while (iter) {
		std::cout << "->" << iter->value;
		iter = iter->next;
	}
}

void
Link_list::self_reverse() {
    if ((!head) || (!head->next)) return;

    Node *new_head = head->next, *old_head = head;
    tail = head;
    tail->next = nullptr;
    
    while (new_head != nullptr) {
        head = new_head;
        new_head = head->next;

        head->next = old_head;
        old_head = head;
    }
    
}

int main() {
    Link_list l;
    for (int i=10 ; i>=0 ; i--) {
        l.insert(i);
    }

    l.print();
    std::cout << "\nlen:" << l.length() << std::endl;
    
    l.self_reverse();
    l.remove(6);
    l.print();
    std::cout << "\nlen:" << l.length() << std::endl;
}