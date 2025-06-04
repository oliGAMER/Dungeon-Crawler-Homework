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

    window.display();
} // End of main game loop
    //--------------------------------------------------------------------------------------------------------------------------------------------------

    return 0;
} // End of main function
