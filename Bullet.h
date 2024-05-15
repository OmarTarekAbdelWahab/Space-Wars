#include "GameObject.h"
#ifndef BULLET_H_
#define BULLET_H_
#include "vector"

class Bullet: public GameObject{
private:
    GLfloat angle, speed;
    int damage;
    std::vector<GLfloat> color;
    bool isDestroy;
public:
    Bullet(Sphere sphere, std::vector<GLfloat> color, GLfloat angle, GLfloat speed, int damage);
    ~Bullet();
    void update(void) override;
    void draw(void) override;
    bool destroyBullet(void);
    int getDamage(void);
};

#endif