#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>

class Paddle {
private:
    sf::RectangleShape shape;
    float speed;
    bool oneTimePaddle;  // Для одноразового дна

public:
    Paddle(float startX, float startY, float width = 100.0f, float height = 20.0f);

    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;  // Метод для получения позиции
    void setSize(float width, float height);  // Новый метод для установки размера
    void setOneTime(bool value) { oneTimePaddle = value; }  // Устанавливаем состояние одноразового дна
    bool isOneTime() const { return oneTimePaddle; }  // Проверяем, одноразовое ли дно
};

#endif
