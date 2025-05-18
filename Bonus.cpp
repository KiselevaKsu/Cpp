#include "Bonus.h"


Bonus::Bonus(float startX, float startY, Type type)
    : bonusType(type), speed(150.f), active(true), duration(3.0f), durationActive(false) {
    shape.setSize(sf::Vector2f(30.0f, 30.0f));
    shape.setPosition(startX, startY);
    switch(bonusType) {
        case Type::SIZE_INCREASE:
            shape.setFillColor(sf::Color::Green); // Зеленый для увеличения платформы
            break;
        case Type::SPEED_INCREASE:
            shape.setFillColor(sf::Color::Red);   // Красный для ускорения мяча
            break;
        case Type::STICKY:
            shape.setFillColor(sf::Color::Magenta); // Фиолетовый для липкости мяча
            break;
        case Type::CHANGE_TRAJECTORY:
            shape.setFillColor(sf::Color::Cyan); // Голубой для смены траектории
            break;
        case Type::ONE_TIME_PADDLE:
            shape.setFillColor(sf::Color::Yellow); // Желтый для одноразовой платформы
            break;
    }
}

void Bonus::update(float deltaTime) {
    if (!durationActive) {
        shape.move(0.0f, speed * deltaTime);

        if (shape.getPosition().y > 600) {
            active = false;
        }
    }
    else {
        duration -= deltaTime;
        if (duration <= 0.0f) {
            active = false;  // когда время вышло, бонус деактивируется
            durationActive = false;
        }
    }
}

void Bonus::draw(sf::RenderWindow& window) {
    if (active) {
        window.draw(shape);
    }
}

bool Bonus::isOutOfWindow(const sf::RenderWindow& window) const {
    return shape.getPosition().y > window.getSize().y;
}

void Bonus::activate(Paddle& paddle, Ball& ball) {
    if (!durationActive) {
        switch (bonusType) {
        case Type::SIZE_INCREASE:
            paddle.setSize(paddle.getBounds().width * 1.5f, paddle.getBounds().height);
            break;
        case Type::SPEED_INCREASE:
            ball.increaseSpeed();
            break;
        case Type::STICKY:
            ball.setSticky(true);
            break;
        case Type::CHANGE_TRAJECTORY:
            ball.changeTrajectory();
            break;
        case Type::ONE_TIME_PADDLE:
            paddle.setOneTime(true);
            break;
        }

        durationActive = true; // запускаем таймер действия бонуса
    }
    active = false;
}

Bonus::Type Bonus::getType() const {
    return bonusType;
}

sf::FloatRect Bonus::getBounds() const {
    return shape.getGlobalBounds();
}

bool Bonus::isActive() const {
    return active;
}
