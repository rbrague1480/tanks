#ifndef _display_h_
#define _display_h_

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <Glut/glut.h>
#include <OpenGL/glext.h>
#include "vector.h"



class display{
	public:
		vector min, max;
		void *font;
		display();
		display(float,float,float,float);
		
		void restorePerspectiveProjection();
		void setOrthographicProjection();
		void renderBitmapString(float x, float y, char *string);
		void text(float, float, char*, float, float, float);

};



#endif