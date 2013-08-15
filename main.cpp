#include <iostream>
#include <fstream>
#include <string>
#include "tank.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <Glut/glut.h>
#include <OpenGL/glext.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <vorbis/vorbisfile.h>
#include "vector.h"
#include "display.h"
#include "bullet.h"
#include "texture.h"
#include "openal.h"
#include "sound.h"

using namespace std;

enum menu{
	mainMenu,
	game,
	pauseMenu,
	options,
	gameEnd
};

struct color {
	float r;
	float g;
	float b;
};

color mainStartColor, mainEndColor, mainOptionsColor, pauseResumeColor, pauseResetColor, pauseMainMenuColor, optionTurnsColor, optionReturnColor, optionWindColor; 

GLuint bulletTexture;
GLuint tankTexture;

OpenAL openal;

menu menuType = mainMenu;
float high = 0.0f;
float side = 0.0f;
int player = 1;
tank tanks[2];
tank *c;
char info[50];
char playerA[20];
char playerB[20];
char playerTurn[20];
char mainStart[13] = "Start Game \0";
char mainEnd[7] = "Quit \0";
char mainOptions[10] = "Options \0";
char pauseResume[9] = "Resume \0";
char pauseMainMenu[12] = "Main Menu \0";
char pauseReset[15] = "Restart Game \0";
char optionWind[11] = "Wind: On\0";
char optionTurns[30] = "Number Of Turns: \0";
char optionReturn[23] = "Return To Main Menu \0";
char gameEndWinner[19] = "WINNER: Player 1\0";

float frame = 0;
float startFPSTime = 0;
float fps = 0;

int option = 2;
int wind = 1;
int turnNumber;
int totalTurns = 5;
int winner = 1;

void gameEndSetup();

display disp(0,100,0,100);
bullet *bull = NULL;
vector gravity(0,-9.80);
float curTime = 0, timebase = 0, elapsedTime = 0;

void saveFile(){
	cout << "Saving file\n";
	ofstream myfile;
	myfile.open ("optionData.txt");
	myfile << wind << endl;
	myfile << totalTurns;
	myfile.close();
	cout << "save successful\n";
	
}

void loadFile(){
	ifstream myfile ("optionData.txt");
	if (myfile.is_open()){
		for(int x = 0; x < 2; x++){
			switch(x){
				case 0:
					myfile >> wind;
					switch(wind){
						case 1:
							cout << "Wind is On" << endl;
							sprintf(optionWind, "Wind: On");
							break;
						case 0:
							cout << "Wind is Off" << endl;
							sprintf(optionWind, "Wind: Off");
					}
					break;
				case 1:
					myfile >> totalTurns;
					cout << "Number of turns: " << totalTurns << endl;
			}
		} 
		myfile.close();
	}else {
		cout << "Unable to open file\n";
		cout << "creating opionData.txt...\n";
		ofstream myfile;
		myfile.open ("optionData.txt");
		myfile << "0\n";
		myfile << "3\n";
		myfile.close();
		cout << "create successful\n";
		
	}
	
}

void windToggle(){
	if (wind == 1){
		wind = 0;
		sprintf(optionWind, "Wind: Off");
	}else if (wind == 0){
		wind = 1;
		sprintf(optionWind, "Wind: On");
	}
}

void randomWind(){
	gravity.x = rand()%11 - 5; 
	if (wind == 0){
		gravity.x = 0;
	}
}

void turn(){
	randomWind();
	if (player == 1){
		player = 2;
		c = &tanks[1];
	}else{
		player = 1;
		c = &tanks[0];
		turnNumber++;
		if (turnNumber > totalTurns){
			if (tanks[0].score < tanks[1].score){
				winner = 2;
			}else if (tanks[0].score > tanks[1].score){
				winner = 1;
			}else{
				winner = 0;
			}
			gameEndSetup();
		}
	}
}

int randomPosition (){
	int pos = rand()%14;
	return(pos);
}

