#include "Sphere.h"

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

class GameObject{
protected:
    Sphere sphere;
public:
    GameObject();
    GameObject(Sphere sphere);
    virtual void draw() = 0;
    virtual void update() = 0;
    const Sphere& getSphere();
};
#endif