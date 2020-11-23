#include <iostream>
#include "stack.h"
using namespace std;

int main(){//
    Stack mystack(5);
    for (int i = 0; i < 6; i++) {
        cout << mystack.isempty() << " ";
        cout << mystack.isfull() << " ";
        cout << mystack.push(i) << endl;
    }
    for (int i = 0; i < 6; i++) {
        Item cur = 0;
        cout << mystack.isempty() << " ";
        cout << mystack.isfull() << " ";
        cout << mystack.pop(cur) << " ";
        cout << cur << endl;
    }
    return 0;
}

Stack::Stack(int n) {
    if (n < 0 || n>MAX)n = MAX;//no overflow plz
    pitems = new Item[n];
    size = n;
    top = 0;
}

Stack::Stack(const Stack& st) {
    if (&st != this) {
        pitems = new Item[st.size];
        for (int i = 0; i < st.top; i++) {
            pitems[i] = st.pitems[i];
        }
        size = st.size;
        top = st.top;
    }
}

Stack::~Stack() {
    delete[] pitems;//safe!
}

bool Stack::isempty() const {
    return !top;
}
bool Stack::isfull() const {
    return size == top;
}
bool Stack::push(const Item& item) {
    if (size - top) {
        pitems[top] = item;
        top++;
        return true;
    }
    return false;
}
bool Stack::pop(Item& item) {
    if (top) {
        item = pitems[top - 1];
        top--;
        return true;
    }
    return false;
}

Stack& Stack::operator= (const Stack& st) {
    if (&st != this) {
        pitems = new Item[st.size];
        for (int i = 0; i < st.top; i++) {
            pitems[i] = st.pitems[i];
        }
        size = st.size;
        top = st.top;
    }
    return *this;
}