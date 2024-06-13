#include "Sphere.h"
#include "Bullet.h"
#include <vector>


#ifndef WEAPON_H_
#define WEAPON_H_


class Weapon{
    int weaponInd;
    std::vector<Bullet> bullets;
public:
    Weapon();
    Weapon(int weaponInd);
    ~Weapon();
    std::vector<Bullet> getBullets(Sphere pos, GLfloat angle);
    void upgrade();
};

#endif