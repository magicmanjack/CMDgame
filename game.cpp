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
bool gameOver;

const int PLYR_LOCAL_X = -1, PLYR_LOCAL_Y = -1; // The player X and Y relative to the xOri and yOri.
const int PLYR_WIDTH = 3, PLYR_HEIGHT = 3; // The player sprite width and height.
char plyrSprite[3][3] = {{' ', '/', '='}, {'_', 'o', '='}, {' ', '\\', '='}};
const int HBOX_WIDTH = 1, HBOX_HEIGHT = 1; // Hitbox width and height (extruding from the player X and Y). 

const int PI = 3.14159265359; // A definition for PI.
int lineTheta = 0, xOri = 20, yOri = 19, speed = 2; // This is the angle at which the line is pointing from vertical as well as the origin point of the line. The speed is the speed that the turret can move.
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
void createLine(int xOri, int yOri, int theta);
void manageClouds();
void manageBullets(); 
void manageSnow();
void drawPlyr();
void reset();
void displayText(string s, int x, int y);
bool collides(int x, int y);

void showIntro() {
	displayText("MISSION: TERMINATE ANY SNOW SPAWNERS", 2, 2);
	displayText("SPOTTED IN THE CLOUDS. THE SHIPS WEAK", 2, 3);
	displayText("SHEILDING MEANS AVOID FALLING HAIL!", 2, 4);
	string s = "TARGET: ";
	s += static_cast<char>(157);
	displayText(s , 2, 6);
	displayText("   __", 0, 10);
	displayText("  /  \\ -> CONTROL TURRET WITH A AND D", 0, 11);
	displayText(" /  _ \\           -> SHOOT WITH SPACE", 0, 12);
	displayText(" | (%)|", 0, 13);
	displayText(" |    | -> SIDE THRUSTERS F AND G", 0, 14);
	displayText(" ======", 0, 15);
	displayText("(ESC IS TO ABORT MISSION)", 0, 17);
	draw();
	cout << "\n\n";
	system("pause");
}

struct KeyInfo {
	char c;
	bool pressed;
} kInf;

int main() {
	bool loop = true;
	gameOver = false;
	fillGrid();
	setLevelFloor(G_HEIGHT - 1);
	
	// Intro screen goes here...
	showIntro();
	//
	
	while(loop) {
		if(kbhit()) { 
			kInf.pressed = true;
			kInf.c = getch(); // Retrieves input.
			while(kbhit()) {
				getch(); // Clears all extra in the input buffer.
			}
			// Key press checking.
			if(kInf.c == 27) { // A char with the value of 27 is ESC.
				loop = false; // ESC exits the gameloop and closes the game.
			} else if(gameOver) {
				reset();
			}
		} else {
			kInf.pressed = false;
		}
		if(!gameOver) {
			update();
			system("CLS");
			draw();
			Sleep(1000 / FPS);
		}
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
		if(kInf.c == 102 && xOri - speed >= 0) { // 'f' pressed.
			xOri-=speed;
		}
		if(kInf.c == 103 && xOri + speed < G_WIDTH) { // 'g' pressed.
			xOri+=speed;
		}
		if(kInf.c == 32 && !reloading) {
			Bullet::bullets.push_back(new Bullet(xOri, yOri, lineTheta)); // Creates a new bullet.
			reloading = true;
		}
	}
	if(abs(lineTheta) >= 360) {
		lineTheta = 0;
	}
	if(!reloading) {
		createLine(xOri, yOri, lineTheta); // Creates the line on the grid.
	}
	manageClouds();
	manageSnow();
	manageBullets();
	drawPlyr();
	
	if(gameOver) {
		fillGrid();
		displayText("GAME OVER", 15, 12);
		system("CLS");
		draw();
	}
}

