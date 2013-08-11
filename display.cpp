#include "display.h"

display::display(){
	min.x = -10;
	min.y = -10;
	max.x = 10;
	max.y = 10;
	font = GLUT_BITMAP_HELVETICA_12;
	
}

display::display(float x, float xx, float y, float yy){
	min.x = x;
	min.y = y;
	max.x = xx;
	max.y = yy;
	font = GLUT_BITMAP_HELVETICA_12;
}

void display::restorePerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();

	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void display::setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();
	
	gluOrtho2D(min.x, max.x, min.y, max.y);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void display::renderBitmapString(float x, float y, char *string) {

	char *d;
	glRasterPos2f(x, y);
	for (d=string; *d != '\0'; d++) {
		glutBitmapCharacter(font, *d);
	}
}

void display::text(float x, float y, char* s, float r, float g, float b){
	glColor3f(r,g,b);
	glPushMatrix();
	glLoadIdentity();
	setOrthographicProjection();
	renderBitmapString(x,y,s);
	glPopMatrix();
	restorePerspectiveProjection();
}
