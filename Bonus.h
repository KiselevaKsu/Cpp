#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"

class Bonus {
public:
    enum class Type {
        SIZE_INCREASE,
        SPEED_INCREASE,
        STICKY,
        CHANGE_TRAJECTORY,
        ONE_TIME_PADDLE
    };

    Bonus(float startX, float startY, Type type);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    bool isOutOfWindow(const sf::RenderWindow& window) const;
    bool isActive() const;

    void activate(Paddle& paddle, Ball& ball);

    Type getType() const;

private:
    sf::RectangleShape shape;
    Type bonusType;
    float speed;
    bool active;
+
    float duration;  // секунды
    bool durationActive;
};
