#include "Block.h"
#include "Ball.h"
#include "Bonus.h"

//создаём прямоугольник, ставим его в нужное место, задаём размер, цвет и здоровье
Block::Block(float startX, float startY, float width, float height, int health, sf::Color color) // конструктор класса block
    : health(health) { // внутренне поле health (из protected) = арументу конструктора (вместо this->health=health внутри тела)(присваивает здоровье)
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(sf::Vector2f(startX, startY));
    shape.setFillColor(color);
}

void Block::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

bool Block::isDestroyed() const {
    return health <= 0;
}

void Block::hit() {
    if (health > 0) {
        health--;
        if (health == 0) {
            shape.setFillColor(sf::Color::Transparent);
        }
    }
}

sf::FloatRect Block::getBounds() const { //получает границы блока
    sf::FloatRect bounds = shape.getGlobalBounds();
    const float padding = 3.f; // чтобы расстояние между блоками не было щелью
    bounds.left -= padding;
    bounds.top -= padding;
    bounds.width += 2 * padding;
    bounds.height += 2 * padding;
    return bounds; //возвращает прямоуг с коорд. и разм.
}

sf::Color Block::getColor() const {
    return shape.getFillColor();
}

GreenBlock::GreenBlock(float x, float y) //наследует конструктор block
    : Block(x, y, 70.0f, 20.0f, 1 + (rand() % 3), sf::Color::Green) {}

void GreenBlock::onHit(std::vector<std::unique_ptr<Bonus>>&, Ball&) {
    hit();
}

VioletBlock::VioletBlock(float x, float y)
    : Block(x, y, 70.0f, 20.0f, 9999, sf::Color(128, 0, 128)) {}

void VioletBlock::onHit(std::vector<std::unique_ptr<Bonus>>&, Ball&) {
    // ничего не делаем
}

CyanBlock::CyanBlock(float x, float y)
    : Block(x, y, 70.0f, 20.0f, 1, sf::Color::Cyan) {}

void CyanBlock::onHit(std::vector<std::unique_ptr<Bonus>>&, Ball& ball) {
    hit();
    ball.increaseSpeed();
}

BonusBlock::BonusBlock(float x, float y)
    : Block(x, y, 70.0f, 20.0f, 1, sf::Color::Yellow) {}

void BonusBlock::onHit(std::vector<std::unique_ptr<Bonus>>& bonuses, Ball&) {
    hit();
    // случайный тип бонуса из enum
    auto type = static_cast<BonusFactory::Type>(rand() % 5);
    // cоздаем бонус через фабрику
    std::unique_ptr<Bonus> bonus = BonusFactory::createBonus(
        shape.getPosition().x + (shape.getSize().x - 30.0f) / 2.0f,
        shape.getPosition().y + shape.getSize().y + 20.0f,
        type
    );

    // если бонус создан успешно, добавляем в вектор
    if (bonus) {
        bonuses.push_back(std::move(bonus));
    }
}


std::unique_ptr<Block> BlockFactory::createRandomBlock(float x, float y) { // возвращается unique_ptr<Block> — умный указатель на базовый тип Block, но внутри — настоящий потомок (GreenBlock, и т.д.)
    static std::random_device rd; //инициал. 1 раз
    static std::mt19937 gen(rd()); // генераторы случайных чисел
    static std::uniform_int_distribution<> dist(0, 3); // 4 типа блоков

    switch (dist(gen)) { // возвращает случайное число от 0 до 3
    case 0: return std::make_unique<GreenBlock>(x, y);
    case 1: return std::make_unique<BonusBlock>(x, y);
    case 2: return std::make_unique<CyanBlock>(x, y);
    case 3: return std::make_unique<VioletBlock>(x, y);
    }

    return std::make_unique<GreenBlock>(x, y);
}
