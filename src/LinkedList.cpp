#include "../headers/LinkedList.h"
//#include <algorithm>
LinkedList::LinkedList() {
    head = NULL;
}

void LinkedList::AddNodeInTheEnd(string description, vector<string> items, Treasure treasure) {
    Node* newNode = new Node;
    //Node* newNode = new Node{nullptr, Room()};  // Properly initialize
    newNode->room.setDescription(description);
    newNode->room.setItems(items);
    newNode->room.setTreasure(treasure);
    newNode->next = NULL;
    if (head == NULL) {
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

LinkedList::~LinkedList() {
    Node* temp = head;
    while (temp != NULL) {
        Node* next = temp->next;
        delete temp;
        temp = next;
    }
}   

void LinkedList::DisplayRoomDetails(int nodeNumber) {
    Node* temp = head;
    for (int i = 1; i < nodeNumber; i++) {
        temp = temp->next;
    }
    cout << temp->room.getDescription() << endl;
    if(temp->room.getItems().size() == 0){
        cout << "" << endl;
    }
    else{
        cout << "Items: ";
        vector<string> items = temp->room.getItems();
        for (int i = 0; i < items.size(); i++) {
            cout << items[i] << " ";
        }
        cout << endl;
    }

    if(temp->room.getTreasure().GetQuantity() == 0){
        cout << "" << endl;
    }
    else{
        cout << "Treasure: " << temp->room.getTreasure().GetQuantity() << " " << temp->room.getTreasure().GetType() << " " << "coins" << endl;
    }
}

// void LinkedList::itemTaken(string item, vector<string> items) {
//     auto it = find(items.begin(), items.end(), item); // Find first occurrence
//     if (it != items.end()) {
//         items.erase(it); // Erase only that occurrence
//     }
// }