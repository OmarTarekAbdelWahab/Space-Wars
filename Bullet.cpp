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
void Bullet::draw(void){
    glPushMatrix();
        glColor3fv(color.data());
        glTranslatef(sphere.x, sphere.y, sphere.z);
        glRotatef(angle, 0.0, 1.0, 0.0);
        glutWireSphere(sphere.radius, 16, 16);
    glPopMatrix();
}