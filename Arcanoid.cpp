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
    Paddle extraPaddle(350.0f, 500.0f, 100.0f, 20.0f);
    Ball ball(400.0f, 300.0f, 10.0f);
    sf::Clock clock;

    std::random_device rd;
    std::mt19937 gen(rd());

    // для случайного выбора типа блока
    std::uniform_int_distribution<> distType(0, 3);
    // для случайного выбора типа бонуса
    std::uniform_int_distribution<> distBonusType(0, 4);

    std::vector<Block> blocks;
    float blockWidth = 70.0f, blockHeight = 20.0f, blockSpacing = 5.0f;

    int bonusBlocks = 6, violetBlocks = 5, cyanBlocks = 2;
    int totalBlocks = 40;

    int row = 0, col = 0;
    for (int i = 0; i < totalBlocks; ++i) {
        float x = col * (blockWidth + blockSpacing) + 10.0f;
        float y = row * (blockHeight + blockSpacing) + 50.0f;

        int blockType = distType(gen);

        Block::Type blockTypeEnum = Block::Type::Green;
        sf::Color blockColor;
        int blockHealth = 1;

        switch (blockType) {
        case 0:  // Фиолетовый
            if (violetBlocks > 0) {
                blockColor = sf::Color(128, 0, 128);
                blockHealth = 9999;
                blockTypeEnum = Block::Type::Violet;
                --violetBlocks;
            }
            else {
                blockColor = sf::Color::Green;
                blockHealth = 1;
                blockTypeEnum = Block::Type::Green;
            }
            break;
        case 1:  // Жёлтый (с бонусом)
            if (bonusBlocks > 0) {
                blockColor = sf::Color::Yellow;
                blockHealth = 1;
                blockTypeEnum = Block::Type::Bonus;
                --bonusBlocks;
            }
            else {
                blockColor = sf::Color::Green;
                blockHealth = 1;
                blockTypeEnum = Block::Type::Green;
            }
            break;
        case 2:  // Голубой (ускоряющий)
            if (cyanBlocks > 0) {
                blockColor = sf::Color::Cyan;
                blockHealth = 1;
                blockTypeEnum = Block::Type::Cyan;
                --cyanBlocks;
            }
            else {
                blockColor = sf::Color::Green;
                blockHealth = 1;
                blockTypeEnum = Block::Type::Green;
            }
            break;
        case 3:  // Зелёный (обычный)
            blockColor = sf::Color::Green;
            blockHealth = 1 + (i % 3);
            blockTypeEnum = Block::Type::Green;
            break;
        }

        blocks.emplace_back(x, y, blockWidth, blockHeight, blockHealth, blockColor, blockTypeEnum);

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ball.releaseSticky();

        paddle.update(window);
        ball.update(deltaTime, paddle.getBounds().left, paddle.getBounds().top, paddle.getBounds().width);

        if (paddle.isOneTime() && !paddle.isOneTimeUsed()) {
            extraPaddle.update(window);
            ball.checkCollisionWithPaddle(
                extraPaddle.getBounds().left,
                extraPaddle.getBounds().top,
                extraPaddle.getBounds().width,
                extraPaddle.getBounds().height,
                true
            );
        }

        ball.checkCollision(window);
        ball.checkCollisionWithPaddle(paddle.getBounds().left, paddle.getBounds().top,
            paddle.getBounds().width, paddle.getBounds().height, paddle.isOneTime());

        if (paddle.isOneTime() && !paddle.isOneTimeUsed()) {
            if (ball.getBounds().intersects(extraPaddle.getBounds())) {
                ball.bounceVertical();
                paddle.setOneTime(false);  // деактивируем доп. каретку после использования
                paddle.resetOneTime();
            }
        }

        // столкновения с блоками
        for (auto& block : blocks) {
            if (!block.isDestroyed() && ball.getBounds().intersects(block.getBounds())) {
                ball.bounceVertical();

                if (block.getType() == Block::Type::Violet) {
                    // фиолетовый блок не разрушается
                }
                else {
                    block.hit();

                    if (block.getType() == Block::Type::Bonus) {
                        Bonus::Type randomType = static_cast<Bonus::Type>(distBonusType(gen));
                        bonuses.emplace_back(
                            block.getBounds().left + (block.getBounds().width - 30.0f) / 2.0f,
                            block.getBounds().top + block.getBounds().height + 20.0f,  // чуть ниже блока
                            randomType);
                    }

                    if (block.getType() == Block::Type::Cyan) {
                        ball.increaseSpeed();
                    }

                    score += 1;
                    std::cout << "Score: " << score << std::endl;
                }
            }
        }

        for (auto& bonus : bonuses) {
            bonus.update(deltaTime);
        }

        // обработка столкновений бонусов с кареткой
        for (size_t i = 0; i < bonuses.size(); ++i) {
            if (bonuses[i].getBounds().intersects(paddle.getBounds())) {
                if (bonuses[i].getType() == Bonus::Type::ONE_TIME_PADDLE) {
                    paddle.setOneTime(true); // активируем доп. каретку
                }
                else if (bonuses[i].getType() == Bonus::Type::CHANGE_TRAJECTORY) {
                    ball.changeTrajectory(); // меняем траекторию шара
                }
                bonuses[i].activate(paddle, ball);
                bonuses.erase(bonuses.begin() + i);
                --i;
            }
        }


        // удаление бонусов, вышедших за экран или неактивных
        bonuses.erase(std::remove_if(bonuses.begin(), bonuses.end(),
            [&window](const Bonus& b) {
                return b.isOutOfWindow(window) || !b.isActive();
            }),
            bonuses.end());

        // проверка падения мяча
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
        if (paddle.isOneTime() && !paddle.isOneTimeUsed()) {
            extraPaddle.draw(window);
        }
        for (auto& block : blocks) block.draw(window);
        for (auto& bonus : bonuses) bonus.draw(window);
        window.display();
    }

    return 0;
}
