#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

const int G_WIDTH = 10, G_HEIGHT = 10; // Grid dimensions.
const int FPS = 2;

void update();
void draw();
void displayInput();

struct KeyInfo {
	char c;
	bool pressed;
} kInf;

int main() {
	bool loop = true;
	while(loop) {
		if(kbhit()) { 
			kInf.pressed = true;
			kInf.c = getch(); // Retrieves input.
			// Key press checking.
			if(kInf.c == 27) { // A char with the value of 27 is ESC.
				loop = false; // ESC exits the gameloop and closes the game.
			}
		} else {
			kInf.pressed = false;
		}
		update();
		system("CLS");
		draw();
		Sleep(1000 / FPS);
	}
	return 0;
}

void update() {
	// All game attributes are updated.
	
}

void draw() {
	// The updated game is then pushed to the console.
	string output; // The output is stored in a string before being printed. This makes the game look smoother.
	for(int iy = 0; iy < G_HEIGHT; iy++) {
		for(int ix = 0; ix < G_WIDTH; ix++) {
			output = output + static_cast<char>(219) + static_cast<char>(219); // Draws tiles to string output.
		}
		output = output + "\n";
	}
	cout << output; // Output is finally sent to the console in one heap.
}

void displayInput() {
	cout << "Key ";
	if(kInf.pressed) {
		cout << "pressed";
	} else {
		cout << "released";
	}
	cout << ", Key num: " << static_cast<int>(kInf.c) << endl;
}
	