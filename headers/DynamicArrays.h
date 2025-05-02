#pragma once

#include <iostream>
#include <string>


using namespace std;

class DynamicArrays {
public:
    DynamicArrays(int capacity = 4); // the starting inventory is 4
    ~DynamicArrays();
    void Resize();
    void AddItem(string element);
    void RemoveItem(string element);   
    void DeSize();
    void Print();
    bool Find(string element);
private:    
    string* arr;
    int capacity;
    int size;
};