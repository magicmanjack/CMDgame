#include <vector>

class Bullet {
	
	public:
		Bullet(int xOrig, int yOrig, int theta);
		update();
		double x, y, xVel, yVel;
		char c;
		static std::vector<Bullet*> bullets;
};