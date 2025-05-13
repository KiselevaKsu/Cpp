#include "Paddle.h"

Paddle::Paddle(float startX, float startY, float width, float height) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(sf::Vector2f(startX, startY));
    shape.setFillColor(sf::Color::White);
    speed = 400.0f;
}

void Paddle::moveLeft(float deltaTime) {
    shape.move(sf::Vector2f(-speed * deltaTime, 0));
}

void Paddle::moveRight(float deltaTime) {
    shape.move(sf::Vector2f(speed * deltaTime, 0));
}

void Paddle::update(const sf::RenderWindow& window) {
    if (shape.getPosition().x < 0)
        shape.setPosition(sf::Vector2f(0, shape.getPosition().y));
    if (shape.getPosition().x + shape.getSize().x > window.getSize().x)
        shape.setPosition(sf::Vector2f(window.getSize().x - shape.getSize().x, shape.getPosition().y));
}

void Paddle::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Paddle::getBounds() {
    return shape.getGlobalBounds();
}
