#include "Paddle.h"

// конструктор класса (не выбрасывает исключений)
Paddle::Paddle(float startX, float startY, float width, float height) noexcept
    : speed(500.f), // инициализация
    oneTimePaddle(false),
    oneTimeUsed(false)
{
    shape.setSize({ width, height });
    shape.setPosition(startX, startY);
    shape.setFillColor(sf::Color::White);

    oneTimeShape.setSize({ width, height });
    oneTimeShape.setFillColor(sf::Color::Yellow);
    oneTimeShape.setPosition(startX, startY - height - 10.f); // 10 пикселей выше основной

}

void Paddle::moveLeft(float deltaTime) noexcept {
    shape.move(-speed * deltaTime, 0.f);
}

void Paddle::moveRight(float deltaTime) noexcept {
    shape.move(speed * deltaTime, 0.f);
}

void Paddle::update(const sf::RenderWindow& window, float deltaTime) noexcept {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        moveLeft(deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        moveRight(deltaTime);
    }

    // ограничение по краям окна
    if (shape.getPosition().x < 0)
        shape.setPosition(0, shape.getPosition().y);
    else if (shape.getPosition().x + shape.getSize().x > window.getSize().x)
        shape.setPosition(window.getSize().x - shape.getSize().x, shape.getPosition().y);
}

void Paddle::draw(sf::RenderWindow& window) const noexcept {
    window.draw(shape); // основная каретка

    if (oneTimePaddle && !oneTimeUsed) {
        window.draw(oneTimeShape); // одноразовая каретка
    }
}

// возвращает прямоугольник, который обводит платформу (используется для коллизий)
sf::FloatRect Paddle::getBounds() const noexcept {
    return shape.getGlobalBounds();
}

sf::Vector2f Paddle::getPosition() const noexcept {
    return shape.getPosition(); //корд. платформы
}

void Paddle::setSize(float width, float height) noexcept {
    shape.setSize({ width, height });
}

void Paddle::setOneTime(bool value) noexcept {
    oneTimePaddle = value;
    oneTimeUsed = false;

    if (value) {
        float paddleHeight = shape.getSize().y;
        float randomX = static_cast<float>(rand() % static_cast<int>(windowWidth - shape.getSize().x));

        //устанавливаем доп каретку в случайной позиции по X,
        // а по Y - на 20 пикселей выше основной каретки (20 пикселей)
        oneTimeShape.setPosition(randomX, shape.getPosition().y - paddleHeight - 20.f);
    }
}

bool Paddle::isOneTime() const noexcept { //активен ли режим одноразовой каретки
    return oneTimePaddle;
}

bool Paddle::isOneTimeUsed() const noexcept {
    return oneTimeUsed;
}

void Paddle::resetOneTime() noexcept {
    oneTimePaddle = false;
    oneTimeUsed = false;
    shape.setSize({ defaultWidth, defaultHeight });
}
