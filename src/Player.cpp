#include "../headers/Player.h"
#include <iostream> 

using namespace std;

Player::Player() {
    this->health = 0;
    this->name = "";
}

void Player::SetHealth(int health) {
    this->health = health;
}   

void Player::SetInventory(string element) {
    this->inventory.AddItem(element);
}

vector<string> Player::GetInventoryVector(){
    vector<string> result;
    for(int i = 0; i < inventory.GetSize(); i++) {
        result.push_back(inventory[i]);
    }
    return result;
}

void Player::SetName(string name) {
    this->name = name;
}

int Player::getHealth() {
    return this->health;
}

void Player::RemoveInventory(string s) {
    this->inventory.RemoveItem(s);
}

/*void Player::removeInventory(string element) {
    if (!this->inventory.Find(element)) {
        cout << "Item not found in inventory!" << endl;
    } else {
        this->inventory.RemoveItem(element);
        cout << element << " removed from inventory." << endl;
    }
}*/

bool Player::CheckInventory(string s) {
    return this->inventory.Find(s);
}

void Player::getStatus() {
    cout << "Player health: " << this->health << endl;
    cout << "Player inventory: ";
    this->inventory.Print();
    cout << endl;
}

string Player::getName() {
    return this->name;
}