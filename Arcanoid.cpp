#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Paddle.h"
#include "Ball.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arkanoid");
    Paddle paddle(350.0f, 550.0f);
    Ball ball(400.0f, 300.0f, 10.0f);
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
        ball.update(deltaTime);
        ball.checkCollision(window);
        ball.checkCollisionWithPaddle(paddle); 

        // падение мяча вниз (игрок проиграл)
        if (ball.getBounds().top > window.getSize().y) {
            ball = Ball(400.0f, 300.0f, 10.0f); // возвращается в центр
        }

        window.clear();
        paddle.draw(window);
        ball.draw(window);
        window.display();
    }

    return 0;
}
