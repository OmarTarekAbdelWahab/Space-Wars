#include <GL/glew.h>
#include <GL/freeglut.h>
#include <chrono>
#include "Player.h"
#include "Sphere.h"
#include "Planet.h"
#include "SolarSystem.h"
#include <iostream>
#include <cmath>
#include "Enemy.h"
#include "GameMode.h"
#include "getBMP.h"

#define SURVIVAL 0
#define TIMER 1

#define MENU 0
#define GAME 1

using namespace std;

class Game
{
private:
    int width, height;
    Player player;
    // Planet planet;
    GameMode game_mode;
    bool keyState[256];
    SolarSystem solarSystem;
    vector<Bullet> bullets;
    vector<Enemy> enemies;

public:
    int frameCount;
    unsigned int texture[16];
    Game(int w, int h) : width(w), height(h)
    {
        game_mode = GameMode();
        // planet = Planet({0}, 30., 1., .5, 12., 12, Sphere(0., 0., 0., 5.0));
        memset(keyState, false, sizeof(bool) * sizeof(keyState));
        frameCount = 0;

        // should take it as an input
        game_mode.chooseMode(SURVIVAL);
    }

    void render(void)
    {
        frameCount++;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0., 0., 0., 0.);
        handleCollision();
        if (player.move(keyState))
        {
            vector<Bullet> newBullets = player.shoot();
            for (Bullet &bullet : newBullets)
                bullets.emplace_back(bullet);
        }
        for (Enemy &enemy : enemies)
        {
            if (enemy.move(player.getSphere()))
            {
                vector<Bullet> newBullets = enemy.shoot();
                for (Bullet &bullet : newBullets)
                    bullets.emplace_back(bullet);
            }
            enemy.update();
        }
        for (int i = 0; i < bullets.size(); i++)
        {
            bullets[i].update();
            if (bullets[i].destroyBullet())
            {
                bullets.erase(bullets.begin() + i);
                i--;
            }
        }
        // cout << "Bullets Size: " << bullets.size() << endl;
        // cout << "Enemy Size: " << enemies.size() << endl;
        // cout << player.getSphere().x << ' ' << player.getSphere().z << ' ' << player.getAngle() << endl;
        game_mode.update(player, enemies, texture[17]);
        solarSystem.update();
        player.update();
        playerView();
        mapView();

