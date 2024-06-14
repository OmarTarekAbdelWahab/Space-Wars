#include "Player.h"
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "PowerUp.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;
Player::Player(){}
Player::Player(GLfloat angle, GLfloat speed, int health, int score, int list, Sphere sphere) 
    : angle(angle), speed(speed), health(health), score(score), list(list), GameObject(sphere), maxHealth(health){
        shootDelay = 0;
        weapon = Weapon(0);
        speedBoost = 1.0;
        speedBoostTimer = 0;
    }
void Player::applyPowerUp(PowerUp powerUp){
    if(powerUp.getType() == HEALTH) increaseHealth(powerUp.getHealthIncrease());
    else if(powerUp.getType() == SPEED){
        speedBoost = powerUp.getSpeedBoost();
        speedBoostTimer = 60*6;
    }else if(powerUp.getType() == WEAPON) upgradeWeapon();
}
void Player::increaseHealth(int amount){
    health = min(amount + health, maxHealth);
}
bool Player::move(bool key[]){
    if(key['d']) angle -= 5.;
    if(key['a']) angle += 5.;
    if(key['w']){
        sphere.x += speedBoost*speed*sin(angle*M_PI/180.);
        sphere.z += speedBoost*speed*cos(angle*M_PI/180.);
    } 
    if(key['s']){
        sphere.x -= speedBoost*speed*sin(angle*M_PI/180.);
        sphere.z -= speedBoost*speed*cos(angle*M_PI/180.);
    }
    if(speedBoostTimer > 0){
        speedBoostTimer--;
        if(speedBoostTimer == 0) speedBoost = 1;
    }
    if (key['s'])
    {
        sphere.x -= speed * sin(angle * M_PI / 180.);
        sphere.z -= speed * cos(angle * M_PI / 180.);
    }
    if (angle < 0)
        angle += 360.;
    if (angle > 360.)
        angle -= 360.;
    if (key[' '] and shootDelay == 0)
        return true;
    return false;
}
void Player::takeDamage(int damage)
{
    health = max(0, health - damage);
    // if health < 0 -> die
}
int Player::getHealth()
{
    return health;
}
void Player::upgradeWeapon(){
    weapon.upgrade();
}
vector<Bullet> Player::shoot()
{
    if(this->getHealth() > 0){
        PlaySound(TEXT("sounds/bullet.wav"), NULL, SND_FILENAME | SND_ASYNC);
    }
    vector<Bullet> bullets;
    shootDelay = 30;
    bullets = weapon.getBullets(Sphere(sphere.x+sphere.radius*sin(angle*M_PI/180.0), 
                                sphere.y,
                                sphere.z+sphere.radius*cos(angle*M_PI/180.0), 1.0), angle);
    return bullets;
}
void Player::update(){
    if(shootDelay > 0)  shootDelay = max(0, shootDelay - (int)speedBoost);
}
GLfloat Player::getAngle()
{
    return angle;
}
void Player::draw()
{
    glPushMatrix();

        GLfloat matAmbANdDif[] = {1.0, 1.0, 0.0, 1.0};
        GLfloat matSpec[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat matShine[] = {50.0};

        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbANdDif);
        glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
        glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

        glTranslatef(sphere.x, sphere.y, sphere.z);
        glRotatef(angle, 0.0, 1.0, 0.0);
        // glColor3f(0.0, 0.0, 1.0);
        glutWireCone(3, sphere.radius, 16, 16);
        glTranslatef(0., 0., 5.);
        // glColor3f(0., 1.0, 0.);
        
        glutSolidSphere(.5, 10, 10);
    glPopMatrix();
}
void Player::drawHealthBar(void){
    glDisable(GL_LIGHTING);
    // draw health bar:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if(speedBoostTimer > 0){
        glPushMatrix();
            glTranslatef(0.0, 0.0, -5.0);
            glColor3f(0.0, 0.0, 1.0);
            glBegin(GL_POLYGON);
                glVertex3f(0.0, 4.75, 0.0);
                glVertex3f(0.0, 4.25, 0.0);
                glVertex3f(1.0, 4.25, 0.0);
                glVertex3f(1.0, 4.75, 0.0);
            glEnd();
            glBegin(GL_POLYGON);
                glVertex3f(0.25, 5.0, 0.0);
                glVertex3f(0.25, 4.0, 0.0);
                glVertex3f(0.75, 4.0, 0.0);
                glVertex3f(0.75, 5.0, 0.0);
            glEnd();
        glPopMatrix();
    }
    glPushMatrix();
    glTranslatef(0.0, 0.0, -5.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-5.0, 5.0, 0.0);
    glVertex3f(-5.0, 4.5, 0.0);
    glVertex3f(-5 + 4 * (health / (GLfloat)maxHealth), 4.5, 0.0);
    glVertex3f(-5 + 4 * (health / (GLfloat)maxHealth), 5.0, 0.0);
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-5.0, 5.0, 0.0);
    glVertex3f(-5.0, 4.5, 0.0);
    glVertex3f(-1.0, 4.5, 0.0);
    glVertex3f(-1.0, 5.0, 0.0);
    glEnd();
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
    glEnable(GL_LIGHTING);
}