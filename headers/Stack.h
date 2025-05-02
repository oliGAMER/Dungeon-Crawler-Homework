#pragma once

#include <iostream>
#include <vector>



using namespace std;

class Stack{
private:
    vector<int> arr;

public:
    void push(int element);
    void pop();
    int peek();
    bool Empty();
};