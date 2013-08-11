#include "bullet.h"



bullet::bullet(vector pos, vector vel, float s){
	position = pos;
	velocity = vel;
	size = s;
}

void bullet::update(float eTime, vector grav){
	position = position + velocity * eTime + grav * 0.5 * eTime * eTime;
	velocity = velocity + grav * eTime;
	
}


void bullet::drawBullet(){
	for (int i = 0; i<360; i = i + 45){
		glLoadIdentity();
		
		glTranslatef(position.x, position.y, 0.0f);
		glRotatef(i, 0.0f, 0.0f, 1.0f);
		
		
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_TRIANGLES);
			glVertex2f(0, 0);
			glVertex2f(0, size);
			glVertex2f(0.7071 * size, 0.7071 * size);
		glEnd();
		
	}
}