#include "Paddle.h"

Paddle::Paddle(float startX, float startY, float width, float height)
    : speed(450.f), oneTimePaddle(false), oneTimeUsed(false) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(startX, startY);
    shape.setFillColor(sf::Color::White);
}

void Paddle::moveLeft(float deltaTime) {
    if (shape.getPosition().x > 0) {
        shape.move(-speed * deltaTime, 0);
    }
}

void Paddle::moveRight(float deltaTime) {
    if (shape.getPosition().x + shape.getSize().x < 800) {
        shape.move(speed * deltaTime, 0);
    }
}

void Paddle::update(const sf::RenderWindow& window) {
    if (oneTimePaddle && oneTimeUsed) {
        // если платформа одноразова€ и уже использована Ч скрываем еЄ
        shape.setSize(sf::Vector2f(0, 0)); // скрываем платформу
    }
}

void Paddle::draw(sf::RenderWindow& window) {
    if (!oneTimePaddle || (oneTimePaddle && !oneTimeUsed)) {
        window.draw(shape);
    }
}

sf::FloatRect Paddle::getBounds() const {
    return shape.getGlobalBounds();
}

sf::Vector2f Paddle::getPosition() const {
    return shape.getPosition();
}

void Paddle::setSize(float width, float height) {
    shape.setSize(sf::Vector2f(width, height));
}

void Paddle::setOneTime(bool value) {
    oneTimePaddle = value;
    oneTimeUsed = false;
}

bool Paddle::isOneTime() const {
    return oneTimePaddle;
}

bool Paddle::isOneTimeUsed() const {
    return oneTimeUsed;
}

void Paddle::resetOneTime() {
    oneTimePaddle = false;
    oneTimeUsed = false;
    shape.setSize(sf::Vector2f(100.0f, 20.0f));
}
