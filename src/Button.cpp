#include "../headers/Button.h"

Button::Button(float x, float y, float width, float height, 
               sf::Font* font, std::string buttonText, 
               sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor,
               std::function<void()> callback) {
    
    this->font = font;
    this->callback = callback;
    
    this->idleColor = idleColor;
    this->hoverColor = hoverColor;
    this->activeColor = activeColor;
    
    this->isPressed = false;
    this->isHovered = false;
    
    // Configure shape
    this->shape.setPosition(sf::Vector2f(x, y));
    this->shape.setSize(sf::Vector2f(width, height));
    this->shape.setFillColor(this->idleColor);
    
    // Configure text
    this->text.setFont(*this->font);
    this->text.setString(buttonText);
    this->text.setFillColor(sf::Color::White);
    this->text.setCharacterSize(24);
    
    // Center text on button
    this->text.setPosition(
        this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - (this->text.getGlobalBounds().width / 2.f),
        this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - (this->text.getGlobalBounds().height / 2.f)
    );
}

void Button::update(const sf::Vector2f mousePos) {
    // Reset button state
    this->isHovered = false;
    this->isPressed = false;
    
    // Check if mouse is over button
    if (this->shape.getGlobalBounds().contains(mousePos)) {
        this->isHovered = true;
        
        // Update button color based on state
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            this->isPressed = true;
            this->shape.setFillColor(this->activeColor);
        } else {
            this->shape.setFillColor(this->hoverColor);
        }
    } else {
        this->shape.setFillColor(this->idleColor);
    }
}

void Button::render(sf::RenderWindow& window) {
    window.draw(this->shape);
    window.draw(this->text);
}

void Button::handleEvent(sf::Event event, sf::Vector2f mousePos) {
    // Check for mouse button release (click completed)
    if (event.type == sf::Event::MouseButtonReleased && 
        event.mouseButton.button == sf::Mouse::Left &&
        this->shape.getGlobalBounds().contains(mousePos)) {
        
        // Execute callback function
        this->callback();
    }
}