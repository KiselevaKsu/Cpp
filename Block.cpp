#include "Block.h"

Block::Block(float startX, float startY, float width, float height, int health) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(sf::Vector2f(startX, startY));
    shape.setFillColor(sf::Color::Green); // зелЄный Ч обычный блок
    this->health = health;
}

void Block::draw(sf::RenderWindow& window) {
    if (health > 0) {
        window.draw(shape);
    }
}

bool Block::isDestroyed() const {
    return health <= 0;
}

void Block::hit() {
    if (health > 0) {
        health--;
    }
    if (health == 0) {
        shape.setFillColor(sf::Color::Transparent); // делаем невидимым
    }
}

sf::FloatRect Block::getBounds() {
    return shape.getGlobalBounds();
}
