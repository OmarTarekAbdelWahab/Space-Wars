#include "Weapon.h"
#include <math.h>
using namespace std;


Weapon::~Weapon(){}
Weapon::Weapon(){}
Weapon::Weapon(int weaponInd):weaponInd(weaponInd){
    bullets.emplace_back(Sphere(0.0, 0.0, 0.0, 0.0), vector<GLfloat>{0.0, 0.0, 1.0}, 0.0, 8.0, 100);
    bullets.emplace_back(Sphere(0.0, 0.0, 0.0, 0.0), vector<GLfloat>{1.0, 0.0, 0.0}, 0.0, 12.0, 140);
};

vector<Bullet> Weapon::getBullets(Sphere pos, GLfloat angle){
    if(weaponInd == 0)
        return {bullets[weaponInd].getCopy(pos, angle)};
    GLfloat tempAngle = angle - 90.0;
    if(tempAngle < 0) tempAngle += 360.0;
    vector<Bullet> shootBullets;
    shootBullets.push_back(bullets[weaponInd].getCopy(
        Sphere(pos.x + 3*sin(tempAngle*M_PI/180.), pos.y, pos.z + 3*cos(tempAngle*M_PI/180.0), pos.radius), angle
    ));
    tempAngle = angle + 90.0;
    if(tempAngle >= 360.0) tempAngle -= 360.0;
    shootBullets.push_back(bullets[weaponInd].getCopy(
        Sphere(pos.x + 3*sin(tempAngle*M_PI/180.), pos.y, pos.z + 3*cos(tempAngle*M_PI/180.0), pos.radius), angle
    ));
    return shootBullets;
}
void Weapon::upgrade(){
    weaponInd = (weaponInd + 1)%bullets.size();
}