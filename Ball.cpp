#include "Ball.h"

Ball::Ball(float startX, float startY, float radius) {
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::White);
    shape.setPosition(sf::Vector2f(startX, startY));
    shape.setOrigin(radius, radius);

    speed = 300.0f;
    velocity = sf::Vector2f(-speed, -speed); // Нач. скорость по диаг. вверх-влево
}

void Ball::update(float deltaTime) {
    shape.move(velocity * deltaTime);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Ball::checkCollision(const sf::RenderWindow& window) {
    if (shape.getPosition().x <= 0 || shape.getPosition().x + shape.getRadius() * 2 >= window.getSize().x)
        bounceHorizontal();

    if (shape.getPosition().y <= 0)
        bounceVertical();
}

sf::FloatRect Ball::getBounds() {
    return shape.getGlobalBounds();
}

void Ball::bounceHorizontal() {
    velocity.x = -velocity.x;
}

void Ball::bounceVertical() {
    velocity.y = -velocity.y;
}
