#include <iostream>
#include "tank.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <Glut/glut.h>
#include <OpenGL/glext.h>
#include "vector.h"
#include "display.h"
#include "bullet.h"


using namespace std;

enum menu{
	mainMenu,
	game,
	pauseMenu
};

struct color {
	float r;
	float g;
	float b;
};

color mainStartColor, mainEndColor, mainOptionsColor, pauseResumeColor, pauseResetColor, pauseMainMenuColor; 


menu menuType = mainMenu;
float high = 0.0f;
float side = 0.0f;
int player = 1;
tank tanks[2];
tank *c;
char info[50];
char mainStart[13] = "Start Game \0";
char mainEnd[7] = "Quit \0";
char mainOptions[10] = "Options \0";
char pauseResume[9] = "Resume \0";
char pauseMainMenu[12] = "Main Menu \0";
char pauseReset[15] = "Restart Game \0";

int option = 2;




display disp(0,100,0,100);
bullet *bull = NULL;
vector gravity(0,-9.80);
float curTime = 0, timebase = 0, elapsedTime = 0;


void turn(){
	if (player == 1){
		player = 2;
		c = &tanks[0];
	}else{
		player = 1;
		c = &tanks[1];
	}
}



void update(){
	curTime = glutGet(GLUT_ELAPSED_TIME);
	elapsedTime = (curTime - timebase)/1000;
	timebase = glutGet(GLUT_ELAPSED_TIME);
	sprintf(info,"ANGLE: %.0f   POWER: %.0f", c->angle, c->power);
	if (bull != NULL){
		bull->update(elapsedTime,gravity);
		if (bull->position.y < 0 || bull->position.x < -3 || bull->position.x >101){
			delete bull;
			bull = NULL;
			turn();
		}
	}
	
	
}


void shoot(){
	if (bull == NULL){
		vector vel, pos, shift;
		vel.component(c->power, 180 - c->angle);
		shift.component(3, 180 - c->angle);
		pos = c->position + shift;
		bull = new bullet(pos, vel, 1);
	}
}







void drawTank() {
	for (int i = 0; i<2; i++){
		c = &tanks[i];
		
		glLoadIdentity();
		glTranslatef(c->position.x, c->position.y, 0.0f);
		
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
			glVertex2f(-2.0f,-0.5f);
			glVertex2f( -1.0f,-1.0f);
			glVertex2f( 1.0f,-1.0f);
			glVertex2f( 2.0f,-0.5f);
			glVertex2f( 2.0f, 1.0f);
			glVertex2f( -2.0f, 1.0f);
		glEnd();
		
		glRotatef(c->angle, 0.0f, 0.0f, -1.0f);
		glBegin(GL_QUADS);
			glVertex2f(-3.0f,-0.25f);
			glVertex2f(-3.0f,0.25f);
			glVertex2f(0.0f,0.25f);
			glVertex2f(0.0f,-0.25f);
		glEnd();
	
	}
	
	if (player == 1){
		c = &tanks[0];
	}else{
		c = &tanks[1];
	}
}


void changeSize(int w, int h) {
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	
	
	
}


void mainMenuSetup(){
	menuType = mainMenu;
	disp.font = GLUT_BITMAP_TIMES_ROMAN_24;
	
	disp.min.x = -1.8;
	disp.max.x = 7.8;
	disp.min.y = 0;
	disp.max.y = 10;
	option = 1;
	
}

void gameSetup(){
	menuType = game;
	
	disp.min.x = -30;
	disp.max.x = 130;
	disp.min.y = 0;
	disp.max.y = 100;
	
	if (bull != NULL){
		delete bull;
		bull = NULL; //this is fun
	}
	
	
	//tank a;
	tanks[0].position.x = 10;
	tanks[0].position.y = 1;
	//tank b;
	tanks[1].position.x = 90;
	tanks[1].position.y = 1;
	//tank *c;
	c = tanks;
}



