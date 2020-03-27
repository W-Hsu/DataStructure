/* created by whsu */
/* 2020.03.26 */
/* Data Structure homework 2.4 */

#include <iostream>
#include <string>

#define pass

struct my_Stack {
    char data[10000];
    long top_index;

    my_Stack();
    char top() const;
    void pop();
    void push(char c);
    int empty() const;
};

my_Stack::my_Stack() {
    top_index = -1;
}

char my_Stack::top() const {
    return data[top_index];
}

void my_Stack::pop() {
    top_index--;
}

void my_Stack::push(char c) {
    data[++top_index] = c;
}

int my_Stack::empty() const {
    return top_index==-1 ? 1 : 0;
}

int main() {
    std::string mid, back;
    std::getline(std::cin, mid);

    size_t i=0, len = mid.length();
    my_Stack s;
    while (i<len) {
        if (mid[i]==' ' || mid[i]=='\t') {
            pass;
        } else if (('A'<=mid[i] && mid[i]<='Z') || ('a'<=mid[i] && mid[i]<='z')) {
            back.push_back(mid[i]);
        } else {
            switch(mid[i]) {
                case '+':
                case '-':
                    while (!s.empty() && s.top()!='(') {
                        back.push_back(s.top());
                        s.pop();
                    }
                case '(':
                    s.push(mid[i]);
                    break;
                case '*':
                case '/':
                    while (!s.empty() && s.top()!='(' && s.top()!='+' && s.top()!='-') {
                        back.push_back(s.top());
                        s.pop();
                    }
                    s.push(mid[i]);
                    break;
                case ')':
                    while (!s.empty() && s.top()!='(') {
                        back.push_back(s.top());
                        s.pop();
                    }
                    if (s.empty()) {
                        std::cout << "Oops! Unpaired bracket!" << std::endl;
                        return 1;
                    } else {
                        s.pop();
                    }
                    break;
                default:
                    break;
            }
        }
        i++;
    }
    while (!s.empty()) {
        if (s.top()==')' || s.top()=='(') {
            std::cout << "Oops! Unpaired bracket!" << std::endl;
            return 1;
        }
        back.push_back(s.top());
        s.pop();
    }
    std::cout << back;
    return 0;
}
