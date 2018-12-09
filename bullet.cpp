#include "bullet.h"
#include <iostream>
#include <cmath>
#include <vector>

const int PI = 3.14159265359;
std::vector<Bullet*> Bullet::bullets;

Bullet::Bullet(int xOrig, int yOrig, int theta) {
	x = xOrig;
	y = yOrig;
	xVel = sin(theta * (PI / 180.0)) * -1;
	yVel = cos(theta * (PI / 180.0)) * -1;
	c = static_cast<char>(176);
}

Bullet::update() {
	x += xVel;
	y += yVel;
	//std::cout << "xVel: " << xVel << "\nyVel: " << yVel << std::endl;
}