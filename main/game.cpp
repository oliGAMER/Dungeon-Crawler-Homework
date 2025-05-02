#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
// for the boss level
#include <thread>
#include <chrono>
//---------------------------------
#include "../headers/DynamicArrays.h"
#include "../headers/Player.h"
#include "../headers/Dungeon.h"   
#include "../headers/Treasure.h"
#include "../headers/Stack.h"

using namespace std;

void SubMenu(Dungeon& dungeon, int& PlayerPos, Player& p1,bool levelOneBeg) {
    int userResponse;
    cout << endl;
    cout << "You can do the following actions: " << endl;
    cout << "1. Move to the next room" << endl;
    cout << "2. Check player stats" << endl;
    cout << "3. Go to the back room" << endl;
    cout << "Enter the number of the action you want to do: ";

    cin >> userResponse;

    while (userResponse == 2) {  // Loop until user selects another action
        cout << "Player stats: " << endl;
        p1.getStatus();
        cout << "You can do the following actions: " << endl;
        cout << "1. Move to the next room" << endl;
        cout << "2. Check player stats" << endl;
        cout << "3. Go to the back room" << endl;
        cout << "Enter the number of the action you want to do: ";
        cin >> userResponse;
    }
    switch (userResponse) {
        case 1:
            if(!levelOneBeg){dungeon.PlayerPathAdd(++PlayerPos);} // boundary case where user cannot go to the next room from the beginning
            else{
                PlayerPos = 1;
                dungeon.PlayerPathAdd(++PlayerPos);
            }
            break;
        case 3:
            if(levelOneBeg){// Boundary Case : Do not go to the backroom from the beginning
                cout << "You are at the beginning of the dungeon" << endl;
                cout << "As a result of your ignorance you will be pushed into the next room" << endl;
                PlayerPos = 1;
                dungeon.PlayerPathAdd(++PlayerPos);
            }
            else{
                cout << "You went back to the previous room" << endl;
                dungeon.PlayerPathBack();
            }
            break;
        default: // if the user enters anything other than the given options
            cout << "Invalid action!" << endl;
            cout << "As a result of your ignorance you will be pushed into the next room" << endl;
            dungeon.PlayerPathAdd(++PlayerPos);
            break;
    }
}


void FinalBossFight(Player& p1) {
    cout << "The Final Boss appears!" << endl;

    if (!p1.CheckInventory("sword")) {
        this_thread::sleep_for(chrono::seconds(2));
        cout << "You have no weapon! The boss crushes you." << endl;
        p1.SetHealth(0);
        return;
    }


    cout << "You have to slash the sword when the boss does his move!" << endl;
    cout << "Prepare yourself..." << endl;

    // Wait for 2 seconds
    this_thread::sleep_for(chrono::seconds(3));

    using Clock = std::chrono::high_resolution_clock;

    cout << "\nNOW! Press 'f' to slash your sword!: ";
    
    string playerInput;
    // Start the timer
    auto start = Clock::now();
    cin >> playerInput;  

    // End the timer after input
    auto end = Clock::now();
    auto reactionTime = chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0; // Convert ms to sec


    // Check if the player reacted in time
    if ((playerInput == "f" || playerInput == "F") && reactionTime < 3.0) {
        cout << "You slash the boss just in time and win the fight!" << endl;
        cout << "Congratulations! You have escaped the dungeon!" << endl;
    } else {
        cout << "You hesitated... The boss crushes you!" << endl;
        p1.SetHealth(0);
    }
}

