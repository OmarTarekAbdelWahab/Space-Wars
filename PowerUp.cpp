#include "PowerUp.h"
#include <math.h>
#include <iostream>
using namespace std;

PowerUp::PowerUp(Sphere sphere, PowerType type): GameObject(sphere), type(type){
    if(type == HEALTH){
        color = {1.0, 0.0, 0.0};
    }else if(type == WEAPON){
        color = {0.0, 1.0, 0.0};
    }else if(type == SPEED){
        color = {0.0, 0.0, 1.0};
    }
    sphere.y = 0;
    angle = 0;
    heightAngle = 0;
    edgeLength = 10.0;
    sphere.radius = edgeLength;
    speedBoost = 2.0;
    healthIncrease = 50;
    // cout << "Spawned at " << sphere.x << ' ' << sphere.z << endl;
}
PowerUp::PowerUp(){}
PowerUp::~PowerUp(){}
int PowerUp::getHealthIncrease(){return healthIncrease;}
GLfloat PowerUp::getSpeedBoost(){return speedBoost;}
PowerType PowerUp::getType(){return type;}
void PowerUp::update(){
    angle += 1.0;
    if(angle >= 360.0) angle -= 360.0;
    heightAngle += 2;
    if(heightAngle >= 360.0) heightAngle -= 360.0;
    sphere.y = 7.0*sin(heightAngle*M_PI/180.0);
}

void PowerUp::draw(){
    glPushMatrix();

        GLfloat matAmbANdDif[] = {color[0], color[1], color[2], 1.0};
        GLfloat matSpec[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat matShine[] = {50.0};

        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbANdDif);
        glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
        glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

        glTranslatef(sphere.x, sphere.y, sphere.z);
        glRotatef(angle, 0.0, 1.0, 0.0);
        glutSolidCube(edgeLength);
    glPopMatrix();
}