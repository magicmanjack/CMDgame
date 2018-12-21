#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cmath>
#include <vector>
#include <sstream>
#include "bullet.h"
#include "cloud.h"
#include "cloudSpwn.h"
#include "snow.h"

using namespace std;

const int G_WIDTH = 40, G_HEIGHT = 25; // Grid dimensions.
const int FPS = 2;
char grid[G_WIDTH][G_HEIGHT];

const int X_ORI = 20, Y_ORI = 19, PI = 3.14159265359; // The origin point of the line as well as a definition for PI.
int lineTheta = 0; // This is the angle at which the line is pointing from vertical.
CloudSpwn spwn(17); // Cloud spawner obj.
vector<int> collisions; // Keeps track of all the collisions in the map.
int floorHeight[G_WIDTH]; // The floor heights across the X-axis.

bool reloading = false;
const int RLD_TIME = 4; // The amount of time in updates that the cannon reloads.
int rldTimeElapsed = 0;
int score = 0;

void setLevelFloor(int height);
void update();
void draw();
void displayInput();
void fillGrid();
void createLine(int xOrig, int yOrig, int theta);
void manageClouds();
void manageBullets(); 
void manageSnow();

struct KeyInfo {
	char c;
	bool pressed;
} kInf;

int main() {
	bool loop = true;
	fillGrid();
	setLevelFloor(G_HEIGHT - 1);
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
	fillGrid(); // Clears the grid
	if(kInf.pressed) {
		if(kInf.c == 97) { // 'a' pressed.
			lineTheta+=10; // Adds 10 degress to the angle.
		}
		if(kInf.c == 100) { // 'd' pressed.
			lineTheta-=10; // Subtracts 10 degress from the angle.
		}
		if(kInf.c == 32 && !reloading) {
			Bullet::bullets.push_back(new Bullet(X_ORI, Y_ORI, lineTheta)); // Creates a new bullet.
			reloading = true;
		}
	}
	if(abs(lineTheta) >= 360) {
		lineTheta = 0;
	}
	if(!reloading) {
		createLine(X_ORI, Y_ORI, lineTheta); // Creates the line on the grid.
	}
	manageClouds();
	manageBullets();
	manageSnow();
}

void manageClouds() { // Manages all cloud activity.
	rldTimeElapsed++;
	if(rldTimeElapsed >= RLD_TIME) {
		rldTimeElapsed = 0;
		reloading = false;
	}
	spwn.update();
	vector<Cloud*> c = Cloud::clouds;
	vector<int> toRemove; // Contains the index of elements that are to be removed.
	for(int i = 0; i < c.size(); i++) {
		for(int j = 0; j < collisions.size(); j++) {
			if(j % 2 == 0) {
				if((c[i] -> x) + (c[i] -> targetX) == collisions[j] && (c[i] -> y) + (c[i] -> targetY) == collisions[j + 1]) {
					c[i] -> art[c[i] -> targetX][c[i] -> targetY] = ' ';
					collisions.erase(collisions.begin() + j); // Erases X element.
					collisions.erase(collisions.begin() + j); // Erases Y element.
					j -= 2; // Sets back index because of the changed order of the vector.
				}
			}
		}
		c[i] -> update(); // Updates the cloud.
		if(c[i] -> x >= G_WIDTH) { // If the clouds exit the screen boundaries.
			toRemove.push_back(i);
		} else {
			for(int ix = 0; ix < Cloud::A_WIDTH; ix++) {
				for(int iy = 0; iy < Cloud::A_HEIGHT; iy++) {
					if(!((c[i] -> x) + ix >= G_WIDTH) && !((c[i] -> x) + ix < 0)) {
						grid[(c[i] -> x) + ix][(c[i] -> y) + iy] = c[i] -> art[ix][iy]; // The cloud is drawn to the grid.
					}
				}
			}
		}
	}
	for(int i = 0; i < toRemove.size(); i++) {
		delete c[toRemove[(toRemove.size() - 1) - i]]; // Deallocates the memory pointed to.
		Cloud::clouds.erase(Cloud::clouds.begin() + toRemove[(toRemove.size() - 1) - i]); // Erases the element from the vector list.
	}
}

