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
    int GetSize(){return size;}
    string& operator[](int index) {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range");
        }
        return arr[index];
    }
private:    
    string* arr;
    int capacity;
    int size;
};