void collisionDetection(){
	if (bull != NULL){
		for (int i = 0; i < 2 ; i++){
			if (bull->position.x - bull->size <= tanks[i].position.x + tanks[i].size.x / 2 && 
				bull->position.x + bull->size >= tanks[i].position.x - tanks[i].size.x / 2 && 
				bull->position.y - bull->size <= tanks[i].position.y + tanks[i].size.y / 2 && 
				bull->position.y + bull->size >= tanks[i].position.y - tanks[i].size.y / 2 ){
					
					if (bull->position.x <= tanks[i].position.x + tanks[i].size.x / 2 && bull->position.x >= tanks[i].position.x - tanks[i].size.x / 2 || bull->position.y <= tanks[i].position.y + tanks[i].size.y / 2 && bull->position.y >= tanks[i].position.y - tanks[i].size.y / 2 ){
						delete bull;
						bull = NULL;
						if (c != &tanks[i]){
							c->score++;
						}else if (c == &tanks[i]){
							c->score--;
						}
						turn();
						openal.play(ALblowUpTank1);
						return;
					}else if(vectorDistance(bull->position, tanks[i].position.x + tanks[i].size.x / 2, tanks[i].position.y + tanks[i].size.y / 2) <= bull->size || vectorDistance(bull->position, tanks[i].position.x + tanks[i].size.x / 2, tanks[i].position.y - tanks[i].size.y / 2) <= bull->size || vectorDistance(bull->position, tanks[i].position.x - tanks[i].size.x / 2, tanks[i].position.y - tanks[i].size.y / 2) <= bull->size || vectorDistance(bull->position, tanks[i].position.x - tanks[i].size.x / 2, tanks[i].position.y + tanks[i].size.y / 2) <= bull->size){
						delete bull;
						bull = NULL;
						if (c != &tanks[i]){
							c->score++;
						}else if (c == &tanks[i]){
							c->score--;
						}
						turn();
						openal.play(ALblowUpTank1);
						return;
					}
			
			}
		}
		
		if(bull != NULL){
			if (bull->position.y + bull->size < 0 || bull->position.x + bull->size < 0 || bull->position.x - bull->size >100){
				delete bull;
				bull = NULL;
				turn();
			}
		}
		 
	}
}

