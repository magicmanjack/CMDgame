#include <vector>

class Cloud {

	public:
		Cloud();
		void update();
		int x, y, speed;
		static std::vector<Cloud*> clouds;
		static int range;
		const static int A_WIDTH = 7, A_HEIGHT = 2;
		static char art[A_WIDTH][A_HEIGHT];

};