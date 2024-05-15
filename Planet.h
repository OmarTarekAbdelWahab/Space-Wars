#include "GameObject.h"
#include "Sphere.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#ifndef PLANET_H_
#define PLANET_H_

class Planet: public GameObject{
    GLfloat orbitRadius, orbitSpeed, spinSpeed;
    GLfloat orbitAngle, spinAngle;
    // Planet moon();
public:
    Planet();
    Planet(GLfloat orbitRadius, GLfloat orbitSpeed, GLfloat spinSpeed, GLfloat orbitAngle, GLfloat spinAngle, Sphere sphere);
    ~Planet();
    void update() override;
    void draw() override;
};

#endif