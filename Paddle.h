#pragma once
#include <SFML/Graphics.hpp>

class Paddle {
public:
    Paddle(float startX, float startY, float width = 100.f, float height = 20.f);

    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void update(const sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

    void setSize(float width, float height);

    void setOneTime(bool value);
    bool isOneTime() const;
    bool isOneTimeUsed() const;

    void resetOneTime();

private:
    sf::RectangleShape shape;
    float speed;
    bool oneTimePaddle; // активировано?
    bool oneTimeUsed;// было ли использовано

};
