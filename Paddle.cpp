#include "Paddle.h"

Paddle::Paddle(float startX, float startY, float width, float height)
    : speed(450.0f), oneTimePaddle(false) {
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
    // Проверяем, если активен одноразовый "щит" и он выходит за нижний край окна
    if (oneTimePaddle && shape.getPosition().y + shape.getSize().y >= window.getSize().y) {
        oneTimePaddle = false; // Отключаем одноразовую каретку
    }
}

void Paddle::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Paddle::getBounds() const{
    return shape.getGlobalBounds();
}

sf::Vector2f Paddle::getPosition() const {
    return shape.getPosition();
}

void Paddle::setSize(float width, float height) {
    shape.setSize(sf::Vector2f(width, height));
}
