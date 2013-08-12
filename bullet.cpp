#include "bullet.h"
#include <iostream>


bullet::bullet(vector pos, vector vel, float s){
	position = pos;
	velocity = vel;
	size = s;
}

void bullet::update(float eTime, vector grav){
	position = position + velocity * eTime + grav * 0.5 * eTime * eTime;
	velocity = velocity + grav * eTime;
	
}


void bullet::drawBullet(GLuint texture){
	
	glLoadIdentity();
	glTranslatef(position.x, position.y, 0.0f);

	glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture (GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-size, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-size, size);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(size, size);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(size, -size);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);

	
}