void pauseMenuSetup(){
	menuType = pauseMenu;
	
	option = 1;
	
}

void drawPauseMenu(){
	
	/*
	glLoadIdentity();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glVertex2f(40.0f,70.0f);
		glVertex2f(40.0f,30.0f);
		glVertex2f(60.0f,30.0f);
		glVertex2f(60.0f,70.0f);
	glEnd();
	*/
	
	
	
	pauseResumeColor.r = 1.0f;
	pauseResumeColor.g = 1.0f;
	pauseResumeColor.b = 1.0f;
	pauseResetColor.r = 1.0f;
	pauseResetColor.g = 1.0f;
	pauseResetColor.b = 1.0f;
	pauseMainMenuColor.r = 1.0f;
	pauseMainMenuColor.g = 1.0f;
	pauseMainMenuColor.b= 1.0f;
	
	if(option == 1){
		pauseResumeColor.r = 1.0f;
		pauseResumeColor.g = 0.0f;
		pauseResumeColor.b = 0.0f;
	}else if(option == 2){
		pauseResetColor.r = 0.0f;
		pauseResetColor.g = 1.0f;
		pauseResetColor.b = 0.0f;
	}else{
		pauseMainMenuColor.r = 0.0f;
		pauseMainMenuColor.g = 0.0f;
		pauseMainMenuColor.b = 1.0f;
	}
	disp.text(45, 60, pauseResume, pauseResumeColor.r, pauseResumeColor.g, pauseResumeColor.b);
	disp.text(42, 50, pauseReset, pauseResetColor.r, pauseResetColor.g, pauseResetColor.b);
	disp.text(43, 40, pauseMainMenu, pauseMainMenuColor.r, pauseMainMenuColor.g, pauseMainMenuColor.b);
	
	
	
}

void renderScene(void) {
	switch (menuType){
		case mainMenu:
			glClear(GL_COLOR_BUFFER_BIT);
			disp.setOrthographicProjection();
			glLoadIdentity();
			
			mainStartColor.r = 1.0f;
			mainStartColor.g = 1.0f;
			mainStartColor.b = 1.0f;
			mainOptionsColor.r = 1.0f;
			mainOptionsColor.g = 1.0f;
			mainOptionsColor.b = 1.0f;
			mainEndColor.r = 1.0f;
			mainEndColor.g = 1.0f;
			mainEndColor.b= 1.0f;
			
			if(option == 1){
				mainStartColor.r = 1.0f;
				mainStartColor.g = 1.0f;
				mainStartColor.b = 0.0f;
			}else if(option == 2){
				mainOptionsColor.r = 1.0f;
				mainOptionsColor.g = 1.0f;
				mainOptionsColor.b = 0.0f;
			}else{
				mainEndColor.r = 1.0f;
				mainEndColor.g = 1.0f;
				mainEndColor.b = 0.0f;
			}
			disp.text(2, 7, mainStart, mainStartColor.r, mainStartColor.g, mainStartColor.b);
			disp.text(2, 5, mainOptions, mainOptionsColor.r, mainOptionsColor.g, mainOptionsColor.b);
			disp.text(2, 3, mainEnd, mainEndColor.r, mainEndColor.g, mainEndColor.b);
			
			glutSwapBuffers();
			
			break;
		case game:
			update();
	
			glClear(GL_COLOR_BUFFER_BIT);
			disp.setOrthographicProjection();
			glLoadIdentity();
	
			//Functions to draw objects
			drawTank();
			if (bull != NULL){
				bull->drawBullet();
			}
			disp.text(5,90,info,1.0f,1.0f,1.0f);
			
			glutSwapBuffers();
			
			break;
		case pauseMenu:
			
	
			glClear(GL_COLOR_BUFFER_BIT);
			disp.setOrthographicProjection();
			glLoadIdentity();
			
			//Functions to draw objects
			//drawPauseMenu();
			glLoadIdentity();
			glColor3f(0.0f, 0.0f, 1.0f);
			glBegin(GL_QUADS);
				glVertex2f(40.0f,70.0f);
				glVertex2f(40.0f,30.0f);
				glVertex2f(60.0f,30.0f);
				glVertex2f(60.0f,70.0f);
			glEnd();
			
			
			//drawPauseMenu();
			
			drawTank();
			if (bull != NULL){
				bull->drawBullet();
			}
			disp.text(5,90,info,1.0f,1.0f,1.0f);
			
			drawPauseMenu();
			
			glutSwapBuffers();
			
			break;
		}
}




