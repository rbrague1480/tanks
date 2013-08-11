#ifndef _vector_h_
#define _vector_h_
#include <math.h>

class vector {
	public:
		float x,y;
		
		vector();
		vector(float, float);
		void component(float, float);
		
		vector operator + (vector);
		vector operator - (vector);
		vector operator *(float);
		void operator = (vector);
};




#endif