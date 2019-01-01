#include <vector>

class Snow {
	
	public:
		int x, y, fallSpd;
		static std::vector<Snow*> fallingSnow;
		static const int MAX_SPEED = 2;
		char c;
		Snow(int x, int y);
		void update();
};