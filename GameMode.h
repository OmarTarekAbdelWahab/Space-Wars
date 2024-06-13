#include "Sphere.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <chrono>
#ifndef GameMode_H_
#define GameMode_H_

class GameMode
{
    int state;
    int dispaly;
    int game_mode;
    int time;
    int level;
    std::chrono::high_resolution_clock::time_point prev_time = std::chrono::high_resolution_clock::now();

public:
    GameMode();
    void chooseMode(int game_mode);
    void update(Player &player, std::vector<Enemy> &enemies, unsigned int enemyTexture);
    void draw();
    void drawOptions();
    int getDisplay();
};

#endif