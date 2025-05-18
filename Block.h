#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>

class Block {
public:
    enum class Type {
        Violet,
        Bonus,
        Cyan,
        Green
    };
private:
    sf::RectangleShape shape;
    int health;
    Type type;

public:
    Block(float startX, float startY, float width = 60.0f, float height = 20.0f, 
        int health = 1, sf::Color color = sf::Color::Green, Type type = Type::Green);

    void draw(sf::RenderWindow& window) const;
    bool isDestroyed() const;
    void hit();
    int getHealth() const;
    sf::FloatRect getBounds();
    sf::Color getColor() const;

    Type getType() const;
};

#endif
