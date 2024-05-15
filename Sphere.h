#include <GL/glew.h>
#include <GL/freeglut.h> 

#ifndef SPHERE_H_
#define SPHERE_H_

class Sphere{
public:
    Sphere();
    GLfloat x, y, z, radius;
    Sphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius);
    ~Sphere();
};

#endif