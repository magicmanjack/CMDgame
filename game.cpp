#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

const int G_WIDTH = 5, G_HEIGHT = 5; // Grid dimensions.
const int FPS = 2;

void update();

int main() {
	bool loop = true;
	while(loop) {
		if(kbhit()) {
			char c = getch();
			// Key press checking.
			if(c == 27) {
				loop = false; // ESC exits the gameloop and closes the game.
			}
		}
		update();
		Sleep(1000 / FPS);
	}
	return 0;
}

void update() {
	// All game attributes are updated.
	
}
	