#include "../headers/Queue.h"
#include <iostream>
#include <vector>

using namespace std;

Queue::Queue(int size) {
    this->capacity = size;
    this->front = 0;
    this->rear = 0;
    this->arr = vector<Enemy>(this->capacity);
}
void Queue::enqueue(string description, int health) {
    if (this->rear == this->capacity) {
        cout << "No more enemies can be added" << endl;
    } else {
        this->arr[this->rear].setDescription(description);
        this->arr[this->rear].SetHealth(health);
        this->rear++;
    }
}

void Queue::dequeue() {
    if (this->front == this->rear) {
        cout << "No more enemies to fight" << endl;
    } else {
        cout << this->arr[this->front].getDescription() << " " << this->arr[this->front].getHealth()  << " " << "hp" << endl;
        this->front++;
    }
}