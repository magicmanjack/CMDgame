#include <iostream>
#include <conio.h>
#include <windows.h>

int main() {
	while(1) {
		if(kbhit()) {
			char c = getch();
			while(kbhit()) {
				getch();
			}
			if(c == 27) {
				break;
			}
			std::cout << static_cast<int>(c) << std::endl;
		}
		Sleep(1000);
	}
	return 0;
}