#pragma once
#include <iostream> 
#include <string>
#include <vector>
#include "Room.h"
#include "Treasure.h"


using namespace std;


struct Node{

    Node* next;
    Room room;
};

class LinkedList{
private:
    Node* head;
public:
    LinkedList();
    ~LinkedList();  
    void AddNodeInTheEnd(string description, vector<string> items, Treasure treasure);
    void DisplayRoomDetails(int nodeNumber);
    //void itemTaken(string item, vector<string> items);
    string getDescriptionAt(int index);
};