#include "vector.h"

vector::vector(){
	x = 0;
	y = 0;
}

vector::vector(float xx, float yy){
	x = xx;
	y = yy;
}

void vector::component(float power, float angle){
	angle = angle / 180 * M_PI;
	x = cos(angle) * power;
	y = sin(angle) * power;
}

vector vector::operator+ (vector a) {
	vector b;
	b.x = x + a.x;
	b.y = y + a.y;

	return (b);
}

vector vector::operator- (vector a) {
	vector b;
	b.x = x - a.x;
	b.y = y - a.y;

	return (b);
}

vector vector::operator *(float scalar){
	vector b;
	b.x = x*scalar;
	b.y = y*scalar;
	
	return (b);
}

void vector::operator= (vector a) {
  x = a.x;
  y = a.y;
}