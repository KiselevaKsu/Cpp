#pragma once
#include <SFML/Graphics.hpp>

class Ball {
public:
    Ball(float startX, float startY, float radius);

    void update(float deltaTime, float paddleX, float paddleY, float paddleWidth);
    void draw(sf::RenderWindow& window);
    void checkCollision(const sf::RenderWindow& window);
    void checkCollisionWithPaddle(float paddleX, float paddleY, float paddleWidth, float paddleHeight, bool isOneTime);

    void bounceHorizontal();
    void bounceVertical();
    void increaseSpeed();
    void changeTrajectory();

    void setSticky(bool value);
    bool isSticky() const;
    void releaseSticky();

    sf::FloatRect getBounds() const;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed;
    bool sticky;
    bool isStickyActive;

    void applyStickyEffect(float paddleX, float paddleY, float paddleWidth);
};
