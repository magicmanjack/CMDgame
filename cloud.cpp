#include "cloud.h"
#include <ctime>
#include <iostream>

std::vector<Cloud> Cloud::clouds;

int Cloud::range = 5;
int Cloud::avgSpeed = 1;

Cloud::Cloud() {
	srand(time(NULL)); // Gives a seed number to further "randomize" the output numbers.
	y = (rand() % range) + 1;
	speed = avgSpeed + (rand() % SPD_DEV);
	for(int i = 0; i < 4; i++) {
		art[i] = static_cast<int>(177);
	}
}

void Cloud::update() {
	x += speed;
}
