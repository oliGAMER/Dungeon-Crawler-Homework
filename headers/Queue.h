#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Enemy.h"

using namespace std;

class Queue{
private:
    vector<Enemy> arr;
    int front;
    int rear;
    int capacity; 
public:
    Queue(int size);
    void enqueue(string description, int health);
    void dequeue();
};