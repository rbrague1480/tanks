#ifndef _tank_h_
#define _tank_h_

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <Glut/glut.h>
#include <OpenGL/glext.h>
#include "vector.h"

class tank {
  public:
  	vector position;
	vector size;
  	float angle;
  	float power;
  	float score;
  	tank();
  	tank(vector, float, float);
  	void drawTank(GLuint);
  	void tankCollision();
  	
};


#endif