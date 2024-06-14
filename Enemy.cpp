#include <math.h>
#include <random>
#include "Enemy.h"
#include <vector>

Enemy::Enemy(Sphere sphere, std::vector<GLfloat> color, GLfloat angle, GLfloat speed, int health, int difficulty, unsigned int texture)
    : GameObject(sphere), color(color), angle(angle), speed(speed), health(health), difficulty(difficulty), texture(texture)
{
    shootDelay = 0;
    qobjEnemy = gluNewQuadric();
}
Enemy::Enemy(){};
Enemy::~Enemy(){};
bool Enemy::move(Sphere targetLoc)
{
    float dz = targetLoc.z - sphere.z, dx = targetLoc.x - sphere.x;
    angle = atan((dx) / (float)(dz)) * 180 / M_PI;
    if (dz < 0)
        angle += 180.0;
    if (angle < 0)
        angle += 360.;
    if (dz * dz + dx * dx <= 30 * 30)
        stop = true;
    else
        stop = false;
    if (rand() / (double)RAND_MAX < 0.005)
        return true;
    return false;
}
std::vector<Bullet> Enemy::shoot(void)
{
    std::vector<Bullet> bullets;
    bullets.emplace_back(Sphere(sphere.x + sphere.radius * sin(angle * M_PI / 180.0),
                                sphere.y,
                                sphere.z + sphere.radius * cos(angle * M_PI / 180.0), 1.0),
                         std::vector<GLfloat>{1.0, 0.0, 0.0}, angle, 8.0, 10);
    return bullets;
}
void Enemy::update(void)
{
    if (not stop)
    {
        sphere.x += speed * sin(angle * M_PI / 180.);
        sphere.z += speed * cos(angle * M_PI / 180.);
    }
    if (shootDelay > 0)
        shootDelay--;
}
void Enemy::draw(void)
{
    glPushMatrix();
    GLfloat matAmbANdDif[] = {color[0], color[1], color[2], 1.0};
    GLfloat matSpec[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat matShine[] = {50.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbANdDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    glTranslatef(sphere.x, sphere.y, sphere.z);
    glRotatef(angle, 0.0, 1.0, 0.0);
    // glColor3fv(color.data());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    gluQuadricTexture(qobjEnemy, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture);
    gluCylinder(qobjEnemy, 3, 0, sphere.radius, 16, 16);
    glTranslatef(0., 0., 5.);

    // glColor3f(0., 1.0, 0.);
    glutSolidSphere(.5, 10, 10);

    // glTranslatef(sphere.x, sphere.y, sphere.z);
    // glRotatef(angle, 0.0, 1.0, 0.0);
    // glutSolidSphere(sphere.radius, 16, 16);
    glPopMatrix();
}