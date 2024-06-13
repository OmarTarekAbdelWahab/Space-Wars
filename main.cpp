#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "Game.cpp"  
int width = 500, height = 500;
Game game(width, height);
int previous = 0;
int powerSpawn = 10;
void drawScene(void){
	game.render();
}
void displayFrame(int value) {
    // Render the game
    drawScene();

    // Calculate the time elapsed since the previous frame
    int current = glutGet(GLUT_ELAPSED_TIME);
    float elapsed = (current - previous) / 1000.0f; // Convert milliseconds to seconds
    previous = current;

    // Calculate the frame delay to achieve approximately 30 FPS
    float frameDelay = 1.0f / 30; // 30 FPS

    // If the actual frame rate exceeds the target frame rate, increase the delay
    if (elapsed < frameDelay) {
        int delay = (int)((frameDelay - elapsed) * 1000); // Convert seconds back to milliseconds

        // Schedule the next frame
        glutTimerFunc(delay, displayFrame, value);
    } else {
        // If the frame rate is higher than the target, reduce the delay
        // to try to bring it closer to the target frame rate
        glutTimerFunc(0, displayFrame, value);
    }
}
int previous2 = 0;
void displayFrameCount(int value){
	// Calculate the time elapsed since the previous frame
	// cout << game.frameCount << " Frames Per Second" << endl;
	game.frameCount = 0; 
    int current = glutGet(GLUT_ELAPSED_TIME);
    float elapsed = (current - previous2) / 1000.0f; // Convert milliseconds to seconds
    previous2 = current;

    // Calculate the frame delay to achieve approximately 30 FPS
    float frameDelay = 1.0f / 1; // 30 FPS
	
    // If the actual frame rate exceeds the target frame rate, increase the delay
    if (elapsed < frameDelay) {
        int delay = (int)((frameDelay - elapsed) * 1000); // Convert seconds back to milliseconds

        // Schedule the next frame
        glutTimerFunc(delay, displayFrameCount, value);
    } else {
        // If the frame rate is higher than the target, reduce the delay
        // to try to bring it closer to the target frame rate
        glutTimerFunc(0, displayFrameCount, value);
    }
}
void spawnPowerUp(int value){
    game.gameSpawnPowerUp();
    glutTimerFunc(powerSpawn*1000, spawnPowerUp, value);
}
// Main routine.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Space Wars.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc([](int w, int h){
		game.resize(w, h);
	});
	glutKeyboardFunc([](unsigned char key, int x, int y){
		game.keyInput(key, x, y);
	});
	glutKeyboardUpFunc([](unsigned char key, int x, int y){
		game.keyUpInput(key, x, y);
	});
	glutSpecialFunc([](int key, int x, int y){
		game.specialKeyInput(key, x, y);
	});
    glutMouseFunc([](int button, int state, int x, int y) {
        game.mouseInput(button, state, x, y);
    });
	
	glewExperimental = GL_TRUE;
	glewInit();
	glutIgnoreKeyRepeat(1);
	glutTimerFunc(34, displayFrame, 1);
	glutTimerFunc(1000, displayFrameCount, 2);
    glutTimerFunc(powerSpawn*1000, spawnPowerUp, 3);
	game.setup();
	glutMainLoop();
}