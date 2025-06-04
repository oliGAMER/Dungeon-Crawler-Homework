#include "../headers/Dungeon.h"


using namespace std;    

Dungeon::Dungeon() {
    //this->playerPos = 0;
}

void Dungeon::newRoom(string description, vector<string> items, Treasure treasure) {
    rooms.AddNodeInTheEnd(description, items, treasure);
}      

void Dungeon::newEnemy(string description, int health) {
    Enemies.enqueue(description, health);
}

void Dungeon::DisplayRoomDetails(int Pos){
    rooms.DisplayRoomDetails(Pos);
}

void Dungeon::PlayerPathAdd(int PoS){
    PlayerPath.push(PoS);
}

int Dungeon::PlayerNow(){
    return PlayerPath.peek();
}

void Dungeon::PlayerPathBack(){
    PlayerPath.pop();
} 

bool Dungeon::PlayerAtTheBeginning(){
    cout << PlayerPath.Empty() << endl;
    return PlayerPath.Empty();
}

// void Dungeon::RemoveItem(string item, vector<string> items){
//     rooms.itemTaken(item,items);
// }

void Dungeon::EnemyStats(){
    Enemies.dequeue();
}

std::string Dungeon::getRoomDescriptionAt(int index) {
    return rooms.getDescriptionAt(index - 1);
}