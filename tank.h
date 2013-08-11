#ifndef _tank_h_
#define _tank_h_
#include "vector.h"

class tank {
  public:
  	vector position;
  	float angle;
  	float power;
  	tank();
  	tank(vector, float, float);
  	
};


#endif