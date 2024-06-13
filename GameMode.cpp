#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <chrono>
#include "GameMode.h"
#include "Enemy.h"
#include "Player.h"
#include <string>

#define SURVIVAL 0
#define TIMER 1

#define MENU 0
#define GAME 1

#define WIN 0
#define LOSE 1
#define PLAY 2

using namespace std;

GameMode::GameMode(){
    this->state = PLAY;
    this->display = MENU;
    this->kills = 0;
}

void GameMode::chooseMode(int game_mode){
    // take input
    this->game_mode = game_mode;
    this->state = PLAY;
    this->display = GAME;
    this->kills = 0;
    if(game_mode == SURVIVAL){
        level = 0;
    }
    else{
        time = 205;
    }
}

void GameMode::update_kill(){
    this->kills++;
}

void GameMode::update(Player& player, std::vector<Enemy>& enemies){

    if(this->game_mode == SURVIVAL){
        if(player.getHealth() <= 0){
            // game will stop
            this->display = MENU;
            this->state = LOSE;
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
        if(player.getHealth() <= 0){
            // game will stop
            this->display = MENU;
            this->state = LOSE;
            return;
        }
        auto now_time = std::chrono::high_resolution_clock::now();
        if(now_time - prev_time >= std::chrono::seconds(1)){
            prev_time = now_time;
            time--;
            if(time == 0){
                // game will stop ... you win
                this->display = MENU;
                this->state = WIN;
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
        glRasterPos2f(-5, 3.5);
        for (char c : "Kills: " + to_string(this->kills)) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
        }
    glPopMatrix();  
    if(this->display == MENU){
        drawOptions();
    }
    
}

// need to put menu options on the screen and tell him if he win or lose
void GameMode::drawOptions(){
    string text;
    if(this->state == WIN){
        text = "You Win!!!";
    }
    else{
        text = "You Lose!!!";
    }

    glPushMatrix();
        glLineWidth(2.0f);
        glTranslatef(0.0, 0.0, -5.0);
        glColor3f(0.0, 0.0, 1.0);
        glScalef(0.005f, 0.005f, 1.0f);
        glTranslatef(-300.0f, 320.0f, 0.0f);
        for (char c : text) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
        }

        glTranslatef(-800.0f, -500.0f, 0.0f); 
        for (char c : "Survival Mode") {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
        }

        glTranslatef(-850.0f, -200.0f, 0.0f);
        for (char c : "Timer Mode") {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
        }
        glLineWidth(1.0f);
    glPopMatrix();
}

int GameMode::getDisplay(){
    return this->display;
}