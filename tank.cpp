#include "tank.h"


tank::tank(){
	angle = 90;
	power = 30;
}

tank::tank(vector pos, float ang, float pow){
	angle = ang;
	power = pow;
	position = pos;
}



void tank::drawTank() {
	glLoadIdentity();
	glTranslatef(position.x, position.y, 0.0f);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
		glVertex2f(-2.0f,-0.5f);
		glVertex2f( -1.0f,-1.0f);
		glVertex2f( 1.0f,-1.0f);
		glVertex2f( 2.0f,-0.5f);
		glVertex2f( 2.0f, 1.0f);
		glVertex2f( -2.0f, 1.0f);
	glEnd();
	
	glRotatef(angle, 0.0f, 0.0f, -1.0f);
	glBegin(GL_QUADS);
		glVertex2f(-3.0f,-0.25f);
		glVertex2f(-3.0f,0.25f);
		glVertex2f(0.0f,0.25f);
		glVertex2f(0.0f,-0.25f);
	glEnd();
}