#include <iostream>
#include "Bullet.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
using namespace std;
Bullet::Bullet(Sphere sphere, vector<GLfloat> color, GLfloat angle, GLfloat speed, int damage): GameObject(sphere), color(color), angle(angle), speed(speed), damage(damage){
    isDestroy = false;
}
Bullet::~Bullet(){}
int Bullet::getDamage(void){return damage;}
bool Bullet::destroyBullet(void){return isDestroy;}
void Bullet::update(void){
    sphere.x += speed*sin(angle*M_PI/180.);
    sphere.z += speed*cos(angle*M_PI/180.);
    if(sphere.x < -1000.0 or sphere.x > 1000.0 or sphere.z < -1000.0 or sphere.z > 1000.0) isDestroy = true;
}
Bullet Bullet::getCopy(Sphere newSphere, GLfloat newAngle){
    return Bullet(newSphere, color, newAngle, speed, damage);
}
void Bullet::draw(void){
    GLfloat matAmbANdDif[] = {color[0], color[1], color[2], 1.0};
    GLfloat matSpec[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat matShine[] = {50.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbANdDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    glPushMatrix();
        // glColor3fv(color.data());
        glTranslatef(sphere.x, sphere.y, sphere.z);
        glRotatef(angle, 0.0, 1.0, 0.0);
        glutSolidSphere(sphere.radius, 16, 16);
    glPopMatrix();
}