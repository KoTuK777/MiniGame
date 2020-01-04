#pragma once
#include <iostream>
#include <vector>

using namespace std;
class Player {
private:
	int x = 1;
	int	y = 1;
	int lastX = -1;
	int	lastY = -1;
	int id = 2;
	char character = '*';
public:
	Player() {};

	Player(int x, int y, char character) {
		this->x = x;
		this->y = y;
		this->character = character;

		/*for (int i = 0; i < nRow; i++) {
			for (int j = 0; j < nCol; j++) {
				if (p[i][j] == 1) cout << "# ";
				else cout << "  ";
			}
			cout << endl;
		}*/
	}

	void moves(int key, vector<vector<int>>& map) {
		lastX = x;
		lastY = y;
		bool up = false, left = false, down = false, right = false;

		switch (key) {
			//w = 119; W = 87; Up arrow = 72
		case 119: case 87: case 72:
			up = true; break;
			//a = 97; A = 65; Left arrow = 75
		case 97: case 65: case 75:
			left = true; break;
			//s = 115; S = 83; Down arrow = 80
		case 115: case 83: case 80:
			down = true; break;
			//d = 100; D = 68; Right arrow = 77
		case 100: case 68: case 77:
			right = true; break;
		}

		// Moves
		if (right && map[y][x + 1] != 1) {
			x++;
		}
		if (left && map[y][x - 1] != 1) {
			x--;
		}
		if (up && map[y - 1][x] != 1) {
			y--;
		}
		if (down && map[y + 1][x] != 1) {
			y++;
		}
	}

	char getChar() {
		return character;
	}
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}
	int getId() { 
		return id;
	}
	int getLastX() {
		return lastX;
	}
	int getLastY() {
		return lastY;
	}
};