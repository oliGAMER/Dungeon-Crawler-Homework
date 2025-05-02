#include "../headers/DynamicArrays.h"
#include <iostream> 
#include <string>
using namespace std;

DynamicArrays::DynamicArrays(int capacity) {
    this->capacity = capacity;
    this->arr = new string[this->capacity];
    this->size = 0;
}

DynamicArrays::~DynamicArrays() {
    delete[] this->arr;
}

void DynamicArrays::Resize() {
    int newCapacity = this->capacity * 2;
    string* temp = new string[newCapacity];
    for (int i = 0; i < this->size; i++) {
        temp[i] = this->arr[i];
    }
    delete[] this->arr;
    this->arr = temp;
    this->capacity = newCapacity;
}   

void DynamicArrays::AddItem(string element) {
    if (this->size == this->capacity) {
        Resize();
    }
    this->arr[this->size] = element;
    this->size++;
}

void DynamicArrays::RemoveItem(string element) {
    int index = -1;

    for (int i = 0; i < size; i++) {
        if (arr[i] == element) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "" << endl;
        return;
    }

    for (int i = index; i < size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    
    size--;

    if(size < capacity / 4){
        DeSize();
    }
    
}

void DynamicArrays::DeSize() {
    int newCapacity = this->capacity / 2;   
    if(newCapacity < 4){
        return;
    }
    string* temp = new string[newCapacity];
    for (int i = 0; i < size; i++) {
        temp[i] = this->arr[i];
    }
    delete[] this->arr;
    this->arr = temp;
    this->capacity = newCapacity;
}

void DynamicArrays::Print() {
    for (int i = 0; i < this->size; i++) {
        cout << this->arr[i] << ",";
    }
    cout << endl;
}

bool DynamicArrays::Find(string element) {
    for (int i = 0; i < this->size; i++) {
        if (this->arr[i] == element) {
            return true;
        }
    }
    return false;
}