#include "GameObject.h"
#include <vector>

#ifndef POWERUP_H_
#define POWERUP_H_
enum PowerType {HEALTH = 0, SPEED = 1, WEAPON = 2};
class PowerUp: public GameObject{
private:
    GLfloat angle, heightAngle, edgeLength;
    std::vector<GLfloat> color;
    PowerType type;
    int healthIncrease;
    GLfloat speedBoost;
public:
    ~PowerUp();
    PowerUp();
    PowerUp(Sphere sphere, PowerType type);
    void update() override;
    void draw() override;
    PowerType getType();
    int getHealthIncrease();
    GLfloat getSpeedBoost();
};

#endif