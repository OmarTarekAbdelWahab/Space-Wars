#include <iostream>
#include <math.h>
#include "Planet.h"
#include "Sphere.h"
#include "GameObject.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;
Planet::~Planet() {}
Planet::Planet()
{
    qobj = gluNewQuadric();
}
Planet::Planet(vector<GLfloat> color, GLfloat orbitRadius, GLfloat orbitSpeed, GLfloat spinSpeed, GLfloat orbitAngle, GLfloat spinAngle, Sphere sphere, unsigned int texture)
    : color(color), orbitRadius(orbitRadius), orbitSpeed(orbitSpeed), spinSpeed(spinSpeed), orbitAngle(orbitAngle), spinAngle(spinAngle), GameObject(sphere), texture(texture)
{
    qobj = gluNewQuadric();
}

void Planet::setMoon(unsigned int moonTexture)
{
    hasMoon = true;
    this->moonTexture = moonTexture;
    moonOrbitSpeed = 1.0;
    moonOrbitAngle = 0.0;
    moonSpinSpeed = 1.0;
    moonSpinAngle = 0.0;
}

void Planet::update()
{
    orbitAngle += orbitSpeed;
    if (orbitAngle > 360.)
        orbitAngle -= 360.;
    spinAngle += spinSpeed;
    if (spinAngle > 360.)
        spinAngle -= 360.;
    sphere.x = orbitRadius * sin(orbitAngle * M_PI / 180.0);
    sphere.z = orbitRadius * cos(orbitAngle * M_PI / 180.0);
    if (hasMoon)
    {
        moonOrbitAngle += moonOrbitSpeed;
        if (moonOrbitAngle > 360.)
            moonOrbitAngle -= 360.;
        moonSpinAngle += moonSpinSpeed;
        if (moonSpinAngle > 360.)
            moonSpinAngle -= 360.;
    }
}
void Planet::draw()
{
    glPushMatrix();
    if (sphere.radius == 80.0)
    {
        // In case of the sun, Turn global ambient lightin up so it is well lit.
        float globAmb[] = {1.0, 1.0, 1.0, 1.0};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
    }
    GLfloat matAmbANdDif[] = {color[0], color[1], color[2], 1.0};
    GLfloat matSpec[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat matShine[] = {50.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbANdDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    // rotate around the sun
    // glColor3fv(color.data());
    // glColor3f(color[0], color[1], color[2]);
    glTranslatef(sphere.x, sphere.y, sphere.z);
    // spin in place
    // glRotatef(90., 1., 0., 0.);
    // glRotatef(spinAngle, 0., 1., 0.);
    // glRotatef(spinAngle, 0.0, 1.0, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glRotatef(-spinAngle, 0.0, 0.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    gluQuadricTexture(qobj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture);
    gluSphere(qobj, sphere.radius, 16, 16);

    if (hasMoon)
    {
        glRotatef(moonOrbitAngle, 0.0, 1.0, 0.0);
        glTranslatef(55.0, 0.0, 0.0);
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        glRotatef(-moonSpinAngle, 0.0, 0.0, 1.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        gluQuadricTexture(qobj, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D, moonTexture);
        gluSphere(qobj, 8.0, 16, 16);
    }

    glPopMatrix();
    if (sphere.radius == 80.0)
    {
        // Don't forget to lower it back down
        float globAmb1[] = {0.5, 0.5, 0.5, 1.0};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb1);
    }
}