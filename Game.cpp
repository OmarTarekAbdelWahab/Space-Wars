#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <chrono>
#include "Player.h"
#include "Sphere.h"
#include "Planet.h"
#include "SolarSystem.h"
#include <iostream>
#include <math.h>
#include "Enemy.h"
using namespace std;

class Game{
private:
    int width, height;
    Player player;
    // Planet planet;
    int time;
    std::chrono::high_resolution_clock::time_point prev_time = std::chrono::high_resolution_clock::now();
    int level;
    bool isSurvival;
    bool keyState[256];
    SolarSystem solarSystem;
    vector<Bullet> bullets;
    vector<Enemy> enemies;
public:
    int frameCount;
    Game(int w, int h): width(w), height(h){
        player = Player(0.0, 2., 100, 100, 10, Sphere(0., 0.0, -500., 5.));
        // planet = Planet({0}, 30., 1., .5, 12., 12, Sphere(0., 0., 0., 5.0));
        solarSystem.populate();
        memset(keyState, false, sizeof(bool)*sizeof(keyState));
        frameCount = 0;
        isSurvival = false;
        level = 0;
        time = 510;
        // updateGame(isSurvival);
        // enemies.emplace_back(Sphere(40.0, 0.0, 40.0, 5.0), vector<GLfloat>{1.0, 1.0, 0.0}, 30.0, 1.0, 100, 1);
        // enemies.emplace_back(Sphere(-40.0, 0.0, -40.0, 5.0), vector<GLfloat>{1.0, 0.0, 1.0}, 130.0, 1.0, 100, 1);
        // enemies.emplace_back(Sphere(40.0, 0.0, -40.0, 5.0), vector<GLfloat>{1.0, 1.0, 1.0}, 230.0, 1.0, 100, 1);
    }
    void updateGame(bool isSurvival){
        if(isSurvival){
            if(player.getHealth() <= 0){
                renderText(-4.5, 4.5, "Game Over!!!");
                return;
            }
            if(enemies.size() == 0){
                level++;
                for(int i = 0; i < level*2; i++){
                GLfloat x = rand()%1000 - 500;
                GLfloat z = rand()%1000 - 500;
                GLfloat angle = rand()%360;
                enemies.emplace_back(Sphere(x, 0.0, z, 5.0), vector<GLfloat>{1.0, 1.0, 0.0}, angle, 1.0, 100, 1);
                }
            }
            renderText(-4.5, 4.5, "Level: " + to_string(level));
        }
        else{
            renderText(-4.5, 4.5, "Time Left: " + to_string(time));
            auto now_time = std::chrono::high_resolution_clock::now();
            if(now_time - prev_time >= std::chrono::seconds(1)){
                prev_time = now_time;
                time--;
                if(time == 0){
                    renderText(-4.5, 4.5, "Game Over!!!");
                    return;
                }
                else if(time%100 == 0){
                    level++;
                }
                else if(time%10 == 0){
                    for(int i = 0; i < level; i++){
                        GLfloat x = rand()%1000 - 500;
                        GLfloat z = rand()%1000 - 500;
                        GLfloat angle = rand()%360;
                        enemies.emplace_back(Sphere(x, 0.0, z, 5.0), vector<GLfloat>{1.0, 1.0, 0.0}, angle, 1.0, 100, 1);
                    }
                }
                if(enemies.size() == 0){
                    renderText(-4.5, 4.5, "You Win!!!");
                return;
                }
            }
                
            
            
        }
    }

