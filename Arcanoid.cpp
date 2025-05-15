#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <random>
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include "Bonus.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arkanoid");
    Paddle paddle(350.0f, 550.0f);
    Ball ball(400.0f, 300.0f, 10.0f);
    sf::Clock clock;

    // Генератор случайных чисел - ОБЪЕДИНИЛ вызов, чтобы не создавать дважды
    std::random_device rd;
    std::mt19937 gen(rd());

    // Для случайного выбора типа блока
    std::uniform_int_distribution<> distType(0, 3);
    // Для случайного выбора типа бонуса
    std::uniform_int_distribution<> distBonusType(0, 5);

    std::vector<Block> blocks;
    float blockWidth = 70.0f, blockHeight = 20.0f, blockSpacing = 5.0f;

    int bonusBlocks = 6, violetBlocks = 5, cyanBlocks = 3;
    int totalBlocks = 40;

    int row = 0, col = 0;
    for (int i = 0; i < totalBlocks; ++i) {
        float x = col * (blockWidth + blockSpacing) + 10.0f;
        float y = row * (blockHeight + blockSpacing) + 50.0f;

        int blockType = distType(gen);

        sf::Color blockColor;
        int blockHealth = 1;

        switch (blockType) {
        case 0:  // Фиолетовый
            if (violetBlocks > 0) {
                blockColor = sf::Color(128, 0, 128);
                blockHealth = 9999;
                --violetBlocks;
            }
            else {
                blockColor = sf::Color::Green;
                blockHealth = 1;
            }
            break;
        case 1:  // Жёлтый (с бонусом)
            if (bonusBlocks > 0) {
                blockColor = sf::Color::Yellow;
                blockHealth = 1;
                --bonusBlocks;
            }
            else {
                blockColor = sf::Color::Green;
                blockHealth = 1;
            }
            break;
        case 2:  // Голубой (ускоряющий)
            if (cyanBlocks > 0) {
                blockColor = sf::Color::Cyan;
                blockHealth = 1;
                --cyanBlocks;
            }
            else {
                blockColor = sf::Color::Green;
                blockHealth = 1;
            }
            break;
        case 3:  // Зелёный (обычный)
            blockColor = sf::Color::Green;
            blockHealth = 1 + (i % 3);
            break;
        }

        blocks.emplace_back(x, y, blockWidth, blockHeight, blockHealth, blockColor);

        col++;
        if (col >= 10) {
            col = 0;
            row++;
        }
    }

    std::vector<Bonus> bonuses;
    int score = 0, defeats = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) paddle.moveLeft(deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) paddle.moveRight(deltaTime);

        paddle.update(window);
        ball.update(deltaTime);
        ball.checkCollision(window);
        ball.checkCollisionWithPaddle(paddle);

        // Столкновения с блоками
        for (auto& block : blocks) {
            if (!block.isDestroyed() && ball.getBounds().intersects(block.getBounds())) {
                ball.bounceVertical();

                if (block.getColor() == sf::Color(128, 0, 128)) {
                    // Фиолетовый блок не разрушается
                }
                else {
                    block.hit();

                    if (block.getColor() == sf::Color::Yellow) {
                        Bonus::Type randomType = static_cast<Bonus::Type>(distBonusType(gen));
                        bonuses.emplace_back(
                            block.getBounds().left + (block.getBounds().width - 30.0f) / 2.0f,
                            block.getBounds().top + block.getBounds().height + 20.0f,  // чуть ниже блока
                            randomType);
                        std::cout << "Bonus created at ("
                            << block.getBounds().left + (block.getBounds().width - 30.0f) / 2.0f
                            << ", "
                            << block.getBounds().top + block.getBounds().height + 20.0f
                            << ")\n";
                    }

                    if (block.getColor() == sf::Color::Cyan) {
                        ball.increaseSpeed();
                    }

                    score += 1;
                    std::cout << "Score: " << score << std::endl;
                }
            }
        }

        // Обновляем бонусы
        for (auto& bonus : bonuses) {
            bonus.update(deltaTime);
        }

        // Обработка столкновений бонусов с кареткой
        for (size_t i = 0; i < bonuses.size(); ++i) {
            if (bonuses[i].getBounds().intersects(paddle.getBounds())) {
                bonuses[i].activate(paddle, ball);
                bonuses.erase(bonuses.begin() + i);
                --i;
            }
        }


        // Удаление бонусов, вышедших за экран или неактивных
        bonuses.erase(std::remove_if(bonuses.begin(), bonuses.end(),
            [&window](const Bonus& b) {
                return b.isOutOfWindow(window) || !b.isActive();
            }),
            bonuses.end());

        // Проверка падения мяча
        if (ball.getBounds().top > window.getSize().y) {
            defeats++;
            score -= 2;
            std::cout << "Defeats: " << defeats << "/3" << std::endl;
            ball = Ball(400.0f, 300.0f, 10.0f);
            float newWidth = paddle.getBounds().width * 0.8f;
            paddle.setSize(newWidth, paddle.getBounds().height);
        }

        if (defeats >= 3) {
            std::cout << "Game Over! Final Score: " << score << std::endl;
            window.close();
        }


        window.clear();
        paddle.draw(window);
        ball.draw(window);
        for (auto& block : blocks) block.draw(window);
        for (auto& bonus : bonuses) bonus.draw(window);
        window.display();
    }

    return 0;
}
