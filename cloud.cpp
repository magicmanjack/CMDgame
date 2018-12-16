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
	targetX = rand() % A_WIDTH;
	targetY = rand() % A_HEIGHT;
	Cloud::art[targetX][targetY] = static_cast<char>(157); // Creates a target on the cloud.
}

void Cloud::update() {
	x += speed;
}
