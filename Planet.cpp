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
Planet::Planet(GLfloat orbitRadius, GLfloat orbitSpeed, GLfloat spinSpeed, GLfloat orbitAngle, GLfloat spinAngle, Sphere sphere)
: orbitRadius(orbitRadius), orbitSpeed(orbitSpeed), spinSpeed(spinSpeed), orbitAngle(orbitAngle), spinAngle(spinAngle), GameObject(sphere){

}
void Planet::update(){
    orbitAngle += orbitSpeed;
    if(orbitAngle > 360.) orbitAngle -= 360.;
    spinAngle += spinSpeed;
    if(spinAngle > 360.) spinAngle -= 360.;
}
void Planet::draw(){
    glPushMatrix();
        // rotate around the sun
        glRotatef(orbitAngle, 0.0, 1.0, 0.0);
        glTranslatef(orbitRadius, 0.0, 0.0);
        // spin in place
        glRotatef(spinAngle, 0., 1., 0.);
        glRotatef(45., 0., 0., 1.);
        glutWireSphere(sphere.radius, 16, 16);
    glPopMatrix();
}