#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include <memory>
#include <vector>

class Bonus { //абстр. класс
public:
    Bonus(float startX, float startY);
    virtual ~Bonus() = default;

    virtual void update(float deltaTime);
    virtual void draw(sf::RenderWindow& window) const;
    virtual void apply(Paddle& paddle, Ball& ball) = 0; // вирт. ф., у каждого бонуса своя реализация этого; как применяется бонус

    sf::FloatRect getBounds() const; //для коллизий (попала ли платформа в бонус)
    bool isActive() const; //геттер: бонус активен?

protected:
    sf::RectangleShape shape;
    bool active;

    void deactivate();
};

class SizeIncreaseBonus : public Bonus {
public:
    SizeIncreaseBonus(float startX, float startY);
    void apply(Paddle& paddle, Ball& ball) override;
};

class SpeedIncreaseBonus : public Bonus {
public:
    SpeedIncreaseBonus(float startX, float startY);
    void apply(Paddle& paddle, Ball& ball) override;
};

class StickyBonus : public Bonus {
public:
    StickyBonus(float startX, float startY);
    void apply(Paddle& paddle, Ball& ball) override;
};

class ChangeTrajectoryBonus : public Bonus {
public:
    ChangeTrajectoryBonus(float startX, float startY);
    void apply(Paddle& paddle, Ball& ball) override;
};

class OneTimePaddleBonus : public Bonus {
public:
    OneTimePaddleBonus(float startX, float startY);
    void apply(Paddle& paddle, Ball& ball) override;
};

class BonusFactory { //чтобы создавать бонусы по типу без названий классов (вместо switch(type){case Type::...})
public: // возвращает умный (удалет автом.) указатель на базу, но фактически внтри - объект дочернего класса
    enum class Type {
        SIZE_INCREASE,
        SPEED_INCREASE,
        STICKY,
        CHANGE_TRAJECTORY,
        ONE_TIME_PADDLE
    };
    //unique_ptr<Bonus>, хранит владеющий объект, удалит при выходе из обл. видимости (сам управляет памятью)
    static std::unique_ptr<Bonus> createBonus(float x, float y, Type type); 
    // static - можно вызвать функцию без создания объекта ( b = BonusFactory::createbonus(..))
};
