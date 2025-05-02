#include "../headers/Stack.h"
#include <iostream>
#include <vector>

using namespace std;

void Stack::push(int element) {
    arr.push_back(element);// do I need to add overflow checks
}

void Stack::pop() {
    if (arr.empty()) {
        cout << "You are at the starting point" << endl;
    } else {
        arr.pop_back();
    }
}

int Stack::peek() {
    return arr.back();
}

bool Stack::Empty(){
    return arr.empty();
}

