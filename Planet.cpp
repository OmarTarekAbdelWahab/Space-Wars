#include <iostream>
#include "Planet.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 

using namespace std;
Planet::~Planet(){
    cout << "Destroyed!!!" << id << endl;
}
Planet::Planet(int a){
    id = a;
    cout << "Created!!!" << id << endl;
        
}
void Planet::draw(int dist){
    glPushMatrix();
        glTranslatef(0.0, 0.0, -900.0 + dist);
        glTranslatef(-800.0, 0.0, 0.0);
        glColor3f(1., 0., 0.);
        for(int i = 0; i < 8; i++){
            glTranslatef(150.0, 0.0, 0.0);
            glutSolidSphere(50., 40., 40.);
        }
        glTranslatef(200.0, 0.0, 0.0);
        glColor3f(1.0, 1.0, 0.0);
        glutSolidSphere(100., 40., 40.);
        glTranslatef(150.0, 0.0, 0.0);
        glColor3f(0.0, 0.0, 1.0);
        glutSolidSphere(20., 40., 40.);
        // glutSolidSphere(50., 40, 40);
    glPopMatrix();
    }