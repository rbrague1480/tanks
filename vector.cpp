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

float vectorDistance (vector a, vector b){
	float ax = a.x;
	float ay = a.y;
	float bx = b.x;
	float by = b.y;
	float ans;
	
	ans = sqrt((ax-bx) * (ax-bx) + (ay-by) * (ay-by));
	return(ans);
}

float vectorDistance (float ax, float ay, vector b){
	float bx = b.x;
	float by = b.y;
	float ans;
	ans = sqrt((ax-bx) * (ax-bx) + (ay-by) * (ay-by));
	return(ans);
	
}

float vectorDistance (vector a, float bx, float by){
	float ax = a.x;
	float ay = a.y;
	float ans;
	ans = sqrt((ax-bx) * (ax-bx) + (ay-by) * (ay-by));
	return(ans);
	
}

float vectorDistance (float ax, float ay, float bx, float by){
	float ans;
	ans = sqrt((ax-bx) * (ax-bx) + (ay-by) * (ay-by));
	return(ans);
	
}
