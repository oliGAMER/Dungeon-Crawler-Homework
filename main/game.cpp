// Headers for the game
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <queue>
#include "../headers/DynamicArrays.h"
#include "../headers/Player.h"
#include "../headers/Dungeon.h"
#include "../headers/Treasure.h"
#include "../headers/Stack.h"
#include <SFML/Graphics.hpp>
#include "../headers/Button.h"
#include "../headers/Leaderboard.h"
#include <iomanip>

using namespace std;

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    LEADERBOARD
};

enum PlayingState {
    INTRO,
    ROOM_NAVIGATION,
    DECISION_MAKING,
    COMBAT,
    BOSS_FIGHT,
    GAME_END
};

struct CombatEvent {
    string source;  // whos intiaitng the action
    string target;  
    string action;  // Type of action
    int value;      // Numerical Value (Damage Amount)
    int priority;   // Determines execution
    
    bool operator<(const CombatEvent& other) const {
        return priority < other.priority; 
    }
};

void displayRoomInfo(sf::RenderWindow& window, sf::Font& font, Dungeon& dungeon, int roomIndex, int movesLeft, Player& p1, float windowWidth) {
    // Room title/description
    sf::Text roomDesc;
    roomDesc.setFont(font);
    roomDesc.setString(dungeon.getRoomDescriptionAt(roomIndex));
    roomDesc.setCharacterSize(24);
    roomDesc.setFillColor(sf::Color::White);
    sf::FloatRect textRect = roomDesc.getLocalBounds();
    roomDesc.setOrigin(textRect.width / 2, 0);
    roomDesc.setPosition(windowWidth / 2, 100);
    window.draw(roomDesc);
    
    // Moves left indicator
    sf::Text movesText;
    movesText.setFont(font);
    movesText.setString("Moves left: " + std::to_string(7 - movesLeft));
    movesText.setCharacterSize(18);
    movesText.setFillColor(sf::Color::Yellow);
    movesText.setPosition(windowWidth - 150, 20);
    window.draw(movesText);
    
    // Player health
    sf::Text healthText;
    healthText.setFont(font);
    healthText.setString("Health: " + std::to_string(p1.getHealth()));
    healthText.setCharacterSize(18);
    healthText.setFillColor(sf::Color::Green);
    healthText.setPosition(50, 20);
    window.draw(healthText);
}


