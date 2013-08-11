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