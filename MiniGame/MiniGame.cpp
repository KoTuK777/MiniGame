#include <iostream>
#include <cstdlib>
#include <ctime>
#include "windows.h"
#include <conio.h>
#include <vector>
#include "Game.h"

using namespace std;


//void clearRow(int row) {
//	COORD cord;
//	cord.X = 0;
//	cord.Y = row;
//	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord);
//	for (int i = 0; i < 20; i++) cout << "  ";
//}
//
//void showCord(int x, int y, int row) {
//	COORD cord;
//	cord.X = 1;
//	cord.Y = row + 1;
//	clearRow(cord.Y);
//	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord);
//	cout << "x: " << x << "  y: " << y << endl;
//}

int main()
{

	Game game;
	game.generateMap(20, 20, 0);
	game.start();
	//game.testFunc();

	system("pause >> NULL");
	return 0;
}