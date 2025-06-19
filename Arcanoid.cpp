#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Paddle.h"
#include "Ball.h"
#include "Bonus.h"
#include "Block.h"

int main() {
    int score = 0;
    int lives = 3;

    // srand - задаёт нач. знач. для генератора случайных чисел (чтобы каждый раз разная последовательность)
    // static_cast<unsigned int> приведение типов (от time_t)
    // std::time(nullptr) - возвращает текущее время
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    //sf::RenderWindow - окно для рисования; размеры();заголовок
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arkanoid");
    window.setFramerateLimit(60); // обращаемся к методу объекта window; setFramerateLimit(60) — метод класса RenderWindow (ограничение фпс)

    Paddle paddle; // создание объекта по умолч.
    Ball ball(400.f, 300.f, 10.f);

    std::vector<std::unique_ptr<Block>> blocks; // <std::unique_ptr<Block>> — умные указатели на объекты Block, которые автоматически управляют памятью
    std::vector<std::unique_ptr<Bonus>> bonuses;

    // создание блоков
    const int rows = 5;
    const int cols = 11;
    const float blockWidth = 70.f;
    const float blockHeight = 20.f;
    const float startX = 0.f;
    const float startY = 0.f;
    const float padding = 2.f; //отступ

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = startX + j * (blockWidth + padding);
            float y = startY + i * (blockHeight + padding);
            blocks.push_back(BlockFactory::createRandomBlock(x, y)); //push_back(...) — добавить в конец вектора blocks
        }                                                           // обращение к методу createRandomBlock(возвр. указательно на блок) из класса BlockFactory
    }

    sf::Clock clock; // игровой таймер ( измерение времени между кадрами)

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds(); //сбрасываем таймер(clock.restart()) и возвращаем время в с (asSeconds())

        //обработка событий
        sf::Event event;
        while (window.pollEvent(event)) {// pollEvent проверяет, есть ли событие в очереди и, если есть, извлекает его в переменную event
            if (event.type == sf::Event::Closed) { //закрытие окна при нажатии крестика
                window.close();
                std::cout << "Game Over! Score: " << score << std::endl;
            }

            // для отлипания мяча от каретки при жёлтом бонусе (пробел)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                ball.releaseSticky();
            }
        }

        // управление кареткой
        paddle.update(window, deltaTime);

        ball.update(deltaTime, paddle.getPosition().x, paddle.getPosition().y, paddle.getBounds().width);
        // проверка столкновений 
        ball.checkCollision(window);
        ball.checkCollisionWithPaddle(
            paddle.getPosition().x,
            paddle.getPosition().y,
            paddle.getBounds().width,
            paddle.getBounds().height,
            paddle.isOneTime()
        );

        // столкновение с блоками
        for (auto& block : blocks) {
            if (!block->isDestroyed() && block->getBounds().intersects(ball.getBounds())) { 
                // -> обращение к методу через указатель(block); intersects(..) — проверка на пересечение прямоугольников
                block->onHit(bonuses, ball);
                ball.bounceVertical();  // предполагаем отражение по вертикали

                if (dynamic_cast<VioletBlock*>(block.get()) == nullptr) { //dynamic_cast — проверка и приведение типа (от block*)
                    score++;
                    std::cout << "Score: " << score << ", Lives: " << lives << std::endl;
                }

                break; // чтобы не обработать несколько блоков за один кадр
            }
        }
        
        // отскок от доп. каретки
        if (paddle.isOneTime() && !paddle.isOneTimeUsed() &&
            paddle.getOneTimeBounds().intersects(ball.getBounds())) {

            ball.bounceVertical();
            paddle.resetOneTime();
        }

        // выход за границу нижнюю
        if (ball.isOutOfBounds(window.getSize().y)) {
            lives--;
            score--;
            std::cout << "Score: " << score << ", Lives: " << lives << std::endl;

            if (lives <= 0) {
                std::cout << "Game Over! Score: " << score << std::endl;
                window.close();
            }
            else {
                ball.reset(400.f, 300.f);           // сбросить мяч в центр
                paddle = Paddle(350.f, 550.f, 100.f, 20.f); // сбросить позицию каретки
            }
        }

        // удаляем уничтоженные блоки
        // remove_if — "сдвигает" все удалённые элементы в конец(возвращ. итератор на начало хвоста); erase(..) — удаляет их из вектора
        blocks.erase(std::remove_if(blocks.begin(), blocks.end(), // 1 и посл эл-т вектора (диапазон всех)
            [](const std::unique_ptr<Block>& b) { return b->isDestroyed(); }),//условие; (если уничтожен - true)
            blocks.end()); //конец  перемещённого хвоста

        // обновляем бонусы
        for (auto& bonus : bonuses) {// по всем элементам контейнера
            if (bonus->isActive()) {
                bonus->update(deltaTime);

                // проверяем столкновение бонуса с кареткой
                if (bonus->getBounds().intersects(paddle.getBounds())) {
                    bonus->apply(paddle, ball);
                }
            }
        }

        // удаляем неактивные бонусы
        bonuses.erase(std::remove_if(bonuses.begin(), bonuses.end(), 
            [](const std::unique_ptr<Bonus>& b) { return !b->isActive(); }), // true - если неактивен
            bonuses.end());

        // очистка экрана (заполнение чёрным)
        window.clear(sf::Color::Black);

        paddle.draw(window);
        ball.draw(window);

        // рисуем блоки и бонусы
        for (const auto& block : blocks)
            block->draw(window);

        for (const auto& bonus : bonuses)
            bonus->draw(window);

        //отображаем 
        window.display();
    }

    return 0;
}
