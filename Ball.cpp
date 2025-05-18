#include "Ball.h"
#include "Paddle.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>

Ball::Ball(float startX, float startY, float radius)
    : speed(400.0f), sticky(false), isStickyActive(false) {
    shape.setRadius(radius);
    shape.setPosition(startX, startY);
    shape.setFillColor(sf::Color::White);

    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }

    float angle = 45.0f * 3.14159f / 180.0f; 
    velocity.x = speed * std::cos(angle);
    velocity.y = -speed * std::sin(angle);
}

void Ball::update(float deltaTime, float paddleX, float paddleY, float paddleWidth) {
    if (sticky && isStickyActive) {
        applyStickyEffect(paddleX, paddleY, paddleWidth);
        return; // м€ч следует за платформой
    }
    shape.move(velocity * deltaTime);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Ball::getBounds() const {
    return shape.getGlobalBounds();
}

void Ball::checkCollision(const sf::RenderWindow& window) {
    if (shape.getPosition().x <= 0 || shape.getPosition().x + shape.getRadius() * 2 >= window.getSize().x) {
        bounceHorizontal();
    }
    if (shape.getPosition().y <= 0) {
        bounceVertical();
    }
}

void Ball::checkCollisionWithPaddle(float paddleX, float paddleY, float paddleWidth, float paddleHeight, bool isOneTime) {
    sf::FloatRect paddleBounds(paddleX, paddleY, paddleWidth, paddleHeight);

    if (shape.getGlobalBounds().intersects(paddleBounds) && !isStickyActive) {

        float ballCenterX = shape.getPosition().x + shape.getRadius();
        float paddleCenterX = paddleX + paddleWidth / 2.0f;

        float offset = (ballCenterX - paddleCenterX) / (paddleWidth / 2.0f); // смещение от центра

        float angle = offset * 60.0f; // макс угол ±60∞

        float angleRadians = angle * 3.14159f / 180.0f; //радианы

        float newSpeed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        velocity.x = newSpeed * sin(angleRadians);
        velocity.y = -abs(newSpeed * cos(angleRadians)); // всегда вверх после касани€
    }
}

void Ball::bounceHorizontal() {
    velocity.x = -velocity.x;
}

void Ball::bounceVertical() {
    velocity.y = -velocity.y;
}

void Ball::increaseSpeed() {
    velocity *= 1.2f;
}

void Ball::changeTrajectory() {
    // угол от -60∞ до +60∞
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDist(-60.0f, 60.0f);

    float angle = angleDist(gen);
    float angleRadians = angle * 3.14159f / 180.0f;

    float newSpeed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    // пересчЄт направлени€ по новому углу
    velocity.x = newSpeed * sin(angleRadians);
    velocity.y = -abs(newSpeed * cos(angleRadians)); // вверх или вниз случайно

    if (rand() % 2 == 0) {
        velocity.y = -velocity.y;
    }
}

void Ball::setSticky(bool value) {
    sticky = value;
    isStickyActive = value;
}

bool Ball::isSticky() const {
    return sticky;
}

void Ball::applyStickyEffect(float paddleX, float paddleY, float paddleWidth) {
    float radius = shape.getRadius();
    shape.setPosition(
        paddleX + paddleWidth / 2 - radius,
        paddleY - radius * 2
    );

    velocity.x = 0;
    velocity.y = 0;
}

void Ball::releaseSticky() {
    if (sticky && isStickyActive) {
        velocity.x = 0;
        velocity.y = -speed; // летит вверх
        isStickyActive = false; // больше не липнет
    }
}
