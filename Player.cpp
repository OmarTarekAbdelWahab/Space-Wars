#include "Player.h"
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <windows.h>
#include <mmsystem.h>

using namespace std;
Player::Player() {}
Player::Player(GLfloat angle, GLfloat speed, int health, int score, int list, Sphere sphere)
    : angle(angle), speed(speed), health(health), score(score), list(list), GameObject(sphere), maxHealth(health)
{
    upgrade = 0;
    shootDelay = 0;
}

void Player::increaseHealth(int amount)
{
    health += amount;
}
bool Player::move(bool key[])
{
    if (key['d'])
        angle -= 5.;
    if (key['a'])
        angle += 5.;
    if (key['w'])
    {
        sphere.x += speed * sin(angle * M_PI / 180.);
        sphere.z += speed * cos(angle * M_PI / 180.);
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
void Player::upgradeWeapon()
{
    upgrade = 30;
}
vector<Bullet> Player::shoot()
{
    vector<Bullet> bullets;
    shootDelay = 15;
    if (upgrade > 0)
    {
        // power up
    }
    else
    {
        sndPlaySound(TEXT("Audio-Files/bullet.wav"), SND_ASYNC);
        bullets.emplace_back(Sphere(sphere.x + sphere.radius * sin(angle * M_PI / 180.0),
                                    sphere.y,
                                    sphere.z + sphere.radius * cos(angle * M_PI / 180.0), 1.0),
                             vector<GLfloat>{0.0, 0.0, 1.0}, angle, 8.0, 100);
    }
    return bullets;
}
void Player::update()
{
    if (upgrade > 0)
        upgrade--;
    if (shootDelay > 0)
        shootDelay--;
}
GLfloat Player::getAngle()
{
    return angle;
}
void Player::draw()
{
    glPushMatrix();
    glTranslatef(sphere.x, sphere.y, sphere.z);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    glutWireCone(3, sphere.radius, 16, 16);
    glTranslatef(0., 0., 5.);
    glColor3f(0., 1.0, 0.);
    glutWireSphere(.5, 10, 10);
    glPopMatrix();
}
void Player::drawHealthBar(void)
{
    // draw health bar:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
}