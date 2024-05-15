#include "Sphere.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 

Sphere::Sphere(){}
Sphere::Sphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius): x(x), y(y), z(z), radius(radius){}
Sphere::~Sphere(){};