#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "Player.h"
#include "Sphere.h"
#include "Planet.h"
#include <iostream>
#include <math.h>
using namespace std;

class Game{
private:
    int width, height;
    Player player;
    Planet planet;
    bool keyState[256];
public:
    int frameCount;
    Game(int w, int h): width(w), height(h){
        player = Player(0.0, 2., 5., 10, 10, Sphere(0., 0., -10., 5.));
        planet = Planet(30., 1., .5, 12., 12, Sphere(0., 0., 0., 5.0));
        memset(keyState, false, sizeof(bool)*sizeof(keyState));
        frameCount = 0;
    }
    void render(void){
        frameCount++;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0., 0., 0., 0.);
        
        playerView();
        mapView();

        
        
        glutSwapBuffers();
    }
    void playerView(void){
        glViewport(0, 0, width, height);
        glLoadIdentity();
        // glScalef()
        gluLookAt(
            player.getSphere().x - 15.*sin(player.getAngle()*M_PI/180.)
            , player.getSphere().y + 9.
            , player.getSphere().z - 15.*cos(player.getAngle()*M_PI/180.)
            , player.getSphere().x - 9.*sin(player.getAngle()*M_PI/180.)
            , player.getSphere().y + 9.
            , player.getSphere().z - 9.*cos(player.getAngle()*M_PI/180.)
            , 0.0, 1.0, 0.0);
        updateScene();
    }
    void mapView(void){
        glViewport(width*3/4, height*3/4, width*1/4, height*1/4);
        glLoadIdentity();
        // Draw a vertical line on the left of the viewport to separate the two viewports
        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
            glVertex3f(-5.0, -5.0, -5.0);
            glVertex3f(-5.0, 5.0, -5.0);
            glVertex3f(5.0, 5.0, -5.0);
            glVertex3f(5.0, -5.0, -5.0);
        glEnd();
        glLineWidth(1.0);

        gluLookAt(0., 100., 0., 0., 0., 0., 0., 0., -1.);
        updateScene();
    }
    void setup(void)
    {
        glClearColor(1.0, 1.0, 1.0, 0.0);
        glEnable(GL_DEPTH_TEST); // Enable depth testing.
    }
    // OpenGL window reshape routine.
    void resize(int w, int h)
    {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);

        glMatrixMode(GL_MODELVIEW);
        width = w, height = h;
    }
    void updateScene(void){
        player.move(keyState);
        planet.update();
        player.draw();
        planet.draw();
        glPushMatrix();
            glColor3f(1.0, 1.0, 0.0);
            glutSolidSphere(5.0, 16, 16);
        glPopMatrix();
    }
    // Keyboard input processing routine.
    void keyInput(unsigned char key, int x, int y)
    {
        keyState[key] = true;
    }
    void keyUpInput(unsigned char key, int x, int y)
    {
        keyState[key] = false;
    }
    // Callback routine for non-ASCII key entry.
    void specialKeyInput(int key, int x, int y)
    {
        glutPostRedisplay();
    }

};