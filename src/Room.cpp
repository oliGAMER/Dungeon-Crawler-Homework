#include "../headers/Room.h"

using namespace std;

string Room::getDescription() {
    return this->description;
}

vector<string> Room::getItems() {
    return this->items;
}   

Treasure Room::getTreasure() {
    return this->treasure;
}   

void Room::setDescription(string description) {
    this->description = description;
}   

void Room::setItems(vector<string> items) {
    this->items = items;
}   

void Room::setTreasure(Treasure treasure) {
    this->treasure = treasure;
}


