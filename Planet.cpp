#include <iostream>
#include <math.h>
#include "Planet.h"
#include "Sphere.h"
#include "GameObject.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 

using namespace std;
Planet::~Planet(){}
Planet::Planet(){}
Planet::Planet(vector<GLfloat> color, GLfloat orbitRadius, GLfloat orbitSpeed, GLfloat spinSpeed, GLfloat orbitAngle, GLfloat spinAngle, Sphere sphere)
: color(color), orbitRadius(orbitRadius), orbitSpeed(orbitSpeed), spinSpeed(spinSpeed), orbitAngle(orbitAngle), spinAngle(spinAngle), GameObject(sphere){}

void Planet::update(){
    orbitAngle += orbitSpeed;
    if(orbitAngle > 360.) orbitAngle -= 360.;
    spinAngle += spinSpeed;
    if(spinAngle > 360.) spinAngle -= 360.;
    sphere.x = orbitRadius*sin(orbitAngle*M_PI/180.0);
    sphere.z = orbitRadius*cos(orbitAngle*M_PI/180.0);
}
void Planet::draw(){
    glPushMatrix();
        // rotate around the sun
        glColor3fv(color.data());
        // glColor3f(color[0], color[1], color[2]);
        glTranslatef(sphere.x, sphere.y, sphere.z);
        // spin in place
        // glRotatef(90., 1., 0., 0.);
        // glRotatef(spinAngle, 0., 1., 0.);
        // glRotatef(spinAngle, 0.0, 1.0, 0.0);
        glRotatef(90., 1.0, 0.0, 0.0);

        glutWireSphere(sphere.radius, 16, 16);
    glPopMatrix();
}