#include "Ball.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

// статическа€ функци€ класса ball (вызываетс€ без создани€ объекта)
float Ball::randomFloat(float min, float max) {
    //static - сохран€ет генератор между вызовами
    //thread_local - каждый поток получает собственный генератор
    // mt19937 - генератор случ. чисел; 
    static thread_local std::mt19937 generator{ std::random_device{}() };
    // distribution(min, max) - создаЄт диапазон от мин - макс
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator); // выдаЄт случ. число в диапазоне
}

// конструктор класса ball
Ball::Ball(float startX, float startY, float radius)
    : speed(400.f), sticky(false), isStickyActive(false), velocity{} {

    shape.setRadius(radius);
    shape.setFillColor(sf::Color::White);
    shape.setPosition(startX, startY);

    // движение под углом 45 градусов
    const float initialAngle = degToRad(45.f); // в радианы
    velocity.x = speed * std::cos(initialAngle);
    velocity.y = -speed * std::sin(initialAngle); // минус тк летит вверх
}

void Ball::update(float deltaTime, float paddleX, float paddleY, float paddleWidth) {
    if (sticky && isStickyActive) {
        applyStickyEffect(paddleX, paddleY, paddleWidth);
        return;
    }
    shape.move(velocity * deltaTime); // если не прилип м€ч
}

void Ball::draw(sf::RenderWindow& window) const { // const - метод не мен€ет Ball
    window.draw(shape);
}

sf::FloatRect Ball::getBounds() const {
    return shape.getGlobalBounds();
}

void Ball::checkCollision(const sf::RenderWindow& window) {
    const auto pos = shape.getPosition();
    const float diameter = shape.getRadius() * 2.f;
    const auto windowSize = window.getSize();

    // отскок от границ (лево\право\верх)
    if (pos.x <= 0.f || pos.x + diameter >= windowSize.x) {
        bounceHorizontal();
    }
    if (pos.y <= 0.f) {
        bounceVertical();
    }
}

void Ball::checkCollisionWithPaddle(float paddleX, float paddleY, float paddleWidth, float paddleHeight, bool /*isOneTime*/) {
    sf::FloatRect paddleBounds(paddleX, paddleY, paddleWidth, paddleHeight);

    if (shape.getGlobalBounds().intersects(paddleBounds) && !isStickyActive) {
        float ballCenterX = shape.getPosition().x + shape.getRadius();
        float paddleCenterX = paddleX + paddleWidth / 2.f;

        float offset = (ballCenterX - paddleCenterX) / (paddleWidth / 2.f); // где по платформе ударилс€ м€ч

        float angleDegrees = offset * 60.f; // угол отражени€ +-60 град.
        float angleRadians = degToRad(angleDegrees);

        // скорость та же, мен€етс€ направление
        float currentSpeed = std::hypot(velocity.x, velocity.y);

        velocity.x = currentSpeed * std::sin(angleRadians);
        velocity.y = -std::abs(currentSpeed * std::cos(angleRadians)); // всегда вверх после касани€
    }
}

void Ball::reset(float startX, float startY) {
    shape.setPosition(startX, startY);

    // движение под углом 45 градусов (как в конструкторе)
    const float initialAngle = degToRad(45.f);
    velocity.x = speed * std::cos(initialAngle);
    velocity.y = -speed * std::sin(initialAngle);

    // сброс sticky состо€ний
    sticky = false;
    isStickyActive = false;
}
 // проверка на падение
bool Ball::isOutOfBounds(float windowHeight) const {
    return shape.getPosition().y - shape.getRadius() > windowHeight;
}

//отскоки 
void Ball::bounceHorizontal() {
    velocity.x = -velocity.x;
}

void Ball::bounceVertical() {
    velocity.y = -velocity.y;
}

void Ball::increaseSpeed() {
    velocity *= 1.1f;
}

void Ball::changeTrajectory() {
    float angleDegrees = randomFloat(-60.f, 60.f);
    float angleRadians = degToRad(angleDegrees);

    float currentSpeed = std::hypot(velocity.x, velocity.y);

    velocity.x = currentSpeed * std::sin(angleRadians);
    velocity.y = -std::abs(currentSpeed * std::cos(angleRadians));

    if (rand() % 2 == 0) {
        velocity.y = -velocity.y;
    }
}

void Ball::setSticky(bool value) {
    sticky = value; //доступен ли бонус
    isStickyActive = value; //применЄн ли
}

bool Ball::isSticky() const {
    return sticky;
}

void Ball::applyStickyEffect(float paddleX, float paddleY, float paddleWidth) {
    float radius = shape.getRadius();
    shape.setPosition(paddleX + paddleWidth / 2.f - radius, paddleY - radius * 2.f);

    velocity = { 0.f, 0.f };
}

void Ball::releaseSticky() {
    if (sticky && isStickyActive) {
        velocity = { 0.f, -speed };
        isStickyActive = false;
    }
}
