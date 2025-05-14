#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>

class Block {
private:
    sf::RectangleShape shape;
    int health;

public:
    Block(float startX, float startY, float width = 60.0f, float height = 20.0f, int health = 1);

    void draw(sf::RenderWindow& window);
    bool isDestroyed() const;
    void hit();
    int getHealth() const;
    sf::FloatRect getBounds();
};

#endif