void manageBullets() {
	vector<int> toRemove;
	for(int i = 0; i < Bullet::bullets.size(); i++) {
		Bullet *b = Bullet::bullets[i];
		b -> update();
		if(b -> x < 0 || b -> x > G_WIDTH || b -> y < 0 || b -> y > G_HEIGHT) { // If the bullet goes outside the grid.
			toRemove.push_back(i);
		}else {
			if(grid[static_cast<int>(b -> x)][static_cast<int>(b -> y)] == static_cast<char>(157)) {
				score++;
				toRemove.push_back(i);
				b -> c = '#';
				collisions.push_back(static_cast<int>(b -> x));
				collisions.push_back(static_cast<int>(b -> y));
			}
			grid[static_cast<int>(b -> x)][static_cast<int>(b -> y)] = b->c;
		}
	}	
	for(int i = 0; i < toRemove.size(); i++) {
		delete Bullet::bullets[toRemove[(toRemove.size() - 1) - i]]; // Deallocates the memory that is being pointed to.
		Bullet::bullets.erase(Bullet::bullets.begin() + toRemove[(toRemove.size() - 1) - i]); // The pointer is then removed from the list of bullets.
	}
}

void manageSnow() {
	vector<Snow*> snw = Snow::fallingSnow;
	vector<int> toRemove;
	for(int i = 0; i < snw.size(); i++) {
		if(snw[i] -> y >= floorHeight[snw[i] -> x]) {
			snw[i] -> harden();
			floorHeight[snw[i] -> x] = (snw[i] -> y) - 1; // Updates the floor height.
		} 
		if(((snw[i] -> fallSpd) > 1) && (snw[i] -> y) + (snw[i] -> fallSpd) >= floorHeight[snw[i] -> x]) {
			snw[i] -> y = floorHeight[snw[i] -> x]; // Snaps to position.
		} else {
			snw[i] -> update();
		}
		if(snw[i] -> x < 0 || snw[i] -> x >= G_WIDTH || snw[i] -> y < 0 || snw[i] -> y >= G_HEIGHT) {
			toRemove.push_back(i);
		}else {
			grid[snw[i] -> x][snw[i] -> y] = snw[i] -> c;
		}
	}
	for(int i = 0; i < toRemove.size(); i++) {
		delete snw[toRemove[(toRemove.size() - 1) - i]];
		Snow::fallingSnow.erase(Snow::fallingSnow.begin() + toRemove[(toRemove.size() - 1) - i]);
	} // Possibly a more efficient way would be: while(toRemove.size() > 0) { delete first element }
}

void createLine(int xOrig, int yOrig, int theta) {
	double dx, dy; // The delta from the origin point.
	for(int iy = 0; iy < G_HEIGHT; iy ++) { // For every section of the grid at Y.
		dy = iy - yOrig; // Delta Y from the origin Y is calculated.
		dx = xOrig + round(tan((theta * PI) / 180.0) * dy); // The Delta X is then found using trigonometry.  
		if(dx >= 0 && dx < G_WIDTH) { // To prevent breaking of array bounderies.
			if((abs(theta) <= 90 || abs(theta) > 270) && dy <= 0) {
				grid[static_cast<int>(dx)][iy] = static_cast<char>(46); // Creates empty space which forms part of the line. Only draws the top.
			}
			if((abs(theta) > 90 && abs(theta) <= 270) && dy >= 0) {
				grid[static_cast<int>(dx)][iy] = static_cast<char>(46); // Creates empty space which forms part of the line. Only draws the bottom.
			}
		}
	}
}

void draw() {
	// The updated game is then pushed to the console.
	ostringstream oss; // The output is put into a string stream before being printed. This makes the game look smoother.
	//system("color 11");
	for(int iy = 0; iy < G_HEIGHT; iy++) {
		oss << "|";
		for(int ix = 0; ix < G_WIDTH; ix++) {
			oss << grid[ix][iy];
		}
		oss << "|\n";
	}
	oss << "Score: " << score;
	cout << oss.str(); // Output is finally sent to the console in one heap.
}

void fillGrid() {
	for(int iy = 0; iy < G_HEIGHT; iy++) {
		for(int ix = 0; ix < G_WIDTH; ix++) {
			grid[ix][iy] = ' ';
		}
	}
}

void setLevelFloor(int height) {
	for(int i = 0; i < G_WIDTH; i++) {
		floorHeight[i] = height;
	}
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
	