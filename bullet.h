#ifndef _bullet_h_
#define _bullet_h_
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <Glut/glut.h>
#include <OpenGL/glext.h>
#include "vector.h"
#include <math.h>


class bullet {
  public:
  	vector position, velocity;
  	float size;
  	
  	bullet(vector pos, vector vel, float s);
  	void update(float, vector);
  	void drawBullet();
};


#endif

