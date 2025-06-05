#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "../headers/DynamicArrays.h"
#include "../headers/Player.h"
#include "../headers/Dungeon.h"
#include "../headers/Treasure.h"
#include "../headers/Stack.h"
#include <SFML/Graphics.hpp>
#include "../headers/Button.h"

using namespace std;

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

enum PlayingState {
    INTRO,
    ROOM_NAVIGATION,
    DECISION_MAKING,
    COMBAT,
    BOSS_FIGHT,
    GAME_END
};

void displayRoomInfo(sf::RenderWindow& window, sf::Font& font, Dungeon& dungeon, int roomIndex, int movesLeft, Player& p1, float windowWidth) {
    // Room title/description
    sf::Text roomDesc;
    roomDesc.setFont(font);
    roomDesc.setString(dungeon.getRoomDescriptionAt(roomIndex));
    roomDesc.setCharacterSize(24);
    roomDesc.setFillColor(sf::Color::White);
    roomDesc.setPosition(50, 50);
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


void createRoomButtons(int roomNumber, vector<Button>& buttons, sf::Font& font, float windowWidth, float windowHeight, Player& p1, bool& level1Done, PlayingState& playingState, Treasure& t1, int& numMoves, int& currentRoom) {
    buttons.clear(); // Clear previous buttons
    
    switch(roomNumber) {
        case 1: // First room - torch or gold
            buttons.push_back(Button(
                windowWidth / 2 - 220, windowHeight / 2 + 50,
                200, 50, &font, "Take Gold",
                sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                [&p1, &level1Done, &playingState, &t1]() { 
                    // Gold selection logic
                    if(!level1Done) {
                        p1.SetInventory(to_string(t1.GetQuantity()) + " " + t1.GetType() + " coins");
                        level1Done = true; // mark gold coins as taken
                    }
                    playingState = ROOM_NAVIGATION;
                }
            ));
            
            buttons.push_back(Button(
                windowWidth / 2 + 20, windowHeight / 2 + 50,
                200, 50, &font, "Take Torch",
                sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                [&]() { 
                    // Torch selection logic
                    p1.SetInventory("torch");
                    playingState = ROOM_NAVIGATION;
                }
            ));
            break;
            
        case 2:
            buttons.push_back(Button(
                windowWidth / 2 - 220, windowHeight / 2 + 50,
                200, 50, &font, "Swim Across",
                sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                [&p1, &numMoves, &playingState, &currentRoom]() { 
                    p1.RemoveInventory("torch");
                    
                    numMoves++;
                    currentRoom++;
                    playingState = ROOM_NAVIGATION;
                }
            ));
            
            buttons.push_back(Button(
                windowWidth / 2 + 20, windowHeight / 2 + 50,
                200, 50, &font, "Jump Over",
                sf::Color(70, 70, 70), sf::Color(150, 150, 150), sf::Color(20, 20, 20),
                [&p1, &numMoves, &playingState, &currentRoom]() { 
                    p1.SetHealth(p1.getHealth() - 10);
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
    static int previousRoom = 0;
    bool level1Done = false;
    // ------------------------- Text Input Setup -------------------------
    string currentInput = "";
    bool isAcceptingInput = true;
    sf::Text inputText;
    sf::RectangleShape inputBox;
    sf::Text promptText;

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

    //------------------------------------------------------------------------- Window Generation -------------------------------------------------------------------------
    // Sets the initial game state
    GameState currentState = MENU;

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
                    currentInput += static_cast<char>(event.text.unicode);
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                if (currentState == PLAYING) {
                    currentState = MENU;
                }
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
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                        playingState = ROOM_NAVIGATION;
                        currentRoom = 1;
                        dungeon.PlayerPathAdd(currentRoom);
                    }
                }
                break;
                
            case ROOM_NAVIGATION:{
                // Display current room info
                displayRoomInfo(window, font, dungeon, currentRoom, numMoves, p1, windowWidth);
                
                // Create room-specific buttons if they don't exist
                if (roomButtons.empty()) {
                    roomButtons.clear();
                    createRoomButtons(currentRoom, roomButtons, font, windowWidth, windowHeight, p1, level1Done, playingState, t1, numMoves, currentRoom);
                    previousRoom = currentRoom;
                }
                
                // Draw room buttons
                for (auto& button : roomButtons) {
                    button.update(mousePosView);
                    button.render(window);
                }
                
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
                checkStatsButton->update(mousePosView);
                backRoomButton->update(mousePosView);
                nextRoomButton->render(window);
                checkStatsButton->render(window);
                backRoomButton->render(window);
                
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
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f clickPosView = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    backButton.handleEvent(event, clickPosView);
                }
                    break;}
                
            case COMBAT:{
                // Enemy combat logic
                break;
            }
            case BOSS_FIGHT:{
                // Final boss fight logic with reaction timer
                static sf::Clock reactionClock;
                static bool bossTimerStarted = false;
                
                if (!bossTimerStarted) {
                    sf::Text prepText;
                    prepText.setFont(font);
                    prepText.setString("The Final Boss appears!\nPrepare yourself...");
                    prepText.setCharacterSize(28);
                    prepText.setFillColor(sf::Color::Red);
                    prepText.setPosition(windowWidth/2 - 150, windowHeight/2 - 50);
                    window.draw(prepText);
                    
                    if (reactionClock.getElapsedTime().asSeconds() > 3.0f) {
                        bossTimerStarted = true;
                        reactionClock.restart();
                    }
                } else {
                    sf::Text attackText;
                    attackText.setFont(font);
                    attackText.setString("NOW! Press F to slash your sword!");
                    attackText.setCharacterSize(36);
                    attackText.setFillColor(sf::Color::Red);
                    attackText.setPosition(windowWidth/2 - 200, windowHeight/2);
                    window.draw(attackText);
                    
                    // Check for F key
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                        float reactionTime = reactionClock.getElapsedTime().asSeconds();
                        if (reactionTime < 3.0f) {
                            // Success
                            playingState = GAME_END;
                            // Victory message
                        } else {
                            // Too slow
                            p1.SetHealth(0);
                            currentState = GAME_OVER;
                        }
                    }
                    
                    // Timeout after 5 seconds
                    if (reactionClock.getElapsedTime().asSeconds() > 5.0f) {
                        p1.SetHealth(0);
                        currentState = GAME_OVER;
                    }
                }
                break;
            }
            case GAME_END:{
                // Display game end result
                sf::Text endText;
                endText.setFont(font);
                if (p1.getHealth() <= 0) {
                    endText.setString("Game Over!\nYou were defeated.");
                    endText.setFillColor(sf::Color::Red);
                } else if (numMoves >= 7) {
                    endText.setString("Game Over!\nYou ran out of moves.");
                    endText.setFillColor(sf::Color::Yellow);
                } else {
                    endText.setString("Congratulations!\nYou escaped the dungeon!");
                    endText.setFillColor(sf::Color::Green);
                }
                endText.setCharacterSize(36);
                endText.setPosition(windowWidth/2 - 200, windowHeight/2 - 50);
                window.draw(endText);
                
                // Return to menu button
                // ...
                
                break;}
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
        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(64);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(windowWidth/2 - 200, windowHeight/2 - 100);
        window.draw(gameOverText);
        
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
        
        // Back to menu button
        // ...
    }

    window.display();
} // End of main game loop
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    delete nextRoomButton;
    delete checkStatsButton;
    delete backRoomButton;
    return 0;
} // End of main function
