#include <math.h>
#include <random>
#include "Enemy.h"
#include <vector>

Enemy::Enemy(Sphere sphere, std::vector<GLfloat> color, GLfloat angle, GLfloat speed, int health, int difficulty)
: GameObject(sphere), color(color), angle(angle), speed(speed), health(health), difficulty(difficulty){
    shootDelay = 0;
}
Enemy::Enemy(){};
Enemy::~Enemy(){};
bool Enemy::move(Sphere targetLoc){
    float dz = targetLoc.z - sphere.z, dx = targetLoc.x - sphere.x;
    angle = atan((dx) / (float)(dz))*180/M_PI;
    if(dz < 0) angle += 180.0;
    if(angle < 0) angle += 360.;
    if(rand()/(double)RAND_MAX < 0.005) return true;
    return false;
}
std::vector<Bullet> Enemy::shoot(void){
    std::vector<Bullet> bullets;
    bullets.emplace_back(Sphere(sphere.x+sphere.radius*sin(angle*M_PI/180.0), 
                            sphere.y,
                            sphere.z+sphere.radius*cos(angle*M_PI/180.0), 1.0), std::vector<GLfloat>{1.0, 0.0, 0.0}, angle, 8.0, 10);
    return bullets;
}
void Enemy::update(void){
    sphere.x += speed*sin(angle*M_PI/180.);
    sphere.z += speed*cos(angle*M_PI/180.);
    if(shootDelay > 0) shootDelay--;
}
void Enemy::draw(void){
    glPushMatrix();
        glTranslatef(sphere.x, sphere.y, sphere.z);
        glRotatef(angle, 0.0, 1.0, 0.0);
        glColor3fv(color.data());
        glutWireCone(3, sphere.radius, 16, 16);
        glTranslatef(0., 0., 5.);
        glColor3f(0., 1.0, 0.);
        glutWireSphere(.5, 10, 10);


        // glTranslatef(sphere.x, sphere.y, sphere.z);
        // glRotatef(angle, 0.0, 1.0, 0.0);
        // glutSolidSphere(sphere.radius, 16, 16);
    glPopMatrix();
}