void manageClouds() { // Manages all cloud activity.
	spwn.update();
	vector<Cloud*> c = Cloud::clouds;
	vector<int> toRemove; // Contains the index of elements that are to be removed.
	for(int i = 0; i < c.size(); i++) {
		if(collides((c[i] -> x) + (c[i] -> targetX), (c[i] -> y) + (c[i] -> targetY))) {
			c[i] -> art[c[i] -> targetX][c[i] -> targetY] = ' ';
			c[i] -> disabled = true;
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
	if(reloading) {
		rldTimeElapsed++;
	}
	if(rldTimeElapsed >= RLD_TIME) {
		rldTimeElapsed = 0;
		reloading = false;
	}
	vector<int> toRemove;
	for(int i = 0; i < Bullet::bullets.size(); i++) {
		Bullet *b = Bullet::bullets[i];
		b -> update();
		if(b -> x < 0 || b -> x > G_WIDTH || b -> y < 0 || b -> y > G_HEIGHT) { // If the bullet goes outside the grid.
			toRemove.push_back(i);
		}else {
			char chTouching = grid[static_cast<int>(b -> x)][static_cast<int>(b -> y)];
			if(chTouching == static_cast<char>(157)) {
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
		if(snw[i] -> y == floorHeight[snw[i] -> x]) {
			snw[i] -> fallSpd = 0;
			snw[i] -> c = static_cast<char>(220);
			if(grid[snw[i] -> x][floorHeight[snw[i] -> x]] == static_cast<char>(220)) {
				snw[i] -> c = static_cast<char>(219);
				floorHeight[snw[i] -> x] = (snw[i] -> y) - 1; // Updates the floor height.
			}
		} 
		if(((snw[i] -> fallSpd) > 1) && (snw[i] -> y) + (snw[i] -> fallSpd) >= floorHeight[snw[i] -> x]) {
			snw[i] -> y = floorHeight[snw[i] -> x]; // Snaps to position.
		} else {
			snw[i] -> update();
		}
		if(snw[i] -> x < 0 || snw[i] -> x >= G_WIDTH || snw[i] -> y < 0 || snw[i] -> y >= G_HEIGHT) {
			toRemove.push_back(i);
		} else {
			grid[snw[i] -> x][snw[i] -> y] = snw[i] -> c;
		}
		if((snw[i] -> x) >= xOri - HBOX_WIDTH && (snw[i] -> x) <= xOri + HBOX_WIDTH && (snw[i] -> y) >= yOri - HBOX_HEIGHT && (snw[i] -> y) <= yOri + HBOX_HEIGHT) {
			toRemove.push_back(i);
			gameOver = true;
		}
	}
	for(int i = 0; i < toRemove.size(); i++) {
		delete snw[toRemove[(toRemove.size() - 1) - i]];
		Snow::fallingSnow.erase(Snow::fallingSnow.begin() + toRemove[(toRemove.size() - 1) - i]);
	} // Possibly a more efficient way would be: while(toRemove.size() > 0) { delete first element }
}

void createLine(int xOri, int yOri, int theta) {
	double dx, dy; // The delta from the origin point.
	for(int iy = 0; iy < G_HEIGHT; iy ++) { // For every section of the grid at Y.
		dy = iy - yOri; // Delta Y from the origin Y is calculated.
		dx = xOri + round(tan((theta * PI) / 180.0) * dy); // The Delta X is then found using trigonometry.  
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

void drawPlyr() {
	for(int ix = 0; ix < PLYR_WIDTH; ix++) {
		for(int iy = 0; iy < PLYR_HEIGHT; iy++) {
			int trueX = xOri + ix + PLYR_LOCAL_X;
			int trueY = yOri + iy + PLYR_LOCAL_Y;
			
			if(trueX >= 0 && trueX < G_WIDTH && trueY >= 0 && trueY < G_HEIGHT) {
				grid[trueX][trueY] = plyrSprite[ix][iy];
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

bool collides(int x, int y) {
	bool collision = false;
	for(int j = 0; j < collisions.size(); j++) {
			if(j % 2 == 0) {
				if(x == collisions[j] && y == collisions[j + 1]) {
					collisions.erase(collisions.begin() + j); // Erases X element.
					collisions.erase(collisions.begin() + j); // Erases Y element.
					j -= 2; // Sets back index because of the changed order of the vector.
					collision = true;
				}
			}
	}
	return collision;
}

void reset() {
	xOri = 20;
	yOri = 19;
	score = 0;
	Cloud::clouds.clear();
	Snow::fallingSnow.clear();
	Bullet::bullets.clear();
	gameOver = false;
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

void displayText(string s, int x, int y) {
	for(int i = 0; i < s.length(); i++) {
		if(x + i >= 0 && x + i < G_WIDTH && y >= 0 && y < G_HEIGHT) {
			grid[x + i][y] = s[i];
		}
	}
}
	