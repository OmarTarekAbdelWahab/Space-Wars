#include "Player.h"
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h> 

using namespace std;
Player::Player(){}
Player::Player(GLfloat angle, GLfloat speed, int health, int score, int list, Sphere sphere) 
    : angle(angle), speed(speed), health(health), score(score), list(list), GameObject(sphere){}

void Player::increaseHealth(int amount){
    health += amount;
}
void Player::move(bool key[]){
    if(key['d']) angle -= 5.;
    if(key['a']) angle += 5.;
    if(key['w']){
        sphere.x += speed*sin(angle*M_PI/180.);
        sphere.z += speed*cos(angle*M_PI/180.);
    } 
    if(key['s']){
        sphere.x -= speed*sin(angle*M_PI/180.);
        sphere.z -= speed*cos(angle*M_PI/180.);
    }
    if(angle < 0) angle += 360.;
    if(angle > 360.) angle -= 360.;
}
void Player::takeDamage(int damage){
    health -= damage;
    // if health < 0 -> die
}
void Player::update(){
    // todo
}
GLfloat Player::getAngle(){
    return angle;
}
void Player::draw(){
    glPushMatrix();
        glTranslatef(sphere.x, sphere.y, sphere.z);
        glRotatef(angle, 0.0, 1.0, 0.0);
        glColor3f(1.0, 0.0, 0.0);
        glutWireCone(3, sphere.radius, 16, 16);
        glTranslatef(0., 0., 5.);
        glColor3f(0., 1.0, 0.);
        glutWireSphere(.5, 10, 10);
    glPopMatrix();
}