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
    sf::RenderWindow window;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    bool fullscreenSuccess = false;
    bool fullscreen = true;
    if (fullscreen) {
        window.create(desktop, "Dungeon Game", sf::Style::Fullscreen);
        fullscreenSuccess = window.isOpen();
    }

    if (!fullscreenSuccess) {
        window.create(sf::VideoMode(800, 600), "Dungeon Game");
    }

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cout << "Error loading font" << std::endl;
        return -1;
    }

    GameState currentState = MENU;

    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;

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

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Dungeon Escape");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(windowWidth / 2 - 150, windowHeight * 0.2f);

    while (window.isOpen()) {
        sf::Event event;
        sf::Vector2f mousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window));

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
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                if (currentState == PLAYING) {
                    currentState = MENU;
                }
            }
        }

        if (currentState == MENU) {
            startButton.update(mousePosView);
            exitButton.update(mousePosView);
        }

        window.clear(sf::Color(30, 30, 30));

        if (currentState == MENU) {
            window.draw(titleText);
            startButton.render(window);
            exitButton.render(window);
        } else if (currentState == PLAYING) {
            sf::Text playingText;
            playingText.setFont(font);
            playingText.setString("Game Started!");
            playingText.setCharacterSize(36);
            playingText.setFillColor(sf::Color::White);
            playingText.setPosition(windowWidth / 2 - 100, windowHeight / 2 - 18);
            window.draw(playingText);
        }

        window.display();
    }

    return 0;
}