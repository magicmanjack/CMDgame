#include "snow.h"
#include <stdlib.h>
#include <time.h>

std::vector<Snow*> Snow::fallingSnow;

Snow::Snow(int x, int y) {
	this -> x = x;
	this -> y = y;
	c = static_cast<char>(155);
	srand(time(NULL));
	fallSpd = (rand() % MAX_SPEED) + 1; // The speed will be randomly either 2, or 1;
}

void Snow::update() {
	y += fallSpd;
}