void update(){
	curTime = glutGet(GLUT_ELAPSED_TIME);
	
	elapsedTime = (curTime - timebase)/1000;
	
	frame++;
	if ((((glutGet(GLUT_ELAPSED_TIME) - startFPSTime))/1000) > 1){
		fps = frame/((curTime-startFPSTime)/1000);
		frame = 0;
		startFPSTime = glutGet(GLUT_ELAPSED_TIME);
		
	}
	
	timebase = glutGet(GLUT_ELAPSED_TIME);
	sprintf(info,"ANGLE: %.0f   POWER: %.0f,   WIND: %.0f   TURN: %d   FPS: %.0f", c->angle, c->power, gravity.x, turnNumber, fps);
	sprintf(playerA,"SCORE: %.0f", tanks[0].score);
	sprintf(playerB,"SCORE: %.0f", tanks[1].score);
	sprintf(playerTurn,"PLAYER: %d", player);
	if (bull != NULL){
		bull->update(elapsedTime,gravity);
		collisionDetection();
		
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
	if (c == &tanks[0]){
		openal.play(ALtank1);
	}else{
		openal.play(ALtank2);
	}
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
	
	randomWind();
	
	disp.min.x = -30;
	disp.max.x = 130;
	disp.min.y = 0;
	disp.max.y = 100;
	
	if (bull != NULL){
		delete bull;
		bull = NULL; //this is fun
	}
	
	
	//tank a;
	tanks[0].position.x = randomPosition() + 3;
	tanks[0].position.y = 1;
	tanks[0].angle = 110;
	tanks[0].power = 30;
	tanks[0].score = 0;
	//tank b;
	tanks[1].position.x = randomPosition() + 83;
	tanks[1].position.y = 1;
	tanks[1].angle = 70;
	tanks[1].power = 30;
	tanks[1].score = 0;
	
	c = tanks;
	player = 1;
	turnNumber = 1;
	
}

void pauseMenuSetup(){
	menuType = pauseMenu;
	
	option = 1;
	
}

void gameEndSetup(){
	menuType = gameEnd;
	option = 2;
	
}

void optionMenuSetup(){
	menuType = options;
	option = 1;
}

void drawOptions(){
	
	optionWindColor.r = 1.0f;
	optionWindColor.g = 1.0f;
	optionWindColor.b = 1.0f;
	optionTurnsColor.r = 1.0f;
	optionTurnsColor.g = 1.0f;
	optionTurnsColor.b = 1.0f;
	optionReturnColor.r = 1.0f;
	optionReturnColor.g = 1.0f;
	optionReturnColor.b= 1.0f;
	
	if(option == 1){
		optionWindColor.r = 1.0f;
		optionWindColor.g = 0.0f;
		optionWindColor.b = 0.0f;
	}else if(option == 2){
		optionTurnsColor.r = 1.0f;
		optionTurnsColor.g = 0.0f;
		optionTurnsColor.b = 0.0f;
	}else{
		optionReturnColor.r = 1.0f;
		optionReturnColor.g = 0.0f;
		optionReturnColor.b = 0.0f;
	}
	
	sprintf(optionTurns,"Number Of Turns: %d", totalTurns);
	
	disp.text(2, 7, optionWind, optionWindColor.r, optionWindColor.g, optionWindColor.b);
	disp.text(2, 5, optionTurns, optionTurnsColor.r, optionTurnsColor.g, optionTurnsColor.b);
	disp.text(2, 3, optionReturn, optionReturnColor.r, optionReturnColor.g, optionReturnColor.b);
	
	
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
		pauseResetColor.r = 1.0f;
		pauseResetColor.g = 0.0f;
		pauseResetColor.b = 0.0f;
	}else{
		pauseMainMenuColor.r = 1.0f;
		pauseMainMenuColor.g = 0.0f;
		pauseMainMenuColor.b = 0.0f;
	}
	disp.text(45, 60, pauseResume, pauseResumeColor.r, pauseResumeColor.g, pauseResumeColor.b);
	disp.text(42, 50, pauseReset, pauseResetColor.r, pauseResetColor.g, pauseResetColor.b);
	disp.text(43, 40, pauseMainMenu, pauseMainMenuColor.r, pauseMainMenuColor.g, pauseMainMenuColor.b);
	
	
	
}

void drawGameEnd(){
	
	pauseResetColor.r = 1.0f;
	pauseResetColor.g = 1.0f;
	pauseResetColor.b = 1.0f;
	pauseMainMenuColor.r = 1.0f;
	pauseMainMenuColor.g = 1.0f;
	pauseMainMenuColor.b= 1.0f;
	
	
	if(option == 2){
		pauseResetColor.r = 1.0f;
		pauseResetColor.g = 0.0f;
		pauseResetColor.b = 0.0f;
	}else if (option == 3){
		pauseMainMenuColor.r = 1.0f;
		pauseMainMenuColor.g = 0.0f;
		pauseMainMenuColor.b = 0.0f;
	}
	disp.text(42, 55, pauseReset, pauseResetColor.r, pauseResetColor.g, pauseResetColor.b);
	disp.text(43, 45, pauseMainMenu, pauseMainMenuColor.r, pauseMainMenuColor.g, pauseMainMenuColor.b);
	if (winner == 0){
		sprintf(gameEndWinner, "WINNER: TIE");
	}else{
		sprintf(gameEndWinner, "WINNER: Player %d", winner);
	}
	disp.text(40, 10, gameEndWinner, 1, 1, 1);
	sprintf(playerA,"SCORE: %.0f", tanks[0].score);
	sprintf(playerB,"SCORE: %.0f", tanks[1].score);
	
	
	
	
}

