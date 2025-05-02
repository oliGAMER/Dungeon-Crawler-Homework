#pragma once
#include <string>
#include "DynamicArrays.h"
#include <vector>


using namespace std;

class Player{
private:
    int health;
    DynamicArrays inventory{4};
    string name;
    
public:
    //Constructor
    Player();
    void SetHealth(int health); 
    void SetInventory(string element);
    void SetName(string name);
    int getHealth();
    //string getInventory();
    string getName();
    void RemoveInventory(string element);
    bool CheckInventory(string element);
    void getStatus();
};
