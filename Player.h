#include "Sphere.h"
#include "Weapon.h"
#include "PowerUp.h"
#include "GameObject.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include "Bullet.h"

#ifndef PLAYER_H_
#define PLAYER_H_

class Player : public GameObject
{
private:
    GLfloat angle, speed;
    int health, score, maxHealth;
    int list, shootDelay;
    Weapon weapon;
    GLfloat speedBoost;
    int speedBoostTimer;
    unsigned int texture;
    GLUquadricObj *qobjPlayer;

public:
    Player();
    Player(GLfloat angle, GLfloat speed, int health, int score, int list, Sphere shpere, unsigned int texture);
    void update() override;
    void draw() override;
    void takeDamage(int damage);
    int getHealth();
    // void addWeapon(Weapon weapon);
    void increaseHealth(int amount);
    bool move(bool key[]);
    GLfloat getAngle();
    void upgradeWeapon(void);
    std::vector<Bullet> shoot(void);
    void drawHealthBar(void);
    void applyPowerUp(PowerUp type);
};

#endif
