#include "Bonus.h"

Bonus::Bonus(float startX, float startY, Type type)
    : bonusType(type), speed(150.f), active(true) {
    shape.setSize(sf::Vector2f(30.0f, 30.0f));  // Размер бонуса
    shape.setPosition(startX, startY);

    // Без цветовой классификации — просто белый цвет
    shape.setFillColor(sf::Color::White);
}

void Bonus::update(float deltaTime) {
    // Двигаем бонус вниз по экрану
    shape.move(0.0f, speed * deltaTime);

    // Если бонус выходит за нижнюю границу окна, деактивируем
    if (shape.getPosition().y > 600) {
        active = false;
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
    switch (bonusType) {
    case Type::SIZE_INCREASE:
        paddle.setSize(paddle.getBounds().width * 1.2f, paddle.getBounds().height);
        break;
    case Type::SPEED_INCREASE:
        ball.increaseSpeed();
        break;
    case Type::STICKY:
        ball.setSticky(true);
        break;
        break;
    case Type::CHANGE_TRAJECTORY:
        ball.changeTrajectory();
        break;
    case Type::ONE_TIME_PADDLE:
        paddle.setOneTime(true);
        break;
    }

    active = false;  // Деактивируем бонус после активации
}
