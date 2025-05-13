#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>

class Paddle {
private:
    sf::RectangleShape shape;
    float speed;

public:
    Paddle(float startX, float startY, float width = 100.0f, float height = 20.0f);

    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds();
};

#endif
