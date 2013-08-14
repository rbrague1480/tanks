#include "tank.h"


tank::tank(){
	angle = 90;
	power = 30;
	size.x = 4;
	size.y = 2;
}

tank::tank(vector pos, float ang, float pow){
	angle = ang;
	power = pow;
	position = pos;
	size.x = 4;
	size.y = 2;
}



void tank::drawTank(GLuint texture) {
	
	glLoadIdentity();
	glTranslatef(position.x, position.y, 0.0f);
	/*
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
		glVertex2f(-size.x/2, -size.y/2);
		glVertex2f(-size.x/2, size.y/2);
		glVertex2f(size.x/2, size.y/2);
		glVertex2f(size.x/2, -size.y/2);
	glEnd();
	*/
	glRotatef(angle, 0.0f, 0.0f, -1.0f);
	glBegin(GL_QUADS);
		glVertex2f(-3.0f,-0.25f);
		glVertex2f(-3.0f,0.25f);
		glVertex2f(0.0f,0.25f);
		glVertex2f(0.0f,-0.25f);
	glEnd();
	
	
	glLoadIdentity();
	glTranslatef(position.x, position.y, 0.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-size.x/2, -size.y/2);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-size.x/2, size.y*1.5);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(size.x/2, size.y*1.5);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(size.x/2, -size.y/2);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
}