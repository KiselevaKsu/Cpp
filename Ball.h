#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

class Ball {
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed;

public:
    Ball(float startX, float startY, float radius);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void checkCollision(const sf::RenderWindow& window);

    sf::FloatRect getBounds();
    void bounceHorizontal();
    void bounceVertical();
};

#endif
