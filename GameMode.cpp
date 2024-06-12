#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <chrono>
#include "GameMode.h"
#include "Enemy.h"
#include "Player.h"
#include <string>

#define SURVIVAL 0
#define TIMER 1

using namespace std;

GameMode::GameMode(){}

void GameMode::chooseMode(int game_mode){
    // take input
    this->game_mode = game_mode;
    if(game_mode == SURVIVAL){
        level = 0;
    }
    else{
        time = 200;
    }
}

void GameMode::update(Player& player, std::vector<Enemy>& enemies){

    if(this->game_mode == SURVIVAL){
        if(player.getHealth() <= 0){
            // game will stop
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
    }
    else{
        auto now_time = std::chrono::high_resolution_clock::now();
        if(now_time - prev_time >= std::chrono::seconds(1)){
            prev_time = now_time;
            time--;
            if(time == 0){
                // game will stop ... you win
                return;
            }
            else if(time%100 == 0){
                level++;
            }

            if(time%10 == 0){
                for(int i = 0; i < level; i++){
                    GLfloat x = rand()%1000 - 500;
                    GLfloat z = rand()%1000 - 500;
                    GLfloat angle = rand()%360;
                    enemies.emplace_back(Sphere(x, 0.0, z, 5.0), vector<GLfloat>{1.0, 1.0, 0.0}, angle, 1.0, 100, 1);
                }
            }
            if(enemies.size() == 0){
                // you win
                // game will stop
                // renderText(-4.5, 4.5, "You Win!!!");
                return;
            }
        }    
    }
}

void GameMode::draw(){
    string text;
    if(this->game_mode == SURVIVAL){
        text = "Level: " + to_string(this->level);
    }
    else{
        text = "Time Left: " + to_string(this->time);
    }
    glPushMatrix();
        glTranslatef(0.0, 0.0, -5.0);
        glColor3f(1.0, 0.0, 0.0);
        glRasterPos2f(-5, 4);
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
    glPopMatrix();  
}