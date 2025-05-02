#pragma once

#include "Treasure.h"
#include <iostream>





#include <string>
#include <vector>
using namespace std;


class Room{
private:
    string description;
    //string exits[4];
    vector<string> items;
    Treasure treasure;
public:
    string getDescription();
    vector<string> getItems();    
    Treasure getTreasure();
    void setDescription(string description);    
    void setItems(vector<string> items);
    void setTreasure(Treasure treasure);
};








