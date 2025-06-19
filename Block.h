#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Ball;
class Bonus;

class Block { //абстр. класс
protected: // не видно снаружи, но видно в дочерних классах
    sf::RectangleShape shape; //прямоугольник
    int health;

public:
    Block(float startX, float startY, float width, float height, int health, sf::Color color); //констр.
    virtual ~Block() = default; //вирт. дестр. чтобы при удалении блока через указатель на базу (Block*) вызывался правильный деструктор (дочернего класса)

    virtual void onHit(std::vector<std::unique_ptr<Bonus>>& bonuses, Ball& ball) = 0; // чисто абстрактная функция (переопределена в наследниках)

    void draw(sf::RenderWindow& window) const; //рисует блок в окно
    bool isDestroyed() const; 
    void hit();
    sf::FloatRect getBounds() const;
    sf::Color getColor() const;
};

class GreenBlock : public Block { // обычный блок, наследует Block
public:
    GreenBlock(float x, float y);
    void onHit(std::vector<std::unique_ptr<Bonus>>& bonuses, Ball& ball) override;
};

class VioletBlock : public Block { //неразрушаемый
public:
    VioletBlock(float x, float y);
    void onHit(std::vector<std::unique_ptr<Bonus>>&, Ball&) override;
};

class CyanBlock : public Block { //ускорение
public:
    CyanBlock(float x, float y);
    void onHit(std::vector<std::unique_ptr<Bonus>>&, Ball& ball) override;
};

class BonusBlock : public Block { //бонус
public:
    BonusBlock(float x, float y);
    void onHit(std::vector<std::unique_ptr<Bonus>>& bonuses, Ball& ball) override;
};

class BlockFactory { //спец. класс, создающий объекты (случ. блоки)
public:
    static std::unique_ptr<Block> createRandomBlock(float x, float y); // возвращает "умный" (сам удалит объект) указатель (чтобы не париться с delete)
};