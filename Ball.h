#pragma once
#include <SFML/Graphics.hpp>
#include <random>

class Ball {
public:
    explicit Ball(float startX, float startY, float radius); // explicit - запрещает случайные преобразовани€ типов; создание м€ча по позици€х с радусом

    void update(float deltaTime, float paddleX, float paddleY, float paddleWidth);
    void draw(sf::RenderWindow& window) const; //не мен€ет м€чик, только рисует

    sf::FloatRect getBounds() const; //возр границы м€чика

    void checkCollision(const sf::RenderWindow& window); //удар об стены
    void checkCollisionWithPaddle(float paddleX, float paddleY, float paddleWidth, float paddleHeight, bool isOneTime);

    void increaseSpeed();
    void changeTrajectory();

    void setSticky(bool value); // вкл./выкл прилипаение
    bool isSticky() const; //активен эффект?

    void releaseSticky();//отпустить м€ч

    void bounceHorizontal(); //отскоки (х)
    void bounceVertical(); //(у)

    void reset(float startX, float startY); //при падении м€ча -сброс

    bool isOutOfBounds(float windowHeight) const; // упал ли м€ч

private:

    void applyStickyEffect(float paddleX, float paddleY, float paddleWidth); //приклеивание к paddle

    sf::CircleShape shape;
    sf::Vector2f velocity; //скорость по х и у
    float speed;

    bool sticky; //активен ли режим липкости
    bool isStickyActive; // м€ч прилип

    // не завис€т от конкр. м€чика, вызываютс€ без объекта
    static float degToRad(float degrees) noexcept { // не кидает исключение
        return degrees * 3.14159265f / 180.f;
    }

    //дл€ изменени€ траектории
    static float randomFloat(float min, float max);
};