void renderScene(void) {
	switch (menuType){
		case mainMenu:
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
		case options:
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			disp.setOrthographicProjection();
			glLoadIdentity();
			
			drawOptions();
			
			glutSwapBuffers();
			
			break;
		case game:
			update();
	
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			disp.setOrthographicProjection();
			glLoadIdentity();
	
			//Functions to draw objects
			tanks[0].drawTank(tankTexture);
			tanks[1].drawTank(tankTexture);
			
			if (bull != NULL){
				bull->drawBullet(bulletTexture);
			}
			disp.text(5,90,info,1.0f,1.0f,1.0f);
			disp.text(5,80,playerA,1.0f,1.0f,1.0f);
			disp.text(90,80,playerB,1.0f,1.0f,1.0f);
			disp.text(90,90,playerTurn,1.0f,1.0f,1.0f);
			
			glutSwapBuffers();
			
			break;
		case pauseMenu:
		

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			disp.setOrthographicProjection();
			glLoadIdentity();
		
			//Functions to draw objects
			if (bull != NULL){
				bull->drawBullet(bulletTexture);
			}
		
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
		
			tanks[0].drawTank(tankTexture);
			tanks[1].drawTank(tankTexture);
		
		
			disp.text(5,90,info,1.0f,1.0f,1.0f);
			disp.text(5,80,playerA,1.0f,1.0f,1.0f);
			disp.text(90,80,playerB,1.0f,1.0f,1.0f);
			disp.text(90,90,playerTurn,1.0f,1.0f,1.0f);
		
			drawPauseMenu();
		
			glutSwapBuffers();
		
			break;
		case gameEnd:
	

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			disp.setOrthographicProjection();
			glLoadIdentity();
	
			
	
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
	
			tanks[0].drawTank(tankTexture);
			tanks[1].drawTank(tankTexture);
	
	
			disp.text(5,90,info,1.0f,1.0f,1.0f);
			disp.text(5,80,playerA,1.0f,1.0f,1.0f);
			disp.text(90,80,playerB,1.0f,1.0f,1.0f);
			disp.text(90,90,playerTurn,1.0f,1.0f,1.0f);
	
			drawGameEnd();
	
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
					if (bull == NULL && c->power < 100){
						c->power += 1;
					}
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
							optionMenuSetup();
							break;
						case 3:
							exit(0);
							break;
					}
					break;
				case options:
					switch (option){
						case 1:
							windToggle();
							break;
						case 2:
							
							break;
						case 3:
							saveFile();
							mainMenuSetup();
							break;
					}
					break;
				case game:
					if (bull == NULL){
						shoot();
					}
					break;
				case pauseMenu:
					switch (option){
						case 1:
							menuType = game;
							timebase = glutGet(GLUT_ELAPSED_TIME);
							break;
						case 2:
							gameSetup();
							break;
						case 3:
							mainMenuSetup();
							break;
					}
					break;
				case gameEnd:
					switch (option){
						case 2:
							gameSetup();
							break;
						case 3:
							mainMenuSetup();
							break;
					}
					break;
			}
			break;
		case 97:
			switch (menuType) {
				case mainMenu:
					
					break;
				case game:
					if (bull == NULL && c->angle > 45){
						c->angle -= 1;
					}
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
					if (bull == NULL && c->angle < 135){
						c->angle += 1;
					}
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
					if (bull == NULL && c->power > 0){
						c->power -= 1;
					}
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
					if (bull == NULL){
						shoot();
					}
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
				case options:
					if (option > 1){
						option--;
					}
					break;
				case gameEnd:
					if (option > 2){
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
				case gameEnd:
					if (option < 3){
						option++;
					}
					break;
				case options:
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
					
					break;
				case game:
					
					break;
				case pauseMenu:
					
					break;
				case options:
					if (option == 2){
						if (totalTurns > 1 ){
							totalTurns--;
						}
					}
			}
			break;
				
		case GLUT_KEY_RIGHT :
			switch (menuType) {
				case mainMenu:
					
					break;
				case game:
					
					break;
				case pauseMenu:
					
					break;
				case options:
					if (option == 2){
						totalTurns++;
					}
			}
			break;
				
	}
}

void startUp(){
	mainMenuSetup();
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
	glShadeModel(GL_FLAT);
	
	loadFile();
	
	load_texture("bullet.png", &bulletTexture);
	load_texture("tankGraphicC.png", &tankTexture);
	
	openal.init();
	
	//openALSetup();
	//snd_load_file("missleSound.ogg", sound);
	
}

int main (int argc, char **argv) {
	
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	srand(time(0));
		
	
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
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	
	// enter GLUT event processing loop
	glutMainLoop();

	return 1;
}
