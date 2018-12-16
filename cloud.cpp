#include "cloud.h"
#include <vector>
#include <time.h>
#include <stdlib.h>

#define BL static_cast<char>(176)

std::vector<Cloud*> Cloud::clouds;
int Cloud::range = 6;
char Cloud::art[A_WIDTH][A_HEIGHT] = {{' ', BL}, {BL, BL}, {BL, BL}, {BL, BL}, {BL, BL}, {BL, BL}, {' ', BL}};

Cloud::Cloud() {
	// To be improved.
	x = 0 - A_WIDTH;
	srand(time(NULL));
	y = rand() % (range + 1); // Creates random y position between 0 and range;
	speed = 1;
}

void Cloud::update() {
	x += speed;
}
