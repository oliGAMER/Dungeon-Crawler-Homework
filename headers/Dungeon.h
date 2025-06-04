#pragma once

#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"
#include <iostream>
#include "Treasure.h"


using namespace std;

class Dungeon{
private:
    LinkedList rooms;
    Queue Enemies{10};
    Stack PlayerPath;
    //int playerPos;
public:
    //constructor
    Dungeon();
    //void PlayerMovement(string action);
    void newRoom(string description, vector<string> items, Treasure treasure);
    void newEnemy(string description, int health);
    void DisplayRoomDetails(int Pos);
    void PlayerPathAdd(int PoS);
    void PlayerPathBack();
    int PlayerNow();
    bool PlayerAtTheBeginning();
    //void RemoveItem(string item, vector<string> items);
    void EnemyStats();
    string getRoomDescriptionAt(int index);
    };  
