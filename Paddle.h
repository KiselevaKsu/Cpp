#pragma once

#include <SFML/Graphics.hpp>

class Paddle {
public:
    // explicet - от преобразовани€ типов, noexept - не выбрасываю ошибки (оптимизаци€)
    explicit Paddle(float startX = 350.0f, float startY = 550.0f, float width = 100.0f, float height = 20.0f) noexcept;

    void moveLeft(float deltaTime) noexcept; //deltaTime - врем€ с прошлого кадра
    void moveRight(float deltaTime) noexcept;

    void update(const sf::RenderWindow& window, float deltaTime) noexcept;
    void draw(sf::RenderWindow& window) const noexcept;

    sf::FloatRect getBounds() const noexcept;
    sf::Vector2f getPosition() const noexcept;

    void setSize(float width, float height) noexcept;

    void setOneTime(bool value) noexcept;
    bool isOneTime() const noexcept;
    bool isOneTimeUsed() const noexcept;

    void resetOneTime() noexcept;

    sf::FloatRect getOneTimeBounds() const noexcept {
        return oneTimeShape.getGlobalBounds();
    }

private:
    sf::RectangleShape shape;
    sf::RectangleShape oneTimeShape;

    float speed;
    bool oneTimePaddle = false;
    bool oneTimeUsed = false;

    static constexpr float windowWidth = 800.0f; // static - одни на весь класс, не хран€тс€ в каждом объекте 
    static constexpr float defaultWidth = 100.0f; // constexpr -  коснт.,известные во врем€ компил€ции
    static constexpr float defaultHeight = 20.0f;
};