int main(){
    Player p1;
    p1.SetHealth(100);
    string name;
    string startPlay;

    //flags for the backtrack 
    bool level1Done = false;


    //The beginning instructions
    cout << "Hello World" << endl;
    cout << "This is a text based game, type your answers into the terminal" << endl;
    cout << "Type the first letter of the word to continue" << endl;
    // other instructions


    //Does the user want to play the game or not
    cout << "Type 'y' or 'yes' to play";
    cin >> startPlay;
    if (startPlay == "y" || startPlay == "yes") {
        cout << "Enter your name: ";
        cin >> name;
        p1.SetName(name);
        cout << "Welcome to Escape the dungeon " << p1.getName() << endl;
    } else {
        cout << "Oh well, I guess you are not up for the challenge" << endl;
        cout << "Exiting the game..." << endl;
        return 0;
    }


    //The beginning scenes
    cout << "You fell down a trap door and landed in a dungeon" << endl;
    cout << endl;
    cout << "Remember you can just type the first letter of the word" << endl;

    //Setting up the whole map and scenes
    // the first room
    Dungeon dungeon;
    Treasure t1;
    t1.SetType("Gold");
    t1.SetQuantity(5);
    dungeon.newRoom("You are in a dark room", {"torch"}, t1);

    // the second room
    Treasure t2;
    t2.SetQuantity(0);
    dungeon.newRoom("You enter the room to the south and come to a pond", {}, t2);

    // the third room
    Treasure t3;
    t3.SetQuantity(0);
    dungeon.newRoom("You stumble upon an unused furnace while taking north", {"sword"}, t3);

    // the fourth room
    Treasure t4;
    t4.SetQuantity(0);
    dungeon.newEnemy("goblin", 10);
    dungeon.newEnemy("creeper", 20);
    dungeon.newRoom("TWO ALIENS APPEAR OUT OF NOWHERE", {}, t4);

    // the fifth room
    Treasure t5;
    t5.SetQuantity(0);
    dungeon.newEnemy("Final Boss", 100);
    dungeon.newRoom("Final Boss Room", {}, t5);


    //The game loop
    string action = "";
    int PlayerPos = 1;
    int numMoves = 1;
    dungeon.PlayerPathAdd(PlayerPos);


    while (p1.getHealth() > 0 && numMoves < 7) {
        int currentRoom = dungeon.PlayerNow();
        string action = "";
    
        dungeon.DisplayRoomDetails(currentRoom);
    
        switch (currentRoom) {
            case 1:
                cout << "You have " << 7-numMoves << " moves left" << endl;
                cout << endl;
                cout << "Will you take the torch or the gold?" << endl;
                cout << "Press 'g' for gold or 't' for torch: ";
                cin >> action;
                if (action == "g") {
                    if(!level1Done){
                        p1.SetInventory(to_string(t1.GetQuantity()) + " " + t1.GetType() + " coins");
                        level1Done = true; // if the player has taken the gold, they cannot take it again
                        cout << "You have taken the gold" << endl;
                    }
                    else{
                        cout << "You already took the gold" << endl;
                    }
                } else {
                    cout << "You have taken the torch" << endl;
                    p1.SetInventory("torch");
                }
                numMoves++;
                SubMenu(dungeon, PlayerPos, p1,true);
                system("cls"); // only for Microsoft Windows

                break;
    
            case 2:
                cout << "You have " << 7-numMoves << " moves left" << endl;
                cout << endl;
                cout << "You have to swim across this swampy pond or jump over it" << endl;
                cout << "Press 's' to swim or 'j' to jump: ";
                cin >> action;
                if (action == "s") {
                    cout << "You swam across the pond but your torch is extinguished" << endl;  // The torch is extinguished
                    p1.RemoveInventory("torch");
                } else if (action == "j") {
                    cout << "Ouch !!! That jump almost costed a knee" << endl;
                    cout << endl;
                    p1.SetHealth(p1.getHealth() - 10);
                }
                numMoves++;
                SubMenu(dungeon, PlayerPos, p1,false);
                system("cls");
                break;
    
            case 3:
                cout << "You have " << 7-numMoves << " moves left" << endl;
                cout << endl;
                if (p1.CheckInventory("torch")) {
                    cout << "Use the torch to light up the furnace" << endl;
                    cout << "Press 's' to take the sword: ";
                    cin >> action;
                    if (action == "s") {
                        p1.SetInventory("sword");
                        dungeon.PlayerPathAdd(++PlayerPos);
                    }
                } else {
                    cout << "You do not have anything to light up the furnace" << endl;
                    // Wait for 2 seconds before the leprechaun appears
                    this_thread::sleep_for(chrono::seconds(3));
                    cout << "A leprechaun appears and offers you a sword but he wants to take your gold" << endl;
                    cout << "Press 'y' to accept or 'n' to decline: ";
                    cin >> action;
                    if (action == "y") {
                        p1.SetInventory("sword");
                        p1.RemoveInventory(to_string(t1.GetQuantity()) + " " + t1.GetType() + " coins");
                        dungeon.PlayerPathAdd(++PlayerPos);
                    }
                    else{
                        cout << "You have declined the offer" << endl;
                        cout << "The leprechaun disappears" << endl;
                        dungeon.PlayerPathAdd(++PlayerPos);
                    }

                }
                numMoves++;

                system("cls");
                break;
    
            case 4:
                cout << "You have " << 7-numMoves << " moves left" << endl;
                cout << endl;
                dungeon.EnemyStats(); // for goblin
                dungeon.EnemyStats(); // for the creeper
                cout << "Press 'f' to fight: ";
                cin >> action;
                if(p1.CheckInventory("sword")){
                    cout << "The goblin bites you on the leg" << endl;
                    p1.SetHealth(p1.getHealth() - 10);
                    this_thread::sleep_for(chrono::seconds(2));
                    cout << "One slash of the sword annhilates the creeper and the goblin" << endl;
                    this_thread::sleep_for(chrono::seconds(2));
                }
                else{
                    cout << "The creeper explodes on you" << endl;
                    p1.SetHealth(p1.getHealth() - 20);
                    this_thread::sleep_for(chrono::seconds(2));
                    cout << "You crush the goblin and the creeper with your bare hands" << endl; 
                }
                numMoves++;
                system("cls");
                cout << "Player stats: " << endl;
                p1.getStatus();

                if(p1.CheckInventory(to_string(t1.GetQuantity()) + " " + t1.GetType() + " coins")){
                    cout << "Do you want to purchase a health potion to heal yourself?" << endl;
                    cout << "Press 'y' for yes or 'n' for no: ";
                    cin >> action;
                    if (action == "y") {
                        p1.RemoveInventory(to_string(t1.GetQuantity()) + " " + t1.GetType() + " coins"); 
                        p1.SetHealth(p1.getHealth() + 20);
                        cout << "You have healed yourself" << endl;
                        cout << "Player stats: " << endl;
                        p1.getStatus();
                    }
                }

                system("cls");
                dungeon.PlayerPathAdd(++PlayerPos);
                break;

            case 5:
                cout << "You have " << 7-numMoves << " moves left" << endl;
                cout << endl;
                FinalBossFight(p1);
                if (p1.getHealth() <= 0) {
                    cout << "Game Over. You were defeated by the Final Boss." << endl;
                    return 0; 
                }
                cout << "You have defeated the boss and escaped the dungeon!" << endl;
                p1.getStatus();
                return 0; 
        
                break;
    
            default:
                cout << "Invalid room number!" << endl;
                break;
        }
    }
    if(p1.getHealth() <= 0){
        cout << "You have lost the game" << endl;
    }
    else if(numMoves >= 7){
        cout << "You have run out of moves" << endl;
        cout << "You have lost the game" << endl;
    }
    else{
        cout << "You have won the game" << endl;
    }   
    return 0;
}