void createRoomButtons(int roomNumber, vector<Button>& buttons, sf::Font& font, float windowWidth, float windowHeight, Player& p1, bool& level1Done, PlayingState& playingState, Treasure& t1, int& numMoves, int& currentRoom, string& statusMessage, float& statusMessageTimer, bool& torchTaken, int& previousRoom, Dungeon& dungeon,  sf::RenderWindow& window) {
    buttons.clear(); // Clear previous buttons
    
    switch(roomNumber) {
        case 1: // First room - torch or gold
            buttons.push_back(Button(
                windowWidth / 2 - 220, windowHeight / 2 + 50,
                200, 50, &font, "Take Gold",
                sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                [&p1, &level1Done, &playingState, &t1, &statusMessage, &statusMessageTimer]() { 
                    // Gold selection logic
                    if(!level1Done) {
                        p1.SetInventory(to_string(t1.GetQuantity()) + " " + t1.GetType() + " coins");
                        level1Done = true; // mark gold coins as taken
                        statusMessage = "You collected 5 gold coins!";
                        statusMessageTimer = 5.0f;
                    }
                    playingState = ROOM_NAVIGATION;
                }
            ));
            
            if (!torchTaken || (torchTaken && previousRoom == 2)) {
                buttons.push_back(Button(
                    windowWidth / 2 + 20, windowHeight / 2 + 50,
                    200, 50, &font, "Take Torch",
                    sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                    [&p1, &torchTaken, &playingState, &statusMessage, &statusMessageTimer]() { 
                        if (!p1.CheckInventory("torch")) {
                            p1.SetInventory("torch");
                            torchTaken = true;
                            statusMessage = "You picked up the torch!";
                        } else {
                            statusMessage = "Cannot carry more than one torch.";
                        }
                        statusMessageTimer = 5.0f;
                        playingState = ROOM_NAVIGATION;
                    }
                ));
            } else {
                // Add disabled button
                buttons.push_back(Button(
                    windowWidth / 2 + 20, windowHeight / 2 + 50,
                    200, 50, &font, "Take Torch",
                    sf::Color(50, 50, 50), sf::Color(50, 50, 50), sf::Color(50, 50, 50),
                    []() {  }
                ));
            }
            break;
            
        case 2:
            buttons.push_back(Button(
                windowWidth / 2 - 220, windowHeight / 2 + 50,
                200, 50, &font, "Swim Across",
                sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                [&p1, &numMoves, &playingState, &currentRoom, &statusMessage, &statusMessageTimer]() { 
                    if (p1.CheckInventory("torch")) {
                        p1.RemoveInventory("torch");
                        statusMessage = "You swam across the pond but your torch is extinguished!";
                    } else {
                        statusMessage = "You swam across the pond.";
                    }
                    statusMessageTimer = 5.0f;
                    
                    numMoves++;
                    currentRoom++;
                    playingState = ROOM_NAVIGATION;
                }
            ));
            
            buttons.push_back(Button(
                windowWidth / 2 + 20, windowHeight / 2 + 50,
                200, 50, &font, "Jump Over",
                sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                [&p1, &numMoves, &playingState, &currentRoom, &statusMessage, &statusMessageTimer]() { 
                    p1.SetHealth(p1.getHealth() - 10);
                    statusMessage = "Ouch!!! That jump almost cost you a knee.";
                    statusMessageTimer = 5.0f;
                    
                    numMoves++;
                    currentRoom++;
                    playingState = ROOM_NAVIGATION;
                }
            ));
            
            buttons.push_back(Button(
                windowWidth / 2 - 200, windowHeight / 2 - 50,
                400, 40, &font, "You have to swim across or jump over the pond",
                sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
                []() { }
            ));
        break;   

        case 3: // Furnace room
            // Common instruction text
            buttons.push_back(Button(
                windowWidth / 2 - 200, windowHeight / 2 - 80,
                400, 40, &font, "You stumble upon an unused furnace",
                sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
                []() { }
            ));
            
            if (p1.CheckInventory("torch")) {
                // Player has torch - show option to light furnace
                buttons.push_back(Button(
                    windowWidth / 2 - 150, windowHeight / 2,
                    300, 50, &font, "Use Torch to Light Furnace",
                    sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                    [&p1, &numMoves, &currentRoom, &statusMessage, &statusMessageTimer, &dungeon]() {
                        p1.SetInventory("sword");
                        statusMessage = "You lit the furnace and found a sword!";
                        statusMessageTimer = 5.0f;
                        
                        numMoves++;
                        currentRoom++;
                        dungeon.PlayerPathAdd(currentRoom);
                    }
                ));
            } else {
                // Create leprechaun dialogue and trade options
                static bool leprechaunAppeared = false;
                static sf::Clock leprechaunTimer;
                
                if (!leprechaunAppeared) {
                    // Reset timer on first entry to room without torch
                    leprechaunTimer.restart();
                    leprechaunAppeared = true;
                    
                    buttons.push_back(Button(
                        windowWidth / 2 - 200, windowHeight / 2,
                        400, 40, &font, "You don't have anything to light the furnace",
                        sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
                        []() { }
                    ));
                    
                    // Add a waiting message
                    buttons.push_back(Button(
                        windowWidth / 2 - 150, windowHeight / 2 + 50,
                        300, 50, &font, "Wait...",
                        sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                        [&statusMessage, &statusMessageTimer, &previousRoom]() {
                            statusMessage = "Waiting in the darkness...";
                            statusMessageTimer = 5.0f;
                            previousRoom = 0; // Reset previous room to indicate waiting
                        }
                    ));
                } else if (leprechaunTimer.getElapsedTime().asSeconds() > 3.0f) {
                    // After 3 seconds, show leprechaun offer
                    buttons.push_back(Button(
                        windowWidth / 2 - 200, windowHeight / 2,
                        400, 40, &font, "A leprechaun offers you a sword for your gold",
                        sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
                        []() { }
                    ));
                    
                    buttons.push_back(Button(
                        windowWidth / 2 - 220, windowHeight / 2 + 50,
                        200, 50, &font, "Accept Trade",
                        sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                        [&p1, &numMoves, &currentRoom, &statusMessage, &statusMessageTimer, &dungeon, &t1]() {
                            if (p1.CheckInventory(to_string(t1.GetQuantity()) + " " + t1.GetType() + " coins")) {
                                p1.SetInventory("sword");
                                p1.RemoveInventory(to_string(t1.GetQuantity()) + " " + t1.GetType() + " coins");
                                statusMessage = "You traded your gold for a sword!";
                            } else {
                                statusMessage = "You don't have any gold to trade!";
                            }
                            statusMessageTimer = 5.0f;
                            
                            numMoves++;
                            currentRoom++;
                            dungeon.PlayerPathAdd(currentRoom);
                        }
                    ));
                    
                    buttons.push_back(Button(
                        windowWidth / 2 + 40, windowHeight / 2 + 50,
                        140, 50, &font, "Decline",
                        sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                        [&numMoves, &currentRoom, &statusMessage, &statusMessageTimer, &dungeon]() {
                            statusMessage = "You declined the leprechaun's offer. He vanishes.";
                            statusMessageTimer = 5.0f;
                            
                            numMoves++;
                            currentRoom++;
                            dungeon.PlayerPathAdd(currentRoom);
                        }
                    ));
                } else {
                    // Still waiting for leprechaun to appear
                    buttons.push_back(Button(
                        windowWidth / 2 - 200, windowHeight / 2,
                        500, 50, &font, "You wait in the darkness...",
                        sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
                        []() { }
                    ));
                    
                    buttons.push_back(Button(
                        windowWidth / 2 - 150, windowHeight / 2 + 50,
                        300, 50, &font, "Continue waiting...",
                        sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                        [&statusMessage, &statusMessageTimer, &previousRoom]() {
                            statusMessage = "Still waiting...";
                            statusMessageTimer = 5.0f;
                            previousRoom = 0;
                        }
                    ));
                }
            }
            break;

            case 4: // Combat room with enemies
                buttons.push_back(Button(
                    windowWidth / 2 - 200, windowHeight / 2 - 80,
                    400, 40, &font, "TWO ALIENS APPEAR OUT OF NOWHERE",
                    sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
                    []() { }
                ));
                
                buttons.push_back(Button(
                    windowWidth / 2 - 100, windowHeight / 2,
                    200, 50, &font, "Fight!",
                    sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                    [&p1, &numMoves, &currentRoom, &statusMessage, &statusMessageTimer, &dungeon, &t1, &playingState, &font, &windowWidth, &windowHeight, &window]() {

                    auto displayCombatResults = [&window, &font, windowWidth, windowHeight](const std::string& results) {
                        sf::Text combatText;
                        combatText.setFont(font);
                        combatText.setString(results);
                        combatText.setCharacterSize(28);
                        combatText.setFillColor(sf::Color::Red);
                    
                        // Center the text
                        sf::FloatRect textRect = combatText.getLocalBounds();
                        combatText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
                        combatText.setPosition(windowWidth / 2.0f, windowHeight / 2.0f - 50);
                    
                    // Draw a background for better visibility
                        sf::RectangleShape background;
                        background.setSize(sf::Vector2f(textRect.width + 40, textRect.height + 40));
                        background.setFillColor(sf::Color(0, 0, 0, 200));
                        background.setOrigin(background.getSize().x / 2.0f, background.getSize().y / 2.0f);
                        background.setPosition(windowWidth / 2.0f, windowHeight / 2.0f - 50);
                    
                    window.draw(background);
                    window.draw(combatText);
                    window.display();
                    
                    // Pause briefly to show combat results
                    sf::sleep(sf::seconds(3.0f));
                };
                        priority_queue<CombatEvent> combatQueue;
                        
                        if (p1.CheckInventory("sword")) {
                            combatQueue.push({"Goblin", "Player", "attack", 10, 10});
                            combatQueue.push({"Player", "All Enemies", "sword_attack", 100, 5});
                        } else {
                            combatQueue.push({"Creeper", "Player", "explosion", 20, 15});
                            combatQueue.push({"Player", "All Enemies", "hand_attack", 50, 3});
                        }
                        
                        string combatResults = "";
                        while (!combatQueue.empty()) {
                            CombatEvent event = combatQueue.top();
                            combatQueue.pop();
                            
                            if (event.source == "Goblin" && event.action == "attack") {
                                p1.SetHealth(p1.getHealth() - event.value);
                                combatResults += "The goblin bites you on the leg!\n";
                            }
                            else if (event.source == "Creeper" && event.action == "explosion") {
                                p1.SetHealth(p1.getHealth() - event.value);
                                combatResults += "The creeper explodes on you!\n";
                            }
                            else if (event.source == "Player" && event.action == "sword_attack") {
                                combatResults += "One slash of the sword annihilates the creeper and the goblin!\n";
                            }
                            else if (event.source == "Player" && event.action == "hand_attack") {
                                combatResults += "You crush the goblin and the creeper with your bare hands!\n";
                            }
                        }
                        
                        // After combat, check if player has gold for healing potion
                        if (p1.CheckInventory(to_string(t1.GetQuantity()) + " " + t1.GetType() + " coins")) {
                            // Queue healing option
                            combatQueue.push({"Merchant", "Player", "offer_potion", 20, 1});
                            combatResults += "\nA merchant appears and offers a healing potion!";
                        }

                        displayCombatResults(combatResults);


                        // // Make combat results more prominent
                        // sf::Text combatText;
                        // combatText.setFont(font);
                        // combatText.setString(combatResults);
                        // combatText.setCharacterSize(28);  // Larger text
                        // combatText.setFillColor(sf::Color::Red);  // More visible color
                        // sf::FloatRect textRect = combatText.getLocalBounds();
                        // combatText.setOrigin(textRect.width / 2, 0);
                        // combatText.setPosition(windowWidth / 2, windowHeight / 2 - 100);
                        
                        statusMessage = combatResults;
                        statusMessageTimer = 5.0f; 
                        numMoves++;
                        currentRoom++;
                        playingState = ROOM_NAVIGATION;
                    }
                ));
            break;
            
            case 5: 
                buttons.push_back(Button(
                    windowWidth / 2 - 100, windowHeight / 2,
                    200, 50, &font, "Confront Boss",
                    sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                    [&playingState, &p1, &statusMessage, &statusMessageTimer]() {
                        playingState = BOSS_FIGHT;
                        statusMessage = "You approach the final boss...";
                        statusMessageTimer = 2.0f;
                    }
                ));

                if (p1.CheckInventory(to_string(t1.GetQuantity()) + " " + t1.GetType() + " coins")) {
                    buttons.push_back(Button(
                        windowWidth / 2 - 150, windowHeight / 2 + 100,
                        300, 50, &font, "Buy Healing Potion (5 Gold)",
                        sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                        [&p1, &statusMessage, &statusMessageTimer, &t1]() {
                            // Only allow purchase if health isn't full
                            if (p1.getHealth() < 100) {
                                p1.RemoveInventory(to_string(t1.GetQuantity()) + " " + t1.GetType() + " coins");
                                
                                // Calculate how much to heal (don't exceed 100)
                                int healAmount = min(20, 100 - p1.getHealth());
                                p1.SetHealth(p1.getHealth() + healAmount);
                                
                                statusMessage = "You used your gold to buy a healing potion! +" + to_string(healAmount) + " Health";
                            } else {
                                statusMessage = "Your health is already full!";
                            }
                            statusMessageTimer = 5.0f;
                        }
                    ));
                }
            break;
    }

}

