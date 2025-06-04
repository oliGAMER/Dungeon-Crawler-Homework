#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font* font;
    
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;
    
    bool isPressed;
    bool isHovered;
    
    std::function<void()> callback;

public:
    Button(float x, float y, float width, float height, 
           sf::Font* font, std::string text, 
           sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor,
           std::function<void()> callback);
    
    void update(const sf::Vector2f mousePos);
    void render(sf::RenderWindow& window);
    
    // Handle events
    void handleEvent(sf::Event event, sf::Vector2f mousePos);
};