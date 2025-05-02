#include "../headers/Treasure.h"
#include <iostream>

using namespace std;

void Treasure::SetType(string type) {
    this->type = type;
}   

void Treasure::SetQuantity(int quantity) {
    this->quantity = quantity;
}

string Treasure::GetType() {
    return this->type;
}   


int Treasure::GetQuantity() {
    return this->quantity;
}
