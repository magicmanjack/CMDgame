#include <vector>

class Snow {
	
	public:
		int x, y, fallSpd;
		static std::vector<Snow*> fallingSnow;
		static char c;
		Snow(int x, int y);
		void update();
	
};