int main() {


    // ------------------------------------------------------------------------- Initialization -------------------------------------------------------------------------
    // for window creation
    sf::RenderWindow window;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    //creates a fullscreen window if possible, otherwise creates a window with default size
    bool fullscreenSuccess = false;
    bool fullscreen = true;
    if (fullscreen) {
        window.create(desktop, "Dungeon Game", sf::Style::Fullscreen);
        fullscreenSuccess = window.isOpen();
    }

    if (!fullscreenSuccess) {
        window.create(sf::VideoMode(800, 600), "Dungeon Game");
    }

    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;

    //font loading
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cout << "Error loading font" << std::endl;
        return -1;
    }

    Player p1;
    p1.SetHealth(100);
    string name;
    string startPlay;
    PlayingState playingState = INTRO;
    int currentRoom = 1;
    int numMoves = 1;
    int previousRoom = 0;
    bool level1Done = false;
    // ------------------------- Text Input Setup -------------------------
    string currentInput = "";
    bool isAcceptingInput = true;
    sf::Text inputText;
    sf::RectangleShape inputBox;
    sf::Text promptText;
    bool torchTaken = false;

    // Set up input text display
    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(windowWidth / 2 - 200, windowHeight / 2 + 50);

    // Create input box
    inputBox.setSize(sf::Vector2f(400, 40));
    inputBox.setFillColor(sf::Color(50, 50, 50, 200));
    inputBox.setOutlineColor(sf::Color::White);
    inputBox.setOutlineThickness(2);
    inputBox.setPosition(windowWidth / 2 - 200, windowHeight / 2 + 50);

    // Set up prompt text
    promptText.setFont(font);
    promptText.setString("Enter your name:");
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(windowWidth / 2 - 200, windowHeight / 2 + 10);

    vector<Button> roomButtons;
    Button* nextRoomButton = nullptr;
    Button* checkStatsButton = nullptr;
    Button* backRoomButton = nullptr;
    string statusMessage = "";
    float statusMessageTimer = 0.0f;
    sf::Clock statusClock;


    // the first room
    Dungeon dungeon;
    Treasure t1;
    t1.SetType("Gold");
    t1.SetQuantity(5);
    dungeon.newRoom("You are in a very-well lit room with a lot of treasure.", {"torch"}, t1);

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

    //------------------------------------------------------------------------- Window Generation -------------------------------------------------------------------------
    // Sets the initial game state
    GameState currentState = MENU;
    ScoreLeaderboard scoreBoard("leaderboard_score.csv");
    EfficiencyLeaderboard efficiencyBoard("leaderboard_efficiency.csv");
    // Create buttons for the menu
    Button startButton(
        windowWidth / 2 - 100,
        windowHeight * 0.4f,
        200, 50, &font, "Start Game",
        sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
        [&currentState]() { 
            currentState = PLAYING;
        }
    );

    Button leaderboardButton(
    windowWidth / 2 - 100,
    windowHeight * 0.5f,
    200, 50, &font, "Leaderboard",
    sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
    [&currentState]() { 
        currentState = LEADERBOARD;
        }
    );
    
    Button exitButton(
        windowWidth / 2 - 100,
        windowHeight * 0.6f,
        200, 50, &font, "Exit Game",
        sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
        [&window]() { 
            window.close();
        }
    );

    // Title text
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Dungeon Escape");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(windowWidth / 2 - 150, windowHeight * 0.2f);

    // Main Game loop
    while (window.isOpen()) {
    sf::Event event;
    sf::Vector2f mousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // Handle events
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (currentState == MENU) {
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f clickPosView = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                startButton.handleEvent(event, clickPosView);
                leaderboardButton.handleEvent(event, clickPosView);
                exitButton.handleEvent(event, clickPosView);
            }
        }
        
        if (currentState == PLAYING && isAcceptingInput) {
            if (event.type == sf::Event::TextEntered) {
                // Handle backspace
                if (event.text.unicode == 8 && !currentInput.empty()) {
                    currentInput.pop_back();
                }
                // Handle enter key to submit input
                else if (event.text.unicode == 13) {
                    // Process the submitted input
                    isAcceptingInput = false;
                    // Store input for game logic
                    startPlay = currentInput;
                    currentInput = "";
                }
                // Regular character input
                else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                    if (currentInput.length() < 20) { 
                        currentInput += static_cast<char>(event.text.unicode);
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                if (currentState == PLAYING) {
                    currentState = MENU;
                }
            }
        }

        if (currentState == PLAYING && playingState == DECISION_MAKING) {
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f clickPosView = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                
                // Create the button here just for event handling
                Button tempBackButton(
                    windowWidth / 2 - 100, 
                    windowHeight - 100,
                    200, 50, &font, "Back to Game",
                    sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                    [&playingState]() {
                        playingState = ROOM_NAVIGATION;
                    }
                );
                tempBackButton.handleEvent(event, clickPosView);
            }
        }

        if (currentState == PLAYING && playingState == ROOM_NAVIGATION) {
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f clickPosView = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                
                // Handle room buttons
                for (auto& button : roomButtons) {
                    button.handleEvent(event, clickPosView);
                }
                
                // Handle navigation buttons if they exist
                if (nextRoomButton) nextRoomButton->handleEvent(event, clickPosView);
                if (checkStatsButton) checkStatsButton->handleEvent(event, clickPosView);
                if (backRoomButton) backRoomButton->handleEvent(event, clickPosView);
            }
        }
    } // End of pollEvent loop

    // Update UI elements
    if (currentState == MENU) {
        startButton.update(mousePosView);
        exitButton.update(mousePosView);
    }
    

    // Clear the window and draw UI elements
    window.clear(sf::Color(30, 30, 30));

    if (currentState == MENU) {
        window.draw(titleText);
        startButton.render(window);
        leaderboardButton.render(window); 
        leaderboardButton.update(mousePosView);
        exitButton.render(window);

    } else if (currentState == PLAYING) {
        // Intialize on first entry to PLAYING state
        static bool gameInitialized = false;


        if (!gameInitialized) {
            // Get player name
            isAcceptingInput = true;
            promptText.setString("Enter your name:");
            gameInitialized = true;
            playingState = INTRO;
        }

         // Handle different playing substates
        switch (playingState) {
            case INTRO:
                // Show introduction and get player name
                if (isAcceptingInput) {
                    // Display intro text and name input
                    sf::Text introText;
                    introText.setFont(font);
                    introText.setString("Welcome to Escape the Dungeon!\nYou fell down a trap door and landed in a dungeon.");
                    introText.setCharacterSize(24);
                    introText.setFillColor(sf::Color::White);
                    introText.setPosition(50, 50);
                    window.draw(introText);
                    
                    window.draw(inputBox);
                    window.draw(promptText);
                    inputText.setString(currentInput + "_");
                    window.draw(inputText);
                } else {
                    // If name submitted, show start message
                    name = startPlay;
                    p1.SetName(name);
                    
                    sf::Text welcomeText;
                    welcomeText.setFont(font);
                    welcomeText.setString("Welcome, " + name + "!\nPress SPACE to begin your adventure.");
                    welcomeText.setCharacterSize(24);
                    welcomeText.setFillColor(sf::Color::White);
                    welcomeText.setPosition(50, 50);
                    window.draw(welcomeText);
                    
                    // Check for space key to start game
                    static sf::Clock introClock;
                    if (introClock.getElapsedTime().asSeconds() > 3.0f) {
                        playingState = ROOM_NAVIGATION;
                        currentRoom = 1;
                        dungeon.PlayerPathAdd(currentRoom);
                        introClock.restart();
                    }
                }
                break;
                
            case ROOM_NAVIGATION:{
            // Add near the beginning of the ROOM_NAVIGATION case
                if (numMoves >= 7) {
                    currentState = GAME_OVER;
                    statusMessage = "You ran out of moves!";
                    statusMessageTimer = 5.0f;
                    break;
                }
                
                // Display current room info
                displayRoomInfo(window, font, dungeon, currentRoom, numMoves, p1, windowWidth);
                
                // Create room-specific buttons if they don't exist
                if (roomButtons.empty() || previousRoom != currentRoom) {
                    roomButtons.clear();
                    createRoomButtons(currentRoom, roomButtons, font, windowWidth, windowHeight, p1, level1Done, playingState, t1, numMoves, currentRoom, statusMessage, statusMessageTimer, torchTaken, previousRoom, dungeon, window);
                    previousRoom = currentRoom;
                }
                
                // Draw room buttons
                for (auto& button : roomButtons) {
                    button.update(mousePosView);
                    button.render(window);
                }
            if (currentRoom <= 2) {   
                if (nextRoomButton == nullptr) {
                    nextRoomButton = new Button(
                        windowWidth / 2 - 300, 
                        windowHeight - 80,
                        200, 50, &font, "Next Room",
                        sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                        [&numMoves, &currentRoom, &dungeon, &playingState]() {
                            numMoves++;
                            currentRoom++;
                            dungeon.PlayerPathAdd(currentRoom);
                            playingState = ROOM_NAVIGATION;
                        }
                    );
                }

                if (checkStatsButton == nullptr) {
                    checkStatsButton = new Button(
                        windowWidth / 2 - 100, 
                        windowHeight - 80,
                        200, 50, &font, "Check Stats",
                        sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                        [&playingState]() {
                            playingState = DECISION_MAKING;
                        }
                    );
                }

                if (backRoomButton == nullptr) {
                    backRoomButton = new Button(
                        windowWidth / 2 + 100, 
                        windowHeight - 80,
                        200, 50, &font, "Back Room",
                        sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                        [&numMoves, &currentRoom, &dungeon, &playingState]() {
                            if (currentRoom > 1) {
                                numMoves++;
                                dungeon.PlayerPathBack();
                                currentRoom--;
                                playingState = ROOM_NAVIGATION;
                            }
                        }
                    );
                }

                // Update and render navigation buttons
                nextRoomButton->update(mousePosView);
                if (currentRoom > 1) backRoomButton->update(mousePosView);
                nextRoomButton->render(window);
                if (currentRoom > 1) backRoomButton->render(window);
            }

            if (checkStatsButton == nullptr) {
                checkStatsButton = new Button(
                    windowWidth / 2 - 100, 
                    windowHeight - 80,
                    200, 50, &font, "Check Stats",
                    sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                    [&playingState]() {
                        playingState = DECISION_MAKING;
                    }
                );
            }

            checkStatsButton->update(mousePosView);
            checkStatsButton->render(window);
                break;}
                
            case DECISION_MAKING:{
            // Display player stats
                sf::Text statsTitle;
                statsTitle.setFont(font);
                statsTitle.setString("Player Stats");
                statsTitle.setCharacterSize(32);
                statsTitle.setFillColor(sf::Color::White);
                statsTitle.setPosition(windowWidth / 2 - 100, 50);
                window.draw(statsTitle);
                
                sf::Text playerName;
                playerName.setFont(font);
                playerName.setString("Name: " + p1.getName());
                playerName.setCharacterSize(24);
                playerName.setFillColor(sf::Color::White);
                playerName.setPosition(windowWidth / 2 - 150, 120);
                window.draw(playerName);
                
                sf::Text playerHealth;
                playerHealth.setFont(font);
                playerHealth.setString("Health: " + to_string(p1.getHealth()));
                playerHealth.setCharacterSize(24);
                playerHealth.setFillColor(sf::Color::Green);
                playerHealth.setPosition(windowWidth / 2 - 150, 160);
                window.draw(playerHealth);
                
                // Display inventory
                sf::Text inventoryTitle;
                inventoryTitle.setFont(font);
                inventoryTitle.setString("Inventory:");
                inventoryTitle.setCharacterSize(24);
                inventoryTitle.setFillColor(sf::Color::White);
                inventoryTitle.setPosition(windowWidth / 2 - 150, 200);
                window.draw(inventoryTitle);
                
                // Get player's inventory
                vector<string> inventory = p1.GetInventoryVector();
                for (int i = 0; i < inventory.size(); i++) {
                    sf::Text item;
                    item.setFont(font);
                    item.setString("- " + inventory[i]);
                    item.setCharacterSize(20);
                    item.setFillColor(sf::Color::White);
                    item.setPosition(windowWidth / 2 - 130, 240 + (i * 30));
                    window.draw(item);
                }
                
                // Back button
                Button backButton(
                    windowWidth / 2 - 100, 
                    windowHeight - 100,
                    200, 50, &font, "Back to Game",
                    sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                    [&playingState]() {
                        playingState = ROOM_NAVIGATION;
                    }
                );
                
                backButton.update(mousePosView);
                backButton.render(window);
                
                // Handle back button events
                // if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                //     sf::Vector2f clickPosView = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                //     backButton.handleEvent(event, clickPosView);
                // }
                    break;}
                
            case COMBAT:{
                // Enemy combat logic
                break;
            }
            case BOSS_FIGHT: {
                static sf::Clock bossReactionClock;
                static bool bossPreparationStarted = false;
                static bool bossPromptShown = false;
                static bool bossResultDetermined = false;
                
                if (!bossPreparationStarted) {
                    bossPreparationStarted = true;
                    bossPromptShown = false;
                    bossResultDetermined = false;
                    bossReactionClock.restart();
                    
                    if (!p1.CheckInventory("sword")) {
                        statusMessage = "You have no weapon! The boss crushes you.";
                        statusMessageTimer = 5.0f;
                        p1.SetHealth(0);
                        currentState = GAME_OVER;
                        break;
                    }
                }
                
                sf::Text bossTitle;
                bossTitle.setFont(font);
                bossTitle.setString("The Final Boss appears!");
                bossTitle.setCharacterSize(72);  
                bossTitle.setFillColor(sf::Color::Red);
                
                sf::FloatRect titleRect = bossTitle.getLocalBounds();
                bossTitle.setOrigin(titleRect.width / 2, 0);
                bossTitle.setPosition(windowWidth/2, 50);
                window.draw(bossTitle);
                
                sf::Text prepareText;
                prepareText.setFont(font);
                prepareText.setString("Prepare yourself...");
                prepareText.setCharacterSize(64);  
                prepareText.setFillColor(sf::Color::Red);
                
                // Center and position below the title
                sf::FloatRect prepRect = prepareText.getLocalBounds();
                prepareText.setOrigin(prepRect.width / 2, 0);
                prepareText.setPosition(windowWidth/2, 140);  
                window.draw(prepareText);
                
                sf::Text instructionText;
                instructionText.setFont(font);
                instructionText.setString("You have to slash your sword when prompted!");
                instructionText.setCharacterSize(32); 
                instructionText.setFillColor(sf::Color::White);
                
                sf::FloatRect instRect = instructionText.getLocalBounds();
                instructionText.setOrigin(instRect.width / 2, instRect.height / 2);
                instructionText.setPosition(windowWidth/2, windowHeight/2);  
                window.draw(instructionText);
                
                if (!bossPromptShown) {
                    if (bossReactionClock.getElapsedTime().asSeconds() > 3.0f) {
                        bossPromptShown = true;
                        bossReactionClock.restart(); 
                    }
                } 
                else if (!bossResultDetermined) {
                    sf::Text attackText;
                    attackText.setFont(font);
                    attackText.setString("NOW! Press F to slash your sword!");
                    attackText.setCharacterSize(36);
                    attackText.setFillColor(sf::Color::Red);
                    attackText.setPosition(windowWidth/2 - 250, windowHeight/2 + 50);
                    window.draw(attackText);
                    
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                        float reactionTime = bossReactionClock.getElapsedTime().asSeconds();
                        bossResultDetermined = true;
                        
                        if (reactionTime < 3.0f) {
                            statusMessage = "You slash the boss just in time and win the fight!";
                            statusMessageTimer = 5.0f;
                            playingState = GAME_END;
                        } else {
                            statusMessage = "You hesitated... The boss crushes you!";
                            statusMessageTimer = 5.0f;
                            p1.SetHealth(0);
                            currentState = GAME_OVER;
                        }
                    }
                    
                    if (bossReactionClock.getElapsedTime().asSeconds() > 5.0f) {
                        bossResultDetermined = true;
                        statusMessage = "You failed to react in time! The boss crushes you!";
                        statusMessageTimer = 5.0f;
                        p1.SetHealth(0);
                        currentState = GAME_OVER;
                    }
                }
                break;
            }
            case GAME_END:{
            // Display victory message
                sf::Text endText;
                endText.setFont(font);
                endText.setString("VICTORY!");
                endText.setCharacterSize(64);
                endText.setFillColor(sf::Color::Green);
                endText.setPosition(windowWidth/2 - 150, windowHeight/2 - 100);
                window.draw(endText);
                
                sf::Text resultText;
                resultText.setFont(font);
                resultText.setString("You defeated the final boss and escaped the dungeon!");
                resultText.setCharacterSize(24);
                resultText.setFillColor(sf::Color::White);
                resultText.setPosition(windowWidth/2 - 250, windowHeight/2);
                window.draw(resultText);
                
                // Show player stats
                sf::Text statsText;
                statsText.setFont(font);
                statsText.setString("Final Stats\nHealth: " + std::to_string(p1.getHealth()) + 
                                    "\nMoves used: " + std::to_string(numMoves));
                statsText.setCharacterSize(20);
                statsText.setFillColor(sf::Color::Yellow);
                statsText.setPosition(windowWidth/2 - 100, windowHeight/2 + 100);
                window.draw(statsText);
                
                // Auto-close after a few seconds
                static sf::Clock endClock;
                if (endClock.getElapsedTime().asSeconds() > 8.0f) {
                    window.close();
                }
                endClock.restart();   
                
                int finalScore = p1.getHealth() * 10 + (7 - numMoves) * 20;  // Calculate score
                bool completed = true;
                PlayerRecord newRecord(name, finalScore, p1.getHealth(), numMoves, completed);
                scoreBoard.addRecord(newRecord);
                efficiencyBoard.addRecord(newRecord);
                scoreBoard.saveToFile();
                efficiencyBoard.saveToFile();
            break;
            }
        }


        
        sf::Text playingText;
        playingText.setFont(font);
        playingText.setCharacterSize(36);
        playingText.setFillColor(sf::Color::White);
        playingText.setPosition(windowWidth / 2 - 100, windowHeight / 2 - 18);
        window.draw(playingText);

        // If accepting input, show input components
        if (isAcceptingInput) {
            window.draw(inputBox);
            window.draw(promptText);
            
            // Update the input text display with current input
            inputText.setString(currentInput + "_"); // Add cursor
            window.draw(inputText);
        }
    }

    else if (currentState == GAME_OVER) {
        // Use a static timer for the GAME_OVER state
        static sf::Clock gameOverClock;
        static bool gameOverClockStarted = false;
        static bool recordedGameOver = false;
        
        if (!gameOverClockStarted) {
            gameOverClock.restart();
            gameOverClockStarted = true;
        }
    
    if (!recordedGameOver) {
        int finalScore = p1.getHealth() * 5;  // Lower score for incomplete games
        bool completed = false;
        PlayerRecord newRecord(name, finalScore, p1.getHealth(), numMoves, completed);
        
        scoreBoard.addRecord(newRecord);
        efficiencyBoard.addRecord(newRecord);
        scoreBoard.saveToFile();
        efficiencyBoard.saveToFile();
        recordedGameOver = true;
    }
        
        // Draw Game Over text
        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(64);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(windowWidth/2 - 200, windowHeight/2 - 100);
        window.draw(gameOverText);
        
        // Draw result message
        sf::Text resultText;
        resultText.setFont(font);
        if (p1.getHealth() <= 0) {
            resultText.setString("You were defeated!");
        } else {
            resultText.setString("You ran out of moves!");
        }
        resultText.setCharacterSize(36);
        resultText.setFillColor(sf::Color::White);
        resultText.setPosition(windowWidth/2 - 150, windowHeight/2);
        window.draw(resultText);
        
        // Close window after 5 seconds
        if (gameOverClock.getElapsedTime().asSeconds() > 5.0f) {
            window.close();
        }
    }

    else if (currentState == LEADERBOARD) {
        window.clear(sf::Color(30, 30, 30));
        
        sf::Text leaderboardTitle;
        leaderboardTitle.setFont(font);
        leaderboardTitle.setString("LEADERBOARDS");
        leaderboardTitle.setCharacterSize(40);
        leaderboardTitle.setFillColor(sf::Color::White);
        leaderboardTitle.setPosition(windowWidth / 2 - 150, 50);
        window.draw(leaderboardTitle);
        scoreBoard.loadFromFile();
        
        sf::Text scoreTitle;
        scoreTitle.setFont(font);
        scoreTitle.setString("TOP SCORES");
        scoreTitle.setCharacterSize(28);
        scoreTitle.setFillColor(sf::Color::Yellow);
        scoreTitle.setPosition(windowWidth / 2 - 80, 120);
        window.draw(scoreTitle);
        
        sf::Text headerText;
        headerText.setFont(font);
        headerText.setString("Rank | Name             | Score | Health | Moves | Status");
        headerText.setCharacterSize(18);
        headerText.setFillColor(sf::Color::Green);
        headerText.setPosition(windowWidth / 2 - 250, 160);
        window.draw(headerText);
        
        int maxEntries = 7;
        int y = 190;
        int rank = 1;
        
        const std::vector<PlayerRecord>& records = scoreBoard.getRecords();
        
        for (const auto& record : records) {
            if (rank > maxEntries) break;
            
            std::string completionStatus = record.completed ? "Complete" : "Failed";
            
            sf::Text recordText;
            recordText.setFont(font);
            
            std::string displayText = 
                std::to_string(rank) + "    | " + 
                (record.name.length() > 16 ? record.name.substr(0, 16) : record.name + std::string(16 - record.name.length(), ' ')) + " | " +
                std::to_string(record.score) + "   | " +
                std::to_string(record.health) + "    | " +
                std::to_string(record.movesUsed) + "    | " +
                completionStatus;
            
            recordText.setString(displayText);
            recordText.setCharacterSize(16);
            recordText.setFillColor(sf::Color::White);
            recordText.setPosition(windowWidth / 2 - 250, y);
            window.draw(recordText);
            
            y += 30;
            rank++;
        }
        
        Button backButton(
            windowWidth / 2 - 100,
            windowHeight - 100,
            200, 50, &font, "Back to Menu",
            sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
            [&currentState]() { 
                currentState = MENU;
            }
        );
        
        backButton.update(mousePosView);
        backButton.render(window);
        
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f clickPosView = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            backButton.handleEvent(event, clickPosView);
        }
    }

    if (!statusMessage.empty() && statusMessageTimer > 0.0f) {
        sf::Text feedbackText;
        feedbackText.setFont(font);
        feedbackText.setString(statusMessage);
        feedbackText.setCharacterSize(24);
        feedbackText.setFillColor(sf::Color::Yellow);
        
        // Make more prominent by centering horizontally and adding background
        sf::FloatRect textRect = feedbackText.getLocalBounds();
        feedbackText.setOrigin(textRect.width / 2, 0);
        feedbackText.setPosition(windowWidth / 2, windowHeight - 150);
        
        // Add background for better visibility
        sf::RectangleShape msgBackground;
        msgBackground.setSize(sf::Vector2f(textRect.width + 40, textRect.height + 20));
        msgBackground.setFillColor(sf::Color(0, 0, 0, 180));
        msgBackground.setOrigin(msgBackground.getSize().x / 2, 0);
        msgBackground.setPosition(windowWidth / 2, windowHeight - 155);
        
        window.draw(msgBackground);
        window.draw(feedbackText);
        
        // Use a fixed time step for more consistent timing
        static sf::Clock frameTimer;
        float elapsed = frameTimer.restart().asSeconds();
        
        // Cap the elapsed time to prevent large jumps
        if (elapsed > 0.1f) elapsed = 0.1f;
        
        statusMessageTimer -= elapsed;
        if (statusMessageTimer <= 0.0f) {
            statusMessage = "";
        }
    }
    window.display();
} // End of main game loop
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    delete nextRoomButton;
    delete checkStatsButton;
    delete backRoomButton;
    return 0;
} // End of main function
