#include <GL/glew.h>
#include <GL/freeglut.h>
#include "PowerUp.h"
#include <chrono>
#include "Player.h"
#include "Sphere.h"
#include "Planet.h"
#include "SolarSystem.h"
#include <iostream>
#include <math.h>
#include "Enemy.h"
#include <random>
#include "GameMode.h"
#include <windows.h>
#include <mmsystem.h>
#include <thread>
#include "getBMP.h"
#pragma comment(lib, "winmm.lib")

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
    vector<Bullet> enemyBullets;
    vector<Bullet> playerBullets;
    vector<Enemy> enemies;
    vector<PowerUp> powerUps;
    chrono::system_clock::time_point prev_time = std::chrono::high_resolution_clock::now();

public:
    int frameCount;
    unsigned int texture[18];
    Game(int w, int h) : width(w), height(h)
    {
        this->player = Player(0.0, 2., 100, 100, 10, Sphere(0., 0.0, -500., 5.));
        this->game_mode = GameMode();
        memset(keyState, false, sizeof(bool) * sizeof(keyState));
        frameCount = 0;
        prev_time = std::chrono::high_resolution_clock::now();
    }

    void restart(int mode)
    {
        this->player = Player(0.0, 2., 100, 100, 10, Sphere(0., 0.0, -500., 5.));
        memset(keyState, false, sizeof(bool) * sizeof(keyState));
        frameCount = 0;
        playerBullets.clear();
        enemyBullets.clear();
        enemies.clear();
        powerUps.clear();
        this->game_mode.chooseMode(mode);
    }

    void render(void)
    {
        frameCount++;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0., 0., 0., 0.);

        float lightAmb[] = {0.0, 0.0, 0.0, 1.0};
        float lightDifAndSpec[] = {1.0, 1.0, 1.0, 1.0};
        float lightPos[] = {0.0, 100.0, 0.0, 1.0};
        float globAmb[] = {0.8, 0.8, 0.8, 1.0};
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

        glEnable(GL_LIGHT0);

        // glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, t);

        handleCollision();
        if (player.move(keyState))
        {
            vector<Bullet> newBullets = player.shoot();
            for (Bullet &bullet : newBullets)
                playerBullets.emplace_back(bullet);
        }
        for (Enemy &enemy : enemies)
        {
            if (enemy.move(player.getSphere()))
            {
                vector<Bullet> newBullets = enemy.shoot();
                for (Bullet &bullet : newBullets)
                    enemyBullets.emplace_back(bullet);
            }
            enemy.update();
        }
        for (int i = 0; i < enemyBullets.size(); i++)
        {
            enemyBullets[i].update();
            if (enemyBullets[i].destroyBullet())
            {
                enemyBullets.erase(enemyBullets.begin() + i);
                i--;
            }
        }
        for (int i = 0; i < playerBullets.size(); i++)
        {
            playerBullets[i].update();
            if (playerBullets[i].destroyBullet())
            {
                playerBullets.erase(playerBullets.begin() + i);
                i--;
            }
        }
        game_mode.update(player, enemies);
        solarSystem.update();
        player.update();
        for (PowerUp &power : powerUps)
            power.update();
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
        updateScene();
    }
    void mapView(void)
    {
        glViewport(width * 2 / 3, height * 2 / 3, width * 1 / 3, height * 1 / 3);
        glLoadIdentity();
        // Draw a vertical line on the left of the viewport to separate the two viewports
        glLineWidth(2.0);
        glDisable(GL_LIGHTING);
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
        glVertex3f(-5.0, -5.0, -5.0);
        glVertex3f(-5.0, 5.0, -5.0);
        glVertex3f(5.0, 5.0, -5.0);
        glVertex3f(5.0, -5.0, -5.0);
        glEnd();
        glEnable(GL_LIGHTING);
        glLineWidth(1.0);

        gluLookAt(0.0, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
        // gluLookAt(player.getSphere().x, 100., player.getSphere().z, player.getSphere().x, 0.0, player.getSphere().z, 0.0, 0.0, 1.0);
        updateScene();
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
        glEnable(GL_LIGHTING);
        glGenTextures(18, texture);
        loadTextures();
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glEnable(GL_TEXTURE_2D);

        solarSystem.populate(texture);

        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
    }
    // OpenGL window reshape routine.
    void resize(int w, int h)
    {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-5.0 - (w - 500) / 100.0, 5.0 + (w - 500) / 100.0, -5.0 - (h - 500) / 100.0, 5.0 + (h - 500) / 100.0, 5.0, 2000.0);

        glMatrixMode(GL_MODELVIEW);
        width = w, height = h;
    }
    void updateScene(void)
    {
        float lightPos[] = {0.0, 0.0, 0.0, 1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        solarSystem.draw();
        player.draw();
        for (Enemy &enemy : enemies)
            enemy.draw();
        for (Bullet &bullet : enemyBullets)
            bullet.draw();
        for (Bullet &bullet : playerBullets)
            bullet.draw();
        for (PowerUp &power : powerUps)
            power.draw();
    }
    void handleCollision()
    {
        // Collision Player & Planet
        Sphere playerSphere = player.getSphere();
        for (Planet &planet : solarSystem.getPlanets())
        {
            Sphere planetSphere = planet.getSphere();
            if (checkSpheresIntersection(playerSphere.x, playerSphere.y, playerSphere.z, playerSphere.radius,
                                         planetSphere.x, planetSphere.y, planetSphere.z, planetSphere.radius))
            {
                auto now = std::chrono::high_resolution_clock::now();
                if (now - prev_time >= std::chrono::seconds(2) && player.getHealth() > 0)
                {
                    prev_time = now;
                    PlaySound(TEXT("sounds/destory.wav"), NULL, SND_FILENAME | SND_ASYNC);
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    player.takeDamage(150);
                    break;
                }
            }
        }
        // // Collision Enemy Body & Player Body (Not Used Anymore)
        // for(Enemy& enemy: enemies){
        //     Sphere enemySphere = enemy.getSphere();
        //     if(checkSpheresIntersection(playerSphere.x, playerSphere.y, playerSphere.z, playerSphere.radius,
        //         enemySphere.x, enemySphere.y, enemySphere.z, enemySphere.radius)){
        //             cout << "You Got Hit!!!\n";
        //         }
        // }
        // Enemy Body & Player Bullet
        for (int bind = 0; bind < playerBullets.size(); bind++)
        {
            Sphere bulletSphere = playerBullets[bind].getSphere();
            for (int eind = 0; eind < enemies.size(); eind++)
            {
                Sphere enemySphere = enemies[eind].getSphere();
                if (checkSpheresIntersection(bulletSphere.x, bulletSphere.y, bulletSphere.z, bulletSphere.radius,
                                             enemySphere.x, enemySphere.y, enemySphere.z, enemySphere.radius))
                {
                    enemies.erase(enemies.begin() + eind);
                    playerBullets.erase(playerBullets.begin() + bind);
                    bind--;
                    game_mode.update_kill();
                    PlaySound(TEXT("sounds/pop.wav"), NULL, SND_FILENAME | SND_ASYNC);
                    break;
                }
            }
        }

        // Enemy Bullets & Player Body
        for (int bind = 0; bind < enemyBullets.size(); bind++)
        {
            Sphere bulletSphere = enemyBullets[bind].getSphere();
            if (checkSpheresIntersection(bulletSphere.x, bulletSphere.y, bulletSphere.z, bulletSphere.radius,
                                         playerSphere.x, playerSphere.y, playerSphere.z, playerSphere.radius))
            {
                player.takeDamage(enemyBullets[bind].getDamage());
                enemyBullets.erase(enemyBullets.begin() + bind);
                bind--;
                if (player.getHealth() > 0)
                {
                    PlaySound(TEXT("sounds/hurt.wav"), NULL, SND_FILENAME | SND_ASYNC);
                }
            }
        }

        // Player Body & Power Ups
        for (int i = 0; i < powerUps.size(); i++)
        {
            Sphere powerSphere = powerUps[i].getSphere();
            if (checkSpheresIntersection(playerSphere.x, playerSphere.y, playerSphere.z, playerSphere.radius,
                                         powerSphere.x, 0.0, powerSphere.z, powerSphere.radius + 2.0))
            {
                player.applyPowerUp(powerUps[i]);
                powerUps.erase(powerUps.begin() + i);
                i--;
                if (player.getHealth() > 0)
                {
                    PlaySound(TEXT("sounds/item-pick-up.wav"), NULL, SND_FILENAME | SND_ASYNC);
                }
                break;
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
    void gameSpawnPowerUp()
    {
        int type = rand() % 3;
        if (powerUps.size() < 10)
        {
            powerUps.emplace_back(Sphere(rand() % 1400 - 700, 0, rand() % 1400 - 700, 1.0), type);
        }
    }
    void mouseInput(int button, int state, int x, int y)
    {
        if (game_mode.getDisplay() == MENU)
        {
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
            {
                if (x >= 120 && x <= 375 && y >= 260 && y <= 300)
                {
                    game_mode.chooseMode(SURVIVAL);
                    restart(SURVIVAL);
                }
                else if (x >= 120 && x <= 375 && y >= 310 && y <= 350)
                {
                    game_mode.chooseMode(TIMER);
                    restart(TIMER);
                }
                else
                {
                    cout << "Mouse Clicked at: " << x << ' ' << y << endl;
                }
            }
            glutPostRedisplay();
        }
    }
};