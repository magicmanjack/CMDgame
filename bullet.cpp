#include "bullet.h"
#include <iostream>
#include <cmath>

const int PI = 3.14159265359;

int x, y, xVel, yVel;
const int speed = 1;
char c = static_cast<char>(177);

Bullet::Bullet(int xOrig, int yOrig, int theta) {
	x = xOrig;
	y = yOrig;
	xVel = sin(theta * (PI / 180)) * speed;
	yVel = cos(theta * (PI / 180)) * speed;
}

Bullet::update() {
	x += xVel;
	y += yVel;
}