        glutSwapBuffers();
    }
    void playerView(void)
    {
        glViewport(0, 0, width, height);
        glLoadIdentity();
        game_mode.draw();
        player.drawHealthBar();
        // glScalef()
        gluLookAt(
            player.getSphere().x - 15. * sin(player.getAngle() * M_PI / 180.), player.getSphere().y + 9., player.getSphere().z - 15. * cos(player.getAngle() * M_PI / 180.), player.getSphere().x - 9. * sin(player.getAngle() * M_PI / 180.), player.getSphere().y + 9., player.getSphere().z - 9. * cos(player.getAngle() * M_PI / 180.), 0.0, 1.0, 0.0);
        updateScene(true);
    }
    void mapView(void)
    {

        glViewport(width * 3 / 4, height * 3 / 4, width * 1 / 4, height * 1 / 4);
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
        updateScene(false);
    }
    void loadTextures()
    {
        imageFile *image[18];
        image[0] = getBMP("bmp texture/2k_sun.bmp");
        image[1] = getBMP("bmp texture/2k_mercury.bmp");
        image[2] = getBMP("bmp texture/2k_venus_surface.bmp");
        image[3] = getBMP("bmp texture/2k_earth_daymap.bmp");
        image[4] = getBMP("bmp texture/2k_mars.bmp");
        image[5] = getBMP("bmp texture/2k_jupiter.bmp");
        image[6] = getBMP("bmp texture/2k_saturn.bmp");
        image[7] = getBMP("bmp texture/2k_uranus.bmp");
        image[8] = getBMP("bmp texture/2k_neptune.bmp");
        image[9] = getBMP("bmp texture/2k_moon.bmp");
        image[10] = getBMP("bmp texture/2k_saturn_ring_alpha.bmp");
        image[11] = getBMP("bmp texture/2k_stars_milky_way.bmp");
        image[12] = getBMP("bmp texture/2k_venus_atmosphere.bmp");
        image[13] = getBMP("bmp texture/sky1.bmp");
        image[14] = getBMP("bmp texture/sky2.bmp");
        image[15] = getBMP("bmp texture/sky3.bmp");
        image[16] = getBMP("bmp texture/bodyMetal.bmp");
        image[17] = getBMP("bmp texture/Red metal texture background.bmp");

        for (int i = 0; i < 18; i++)
        {
            glBindTexture(GL_TEXTURE_2D, texture[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[i]->width, image[i]->height, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, image[i]->data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
    }
    void setup(void)
    {
        glClearColor(1.0, 1.0, 1.0, 0.0);
        glEnable(GL_DEPTH_TEST); // Enable depth testing.
        glGenTextures(18, texture);
        loadTextures();
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glEnable(GL_TEXTURE_2D);
        solarSystem.populate(texture);
        player = Player(0.0, 2., 100, 100, 10, Sphere(0., 0.0, -500., 5.), texture[16]);
    }
    // OpenGL window reshape routine.
    void resize(int w, int h)
    {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 8400.0);

        glMatrixMode(GL_MODELVIEW);
        width = w, height = h;
    }

    void updateScene(bool isPlayer)
    {
        player.draw();
        solarSystem.draw();
        for (Enemy &enemy : enemies)
            enemy.draw();
        for (Bullet &bullet : bullets)
            bullet.draw();
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glutSolidSphere(5.0, 16, 16);

        if (isPlayer)
        {
            glBindTexture(GL_TEXTURE_2D, texture[15]);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(-3000.0, -100.0, 3000.0);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(3000.0, -100.0, 3000.0);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(3000.0, -100.0, -3000.0);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(-3000.0, -100.0, -3000.0);
            glEnd();

            glBindTexture(GL_TEXTURE_2D, texture[15]);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(-3000.0, 2100.0, 3000.0);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(3000.0, 2100.0, 3000.0);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(3000.0, 2100.0, -3000.0);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(-3000.0, 2100.0, -3000.0);
            glEnd();

            glBindTexture(GL_TEXTURE_2D, texture[15]);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(-3000.0, -100.0, 3000.0);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(-3000.0, -100.0, -3000.0);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(-3000.0, 2100.0, -3000.0);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(-3000.0, 2100.0, 3000.0);
            glEnd();

            glBindTexture(GL_TEXTURE_2D, texture[15]);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(3000.0, -100.0, 3000.0);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(3000.0, 2100.0, 3000.0);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(3000.0, 2100.0, -3000.0);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(3000.0, -100.0, -3000.0);
            glEnd();

            glBindTexture(GL_TEXTURE_2D, texture[15]);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(3000.0, -100.0, 3000.0);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(-3000.0, -100.0, 3000.0);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(-3000.0, 2100.0, 3000.0);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(3000.0, 2100.0, 3000.0);
            glEnd();

            glBindTexture(GL_TEXTURE_2D, texture[15]);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(-3000.0, -100.0, -3000.0);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(3000.0, -100.0, -3000.0);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(3000.0, 2100.0, -3000.0);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(-3000.0, 2100.0, -3000.0);
            glEnd();
        }
        glPopMatrix();
    }
    void handleCollision()
    {
        Sphere playerSphere = player.getSphere();
        for (Planet &planet : solarSystem.getPlanets())
        {
            Sphere planetSphere = planet.getSphere();
            if (checkSpheresIntersection(playerSphere.x, playerSphere.y, playerSphere.z, playerSphere.radius,
                                         planetSphere.x, planetSphere.y, planetSphere.z, planetSphere.radius))
            {
                cout << "Game Over\n";
            }
        }
        for (Enemy &enemy : enemies)
        {
            Sphere enemySphere = enemy.getSphere();
            if (checkSpheresIntersection(playerSphere.x, playerSphere.y, playerSphere.z, playerSphere.radius,
                                         enemySphere.x, enemySphere.y, enemySphere.z, enemySphere.radius))
            {
                cout << "You Got Hit!!!\n";
            }
        }
        for (int bind = 0; bind < bullets.size(); bind++)
        {
            bool isHit = false;
            Sphere bulletSphere = bullets[bind].getSphere();
            for (int eind = 0; eind < enemies.size(); eind++)
            {
                Sphere enemySphere = enemies[eind].getSphere();
                if (checkSpheresIntersection(bulletSphere.x, bulletSphere.y, bulletSphere.z, bulletSphere.radius,
                                             enemySphere.x, enemySphere.y, enemySphere.z, enemySphere.radius))
                {
                    enemies.erase(enemies.begin() + eind);
                    isHit = true;
                    break;
                }
            }
            if (isHit)
            {
                bullets.erase(bullets.begin() + bind);
                bind--;
                continue;
            }
            if (checkSpheresIntersection(bulletSphere.x, bulletSphere.y, bulletSphere.z, bulletSphere.radius,
                                         playerSphere.x, playerSphere.y, playerSphere.z, playerSphere.radius))
            {
                player.takeDamage(bullets[bind].getDamage());
                isHit = true;
            }
            if (isHit)
            {
                bullets.erase(bullets.begin() + bind);
                bind--;
            }
        }
        for (int eind = 0; eind < enemies.size(); eind++)
        {
            bool isHit = false;
            Sphere enemySphere = enemies[eind].getSphere();
            for (int bind = 0; bind < bullets.size(); bind++)
            {
                Sphere bulletSphere = bullets[bind].getSphere();
                if (checkSpheresIntersection(bulletSphere.x, bulletSphere.y, bulletSphere.z, bulletSphere.radius,
                                             enemySphere.x, enemySphere.y, enemySphere.z, enemySphere.radius))
                {
                    bullets.erase(bullets.begin() + bind);
                    isHit = true;
                    break;
                }
            }
            if (isHit)
            {
                enemies.erase(enemies.begin() + eind);
                eind--;
            }
        }
    }
    int checkSpheresIntersection(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat r1,
                                 GLfloat x2, GLfloat y2, GLfloat z2, GLfloat r2)
    {
        return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2) <= (r1 + r2) * (r1 + r2));
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
    void mouseInput(int button, int state, int x, int y)
    {
        cout << "Mouse Clicked at: " << x << ' ' << y << endl;
        cout << "Game Mode: " << game_mode.getDisplay() << endl;
        // if(game_mode.getDisplay() == MENU){
        // if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // if (x >= 10 && x <= 100 && y >= 10 && y <= 30) {
        // Option 1 was clicked
        // cout << "Option 1 was clicked" << endl;
        // game_mode.chooseMode(SURVIVAL);
        // } else if (x >= 10 && x <= 100 && y >= 30 && y <= 50) {
        //     // Option 2 was clicked
        //     // isOption1 = false;
        //     cout << "Option 2 was clicked" << endl;
        //     game_mode.chooseMode(TIMER);
        // }
        // else{
        //     cout << "Mouse Clicked at: " << x << ' ' << y << endl;
        // }
        // }
        // glutPostRedisplay(); // Request to redraw the screen with the new option
        // }
    }
};