    void renderText(float x, float y, std::string text) {
        glRasterPos2f(x, y);
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
        }
    }

    void render(void){
        frameCount++;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0., 0., 0., 0.);
        handleCollision();
        if(player.move(keyState)){
            vector<Bullet> newBullets = player.shoot();
            for(Bullet& bullet: newBullets) bullets.emplace_back(bullet);
        }
        for(Enemy& enemy: enemies) {
            if(enemy.move(player.getSphere())){
                vector<Bullet> newBullets = enemy.shoot();
                for(Bullet& bullet: newBullets) bullets.emplace_back(bullet);
            }
            enemy.update();
        }
        for(int i = 0; i < bullets.size(); i++){
            bullets[i].update();
            if(bullets[i].destroyBullet()){
                bullets.erase(bullets.begin()+i);
                i--;
            }
        } 
        // cout << "Bullets Size: " << bullets.size() << endl;
        // cout << "Enemy Size: " << enemies.size() << endl;
        // cout << player.getSphere().x << ' ' << player.getSphere().z << ' ' << player.getAngle() << endl;
        updateGame(isSurvival);
        solarSystem.update();
        player.update();
        playerView();
        mapView();

        
        
        glutSwapBuffers();
    }
    void playerView(void){
        glViewport(0, 0, width, height);
        glLoadIdentity();
        player.drawHealthBar();
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

        gluLookAt(0.0, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
        // gluLookAt(player.getSphere().x, 100., player.getSphere().z, player.getSphere().x, 0.0, player.getSphere().z, 0.0, 0.0, 1.0);
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
        glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 1000.0);

        glMatrixMode(GL_MODELVIEW);
        width = w, height = h;
    }
    void updateScene(void){
        player.draw();
        solarSystem.draw();
        for(Enemy& enemy: enemies) enemy.draw();
        for(Bullet& bullet: bullets) bullet.draw();
        glPushMatrix();
            glColor3f(1.0, 1.0, 0.0);
            glutSolidSphere(5.0, 16, 16);
        glPopMatrix();
    }
    void handleCollision(){
        Sphere playerSphere = player.getSphere();
        for(Planet& planet: solarSystem.getPlanets()){
            Sphere planetSphere = planet.getSphere();
            if(checkSpheresIntersection(playerSphere.x, playerSphere.y, playerSphere.z, playerSphere.radius,
                planetSphere.x, planetSphere.y, planetSphere.z, planetSphere.radius)){
                    cout << "Game Over\n";
                }
        }
        for(Enemy& enemy: enemies){
            Sphere enemySphere = enemy.getSphere();
            if(checkSpheresIntersection(playerSphere.x, playerSphere.y, playerSphere.z, playerSphere.radius,
                enemySphere.x, enemySphere.y, enemySphere.z, enemySphere.radius)){
                    cout << "You Got Hit!!!\n";
                }
        }
        for(int bind = 0; bind < bullets.size(); bind++){
            bool isHit = false;
            Sphere bulletSphere = bullets[bind].getSphere();
            for(int eind = 0; eind < enemies.size(); eind++){
                Sphere enemySphere = enemies[eind].getSphere();
                if(checkSpheresIntersection(bulletSphere.x, bulletSphere.y, bulletSphere.z, bulletSphere.radius,
                enemySphere.x, enemySphere.y, enemySphere.z, enemySphere.radius)){
                    enemies.erase(enemies.begin()+eind);
                    isHit = true;
                    break;
                }
            }
            if(isHit){
                bullets.erase(bullets.begin()+bind);
                bind--;
                continue;
            }
            if(checkSpheresIntersection(bulletSphere.x, bulletSphere.y, bulletSphere.z, bulletSphere.radius,
                playerSphere.x, playerSphere.y, playerSphere.z, playerSphere.radius)){
                    player.takeDamage(bullets[bind].getDamage());
                    isHit = true;
            }
            if(isHit){
                bullets.erase(bullets.begin()+bind);
                bind--;
            }
        }
        for(int eind = 0; eind < enemies.size(); eind++){
            bool isHit = false;
            Sphere enemySphere = enemies[eind].getSphere();
            for(int bind = 0; bind < bullets.size(); bind++){
                Sphere bulletSphere = bullets[bind].getSphere();
                if(checkSpheresIntersection(bulletSphere.x, bulletSphere.y, bulletSphere.z, bulletSphere.radius,
                enemySphere.x, enemySphere.y, enemySphere.z, enemySphere.radius)){
                    bullets.erase(bullets.begin()+bind);
                    isHit = true;
                    break;
                }
            }
            if(isHit){
                enemies.erase(enemies.begin()+eind);
                eind--;
            }
        }
    }
    int checkSpheresIntersection(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat r1,
	GLfloat x2, GLfloat y2, GLfloat z2, GLfloat r2)
    {
        return ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) <= (r1 + r2)*(r1 + r2));
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