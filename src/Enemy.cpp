#include "../headers/Enemy.h"
#include <iostream> 

using namespace std;    

void Enemy::SetHealth(int health) {
    this->health = health;
}   

void Enemy::setDescription(string description) {
    this->description = description;
}   

string Enemy::getDescription() {
    return this->description;
}   

int Enemy::getHealth() {
    return this->health;
}

