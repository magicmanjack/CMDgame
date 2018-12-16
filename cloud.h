#include <vector>

#define BL static_cast<char>(176)

class Cloud {

	public:
		Cloud();
		void update();
		int x, y, speed, targetX, targetY;
		static std::vector<Cloud*> clouds;
		static int range;
		const static int A_WIDTH = 7, A_HEIGHT = 2;
		char art[A_WIDTH][A_HEIGHT] = {{' ', BL}, {BL, BL}, {BL, BL}, {BL, BL}, {BL, BL}, {BL, BL}, {' ', BL}};

};