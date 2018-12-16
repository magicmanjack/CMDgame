#include "cloud.h"
#include <vector>
#include <time.h>
#include <stdlib.h>

std::vector<Cloud*> Cloud::clouds;
int Cloud::range = 6;

Cloud::Cloud() {
	// To be improved.
	x = 0 - A_WIDTH;
	srand(time(NULL));
	y = rand() % (range + 1); // Creates random y position between 0 and range;
	speed = 1;
	srand(time(NULL));
	Cloud::art[rand() % A_WIDTH][rand() % A_HEIGHT] = static_cast<char>(157);
}

void Cloud::update() {
	x += speed;
}