void processNormalKeys(unsigned char key, int x, int y) {
	switch(key){
		case 27:
			switch (menuType) {
				case mainMenu:
					
					break;
				case game:
					pauseMenuSetup();
					break;
				case pauseMenu:
					menuType = game;
					timebase = glutGet(GLUT_ELAPSED_TIME);
					break;
			}
			break;
		case 119:
			switch (menuType) {
				case mainMenu:
					
					break;
				case game:
					c->power += 1;
					break;
				case pauseMenu:
					
					break;
			}
			break;
		case 13:
			switch (menuType) {
				case mainMenu:
					switch (option){
						case 1:
							gameSetup();
							break;
						case 2:
							
							break;
						case 3:
							exit(0);
							break;
					}
					break;
				case game:
					shoot();
					break;
				case pauseMenu:
					
					break;
			}
			break;
		case 97:
			switch (menuType) {
				case mainMenu:
					
					break;
				case game:					
					c->angle -= 1;
					break;
				case pauseMenu:
					
					break;
			}
			break;
		case 100:
			switch (menuType) {
				case mainMenu:
					
					break;
				case game:
					c->angle += 1;
					break;
				case pauseMenu:
					
					break;
			}
			break;
		case 115:
			switch (menuType) {
				case mainMenu:
					
					break;
				case game:
					c->power -= 1;
					break;
				case pauseMenu:
					
					break;
			}
			break;
		case 32:
			switch (menuType) {
				case mainMenu:
					
					break;
				case game:
					shoot();
					break;
				case pauseMenu:
					
					break;
			}
			break;
	}
}

void processSpecialKeys(int key, int x, int y) {

	switch(key) {
		case GLUT_KEY_UP :
			switch (menuType) {
				case mainMenu:
					if (option > 1){
						option--;
					}
					break;
				case game:
					
					break;
				case pauseMenu:
					if (option > 1){
						option--;
					}
					break;
			}
			break;
			
		case GLUT_KEY_DOWN :
			switch (menuType) {
				case mainMenu:
					if (option < 3){
						option++;
					}
					break;
				case game:
					
					break;
				case pauseMenu:
					if (option < 3){
						option++;
					}
					break;
			}
			break;
			
		case GLUT_KEY_LEFT :
			switch (menuType) {
				case mainMenu:
					if (option > 1){
						option--;
					}
					break;
				case game:
					
					break;
				case pauseMenu:
					
					break;
			}
			break;
				
		case GLUT_KEY_RIGHT :
			switch (menuType) {
				case mainMenu:
					if (option > 1){
						option--;
					}
					break;
				case game:
					
					break;
				case pauseMenu:
					
					break;
			}
			break;
				
	}
}

void startUp(){
	mainMenuSetup();
	//menuType = game;
}

int main (int argc, char **argv) {
	
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	
	/*
	glutInitWindowPosition(1,1);
	glutInitWindowSize(800,800);
	glutCreateWindow("Tanks");
	*/
	
	glutGameModeString("1280x800");
	// enter full screen
	if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
		glutEnterGameMode();
	else {
		printf("The select mode is not available\n");
		exit(1);
	}
	
	startUp();
		
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	
	// enter GLUT event processing loop
	glutMainLoop();

	return 1;
}
