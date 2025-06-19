#include "Bonus.h"
#include<iostream>

Bonus::Bonus(float startX, float startY) //конструктор базового бонуса
    : active(true) { //инициализация
    shape.setSize({ 30.f, 30.f });
    shape.setPosition(startX, startY);
}

void Bonus::update(float deltaTime) {
    const float speed = 150.f;
    shape.move(0.f, speed * deltaTime); // сколько пикселей за кадр (последнее); движение по вертикали вниз

    if (shape.getPosition().y > 600.f) {
        deactivate();
    }
}

void Bonus::draw(sf::RenderWindow& window) const {
    if (active) {
        window.draw(shape);
    }
}

sf::FloatRect Bonus::getBounds() const {
    return shape.getGlobalBounds();
}

bool Bonus::isActive() const {
    return active;
}

void Bonus::deactivate() {
    active = false;
}

//дочерние классы

SizeIncreaseBonus::SizeIncreaseBonus(float startX, float startY) : Bonus(startX, startY) { //конструктор вызывает родительский бонус, потом красит
    shape.setFillColor(sf::Color::Green);
}

void SizeIncreaseBonus::apply(Paddle& paddle, Ball&) {
    paddle.setSize(paddle.getBounds().width * 1.5f, paddle.getBounds().height);
    deactivate();
}


SpeedIncreaseBonus::SpeedIncreaseBonus(float startX, float startY) : Bonus(startX, startY) {
    shape.setFillColor(sf::Color::Red);
}

void SpeedIncreaseBonus::apply(Paddle&, Ball& ball) {
    ball.increaseSpeed();
    deactivate();
}


StickyBonus::StickyBonus(float startX, float startY) : Bonus(startX, startY) {
    shape.setFillColor(sf::Color::Magenta);//пурпурный
}

void StickyBonus::apply(Paddle&, Ball& ball) {
    ball.setSticky(true);
    deactivate();
}


ChangeTrajectoryBonus::ChangeTrajectoryBonus(float startX, float startY) : Bonus(startX, startY) {
    shape.setFillColor(sf::Color::Cyan); //бирюзовый
}

void ChangeTrajectoryBonus::apply(Paddle&, Ball& ball) {
    ball.changeTrajectory();
    deactivate();
}

OneTimePaddleBonus::OneTimePaddleBonus(float startX, float startY) : Bonus(startX, startY) {
    shape.setFillColor(sf::Color::Yellow);
}

void OneTimePaddleBonus::apply(Paddle& paddle, Ball& ball) {
    paddle.setOneTime(true);
    deactivate();
}

//создание бонуса по типу
std::unique_ptr<Bonus> BonusFactory::createBonus(float x, float y, Type type) {//возвращаем дочерний класс, но через указатель на базовый.
    switch (type) {
    case Type::SIZE_INCREASE:
        return std::make_unique<SizeIncreaseBonus>(x, y);
    case Type::SPEED_INCREASE:
        return std::make_unique<SpeedIncreaseBonus>(x, y);
    case Type::STICKY:
        return std::make_unique<StickyBonus>(x, y);
    case Type::CHANGE_TRAJECTORY:
        return std::make_unique<ChangeTrajectoryBonus>(x, y);
    case Type::ONE_TIME_PADDLE:
        return std::make_unique<OneTimePaddleBonus>(x, y);
    default:
        return nullptr;
    }
}
