#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>
#include "Paddle.h"

class Ball {
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed;
    bool sticky;            // Липкий мяч
    bool extraLife;         // Одноразовое дно

public:
    // Конструктор
    Ball(float startX, float startY, float radius);

    // Основные функции
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void checkCollision(const sf::RenderWindow& window);
    void checkCollisionWithPaddle(Paddle& paddle);
    sf::FloatRect getBounds();

    // Управление движением и скоростью
    void bounceHorizontal();
    void bounceVertical();
    void increaseSpeed();
    void changeTrajectory(); // Изменение траектории мяча

    // Управление бонусами
    void setSticky(bool isSticky) { sticky = isSticky; }
    void setExtraLife(bool hasExtraLife) { extraLife = hasExtraLife; }

    bool isSticky() const { return sticky; }
    bool hasExtraLife() const { return extraLife; }

    // Специальные эффекты
    void applyStickyEffect(const Paddle& paddle);  // Применяем липкость мяча
};

#endif
