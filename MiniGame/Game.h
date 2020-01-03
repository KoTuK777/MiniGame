#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "windows.h"
#include <conio.h>
#include <vector>
#include "Player.h"
#include "Enemy.h"
using namespace std;

void showChar(int x, int y, char character) {
	COORD cord;
	//1 unit = 0.5 width and 1 height
	cord.X = x * 2;
	cord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord);
	cout << character;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord);
}

class Game {

	int offsetX = 0;
	int offsetY = 0;

	vector <vector <int> > map {};
	
	void generateWalls(int num) {
		int direction, m, n;
		srand((unsigned)time(NULL));
		for (int i = 0; i < num;) {
			//The start of the main loop
		start:
			direction = rand() % 2;
			m = 2 + rand() % (map[0].size() - 2);
			n = 2 + rand() % (map.size() - 2);
	
			switch (direction) {
				//Vertical
			case 0:
				for (int i = m - 2; i <= m + 2; i++) {
					for (int j = n - 1; j <= n + 1; j++) {
						//Exit from 2 loops to the start of the main loop
						if (map[i][j] == 1) goto start;
					}
				}
				map[m][n] = 1;
				map[m - 1][n] = 1;
				map[m + 1][n] = 1;
				i++;
				break;
	
				//Horizontal
			case 1:
				for (int i = m - 1; i <= m + 1; i++) {
					for (int j = n - 2; j <= n + 2; j++) {
						//Exit from 2 loops to the start of the main loop
						if (map[i][j] == 1) goto start;
					}
				}
				map[m][n] = 1;
				map[m][n - 1] = 1;
				map[m][n + 1] = 1;
				i++;
				break;
			}
		}
	}

	vector <Enemy> enemies;
public:
	Player player;
	Game() {};

	void generateMap(int width, int height, int walls) {
		//Generate frame
		for (int i = 0; i < height; i++) {
			vector<int> row;
			for (int j = 0; j < width; j++) {
				if (i == 0 || j == 0 || i == height - 1 || j == width - 1) row.push_back(1);
				else row.push_back(0);
			}
			map.push_back(row);
		}

		generateWalls(walls);

		
		//map[2][6] = 2;
		//map[7][8] = 1;
		//map[6][7] = 1;
		//map[8][7] = 1;
	}

	

	void showMap() {
		for (size_t i = 0; i < map.size(); i++) {
			for (size_t j = 0; j < map[0].size(); j++) {
				if (map[i][j] == 1) cout << "# ";
				else cout << "  ";
			}
			cout << endl;
		}
	}

	void updateMap() {
		//Clear map
		//Probably need of optimization
		for (size_t i = 0; i < map.size(); i++) {
			for (size_t j = 0; j < map[0].size(); j++) {
				if (map[i][j] != 1 && map[i][j] != 0) map[i][j] = 0;
			}
		}

		//Update position of the player
		map[player.getY()][player.getX()] = player.getId();

		//Update positions of the enemies
		/*for (int i = 0; i < enemies.size(); i++) {
			map[enemies[i].getY()][enemies[i].getX()] = enemies[i].getId();
		}*/
	}

	void showEntities() {
		//Clear player
		showChar(player.getLastX(), player.getLastY(), ' ');

		//Clear enemies
		for (size_t i = 0; i < enemies.size(); i++) {
			showChar(enemies[i].getLastX(), enemies[i].getLastY(), ' ');
		}

		//Show enemies
		for (size_t i = 0; i < enemies.size(); i++) {
			showChar(enemies[i].getX(), enemies[i].getY(), enemies[i].getChar());
		}
		
		//Show player
		showChar(player.getX(), player.getY(), player.getChar());
	}

	void eventsListener() {
		//Moves of player
		player.moves(_getch(), map);
		enemies[0].moves(map);

	}

	/*void testFunc() {
		showMap();
		showEntities();
		cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;

		enemies.push_back(Enemy());
		enemies[0].createViewField(map);
		enemies[0].showFeildOfView();
		enemies[0].calculateNextStep();
	}*/

	void start() {
		enemies.push_back(Enemy());
		showMap();
		showEntities();

		do {
			/*player.showMap(map);*/
			eventsListener();
			updateMap();
			showEntities();

			Sleep(100);
		} while (true);
	}
};