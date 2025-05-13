#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arkanoid");
    Paddle paddle(350.0f, 550.0f);
    Ball ball(400.0f, 300.0f, 10.0f);
    sf::Clock clock;

    std::vector<Block> blocks;
    float blockWidth = 60.0f;
    float blockHeight = 20.0f;
    float blockSpacing = 5.0f; // между блоками

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 10; ++col) {
            float x = col * (blockWidth + blockSpacing) + 10.0f;
            float y = row * (blockHeight + blockSpacing) + 50.0f;
            blocks.emplace_back(x, y, blockWidth, blockHeight);
        }
    }

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

        for (auto& block : blocks) {
            if (!block.isDestroyed() && ball.getBounds().intersects(block.getBounds())) {
                block.hit();
                ball.bounceVertical();
                break; // чтобы мяч не пробивал сразу несколько блоков
            }
        }

        // падение мяча вниз (игрок проиграл)
        if (ball.getBounds().top > window.getSize().y) {
            ball = Ball(400.0f, 300.0f, 10.0f); // возвращается в центр
        }

        // Отрисовка
        window.clear();
        paddle.draw(window);
        ball.draw(window);

        for (auto& block : blocks) {
            block.draw(window);
        }

        window.display();
    }

    return 0;
}
