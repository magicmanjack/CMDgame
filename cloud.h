#include <vector>

class Cloud {

	public:
		Cloud();
		int x, y, speed;
		static std::vector<Cloud> clouds;
		static int range, avgSpeed;
		static const int SPD_DEV = 2;
		char art[4];
		void update();
};