#pragma once

#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Bonus.h"

class Bonus {
public:
    enum class Type {
        SIZE_INCREASE,   // Увеличение размера каретки
        SPEED_INCREASE,  // Ускорение мяча
        STICKY,          // Прилипание мяча
        CHANGE_TRAJECTORY,  // Изменение траектории
        ONE_TIME_PADDLE  // Одноразовое дно
    };

    Bonus(float startX, float startY, Type type);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    bool isOutOfWindow(const sf::RenderWindow& window) const;
    bool isActive() const { return active; }

    void activate(Paddle& paddle, Ball& ball);

private:
    sf::RectangleShape shape;
    Type bonusType;

    float speed;
    bool active;
};

