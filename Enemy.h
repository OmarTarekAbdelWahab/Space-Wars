#include "GameObject.h"
#include <vector>
#include "Bullet.h"
#ifndef ENEMY_H_
#define ENEMY_H_

class Enemy : public GameObject
{
private:
    GLfloat angle, speed;
    int health, difficulty;
    std::vector<GLfloat> color;
    int shootDelay;
    bool stop = false;
    GLUquadricObj *qobjEnemy;
    unsigned int texture;

public:
    Enemy();
    Enemy(Sphere sphere, std::vector<GLfloat> color, GLfloat angle, GLfloat speed, int health, int difficulty, unsigned int texture);
    ~Enemy();
    void update(void) override;
    void draw(void) override;
    bool move(Sphere targetLoc);
    void takeDamage(int damage);
    // void addWeapon(Weapon weapon);
    GLfloat getAngle();
    std::vector<Bullet> shoot(void);
};

#endif