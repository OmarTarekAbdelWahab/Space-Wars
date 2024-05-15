#include "Sphere.h"
#include "GameObject.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 

#ifndef PLAYER_H_
#define PLAYER_H_

class Player: public GameObject{
private:
    GLfloat angle, speed;
    int health, score;
    int list;
    //Weapon weapon;
public:
    Player();
    Player(GLfloat angle, GLfloat speed, int health, int score, int list, Sphere shpere);
    void update() override;
    void draw() override;
    void takeDamage(int damage);
    // void addWeapon(Weapon weapon);
    void increaseHealth(int amount);
    void move(bool key[]);
    GLfloat getAngle();
};

#endif