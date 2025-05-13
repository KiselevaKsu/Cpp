#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arkanoid - Этап 1");
    Paddle paddle(350.0f, 550.0f);
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            paddle.moveLeft(deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            paddle.moveRight(deltaTime);
        }

        paddle.update(window);

        window.clear();
        paddle.draw(window);
        window.display();
    }

    return 0;
}
