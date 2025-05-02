#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 300), "SFML Window");

    // Intentional memory leak (for Valgrind testing)
    int* leaky_array = new int[10];

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.display();
    }
    delete[] leaky_array;
    return 0; // Memory leak: we never delete[] leaky_array
}

