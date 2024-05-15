#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "Game.cpp"  
int width = 500, height = 500;
Game game(width, height);
void displayFrame(int value){
	game.render();
	glutTimerFunc(34, displayFrame, value);
}
void displayFrameCount(int value){
	cout << game.frameCount << " Frames per second\n";
	game.frameCount = 0;
	glutTimerFunc(1000, displayFrameCount, value);
}
// Main routine.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Space Wars.cpp");
	glutDisplayFunc([](){
		game.render();
	});
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
	
	glewExperimental = GL_TRUE;
	glewInit();
	glutIgnoreKeyRepeat(1);
	glutTimerFunc(34, displayFrame, 1);
	glutTimerFunc(1000, displayFrameCount, 2);
	game.setup();
	glutMainLoop();
}