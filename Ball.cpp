#include "Ball.h"
#include <cstdlib> // Для rand()
#include <ctime>   // Для srand(time(0))
#include <iostream>
#include <random>

Ball::Ball(float startX, float startY, float radius)
    : speed(400.0f), sticky(false), extraLife(false) {
    shape.setRadius(radius);
    shape.setPosition(startX, startY);
    shape.setFillColor(sf::Color::White);

    float angle = 45.0f * 3.14159f / 180.0f;  // 45 градусов
    velocity.x = speed * std::cos(angle);
    velocity.y = -speed * std::sin(angle);    // минус для направления вверх
}

// Функция изменения траектории
void Ball::changeTrajectory() {
    // Генерация случайного угла отклонения (от -45 до 45 градусов)
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    float angle = (std::rand() % 90 - 45) * 3.14159f / 180.0f; // Преобразуем в радианы

    float currentSpeed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    float newAngle = std::atan2(velocity.y, velocity.x) + angle;

    // Обновляем скорость с учетом нового направления
    velocity.x = currentSpeed * std::cos(newAngle);
    velocity.y = currentSpeed * std::sin(newAngle);
}

// Основное обновление мяча
void Ball::update(float deltaTime) {
    if (sticky) {
        return; // Мяч не движется, если он липкий
    }

    shape.move(velocity * deltaTime);
}

// Отрисовка мяча
void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

// Границы мяча
sf::FloatRect Ball::getBounds() {
    return shape.getGlobalBounds();
}

// Проверка столкновений с краями окна
void Ball::checkCollision(const sf::RenderWindow& window) {
    if (shape.getPosition().x <= 0 || shape.getPosition().x + shape.getRadius() * 2 >= window.getSize().x) {
        bounceHorizontal();
    }
    if (shape.getPosition().y <= 0) {
        bounceVertical();
    }
}

// Проверка столкновений с кареткой
void Ball::checkCollisionWithPaddle(Paddle& paddle) {
    if (getBounds().intersects(paddle.getBounds())) {
        bounceVertical();
        if (sticky) {
            applyStickyEffect(paddle);  // Применяем липкость
        }
    }
}

// Отражение по горизонтали
void Ball::bounceHorizontal() {
    velocity.x = -velocity.x;
}

// Отражение по вертикали
void Ball::bounceVertical() {
    velocity.y = -velocity.y;
}

// Увеличение скорости мяча
void Ball::increaseSpeed() {
    velocity *= 1.2f;
}

// Применение липкого эффекта
void Ball::applyStickyEffect(const Paddle& paddle) {
    shape.setPosition(
        paddle.getPosition().x + paddle.getBounds().width / 2 - shape.getRadius(),
        paddle.getPosition().y - shape.getRadius() * 2
    );
}
