#pragma once
class Enemy {
	int x = 10;
	int	y = 10;
	int id = 3;
	int lastX = -1;
	int	lastY = -1;
	char character = '0';

	int radiusOfView = 5;

	vector<vector<float>> fieldOfView;

	float up = 0, down = 0, right = 0, left = 0;
	bool Up = true, Down = true, Right = true, Left = true;

	//Enemy setting
	float playerNum = 50;
	float voidNum = 0.9;
	int mulCellFactor = 2;

public:

	void initFieldOfView() {
		fieldOfView.clear();

		for (int i = 0; i < 1 + radiusOfView * 2; i++) {
			vector<float> row;
			for (int j = 0; j < 1 + radiusOfView * 2; j++) {
				row.push_back(0);
			}
			fieldOfView.push_back(row);
		}
	}
	
	void createViewField(vector<vector<int>>& map) {
		//Here we generate new map for enemies to calculate their moves
		/*
		 *
		 *
		*/
		initFieldOfView();
		
		for (int i = y - radiusOfView, m = 0; i <= y + radiusOfView; i++, m++) {
			for (int j = x - radiusOfView, n = 0; j <= x + radiusOfView; j++, n++) {

				if (i < 0 || j < 0 || i >= map.size() || j >= map.size()) continue;

				switch (map[i][j])
				{
				//Ми перебираємо id на карті і виставляємо коефіцієнти для вичислень
				//Walls
				case 1:
					fieldOfView[m][n] = -1;
					break;
				//Void
				case 0:
					if (i == y - radiusOfView || j == x - radiusOfView || i == y + radiusOfView || j == x + radiusOfView) fieldOfView[m][n] = voidNum;
					else fieldOfView[m][n] = 0;
					break;
				//Player
				case 2:
					fieldOfView[m][n] = playerNum;
					break;
				//Enemy
				case 3:
					fieldOfView[m][n] = -2;
					break;
				default:
					break;
				}						
			}
		}
	};

	void calculateNextStep() {
		//Тут ми вираховуємо середнє арифметичне 3 попередніх клітинок і записуєм ці данні в наступну клітинку
		//Тобто створюємо пірамідку з попередніх значень, також для тих клітинок, що знаходяться поруч множаться на 
		//коефіцієнт для того щоб ворог не йшов на стінку
		//Приклад:
		//| 1 | 2 | 3 |
		//	  | 2 |
		size_t center = fieldOfView.size() / 2;

		//Перевірка ближніх клітинок на стіни
		if (fieldOfView[center - 1][center] == -1) Up = false;
		if (fieldOfView[center + 1][center] == -1) Down = false;
		if (fieldOfView[center][center + 1] == -1) Right = false;
		if (fieldOfView[center][center - 1] == -1) Left = false;

		//Up & Down
		size_t size = fieldOfView[0].size() - 1;
		size_t start = 1;

		for (size_t i = 0; i < fieldOfView.size() / 2; i++) {
			for (size_t j = start; j < size; j++) {
				int k = fieldOfView.size() - 1 - i;
				if (size - start < 3) {
					if (Up) up = (fieldOfView[i][j - 1] + fieldOfView[i][j] * mulCellFactor + fieldOfView[i][j + 1]) / 3;
					else up = -999;

					if (Down) down = (fieldOfView[k][j - 1] + fieldOfView[k][j] * mulCellFactor + fieldOfView[k][j + 1]) / 3;
					else down = -999;
					break;
				}
				else {
					//Up
					if (fieldOfView[i + 1][j] != -1 && fieldOfView[i + 1][j] != playerNum && Up) {
						fieldOfView[i + 1][j] = (fieldOfView[i][j - 1] + fieldOfView[i][j] + fieldOfView[i][j + 1]) / 3;
					}
					//Down
					if (fieldOfView[k - 1][j] != -1 && fieldOfView[k - 1][j] != playerNum && Down) {
						fieldOfView[k - 1][j] = (fieldOfView[k][j - 1] + fieldOfView[k][j] + fieldOfView[k][j + 1]) / 3;
					}
				}
			}
			size--;
			start++;
		}

		size = fieldOfView.size() - 1;
		start = 1;

		//Left & Right
		for (size_t i = 0; i < fieldOfView.size() / 2; i++) {
			for (size_t j = start; j < size; j++) {
				int k = fieldOfView[0].size() - 1 - i;
				if (size - start < 3) {
					if (Left) left = (fieldOfView[j - 1][i] + fieldOfView[j][i] * mulCellFactor + fieldOfView[j + 1][i]) / 3;
					else left = -999;
					if (Right) right = (fieldOfView[j - 1][k] + fieldOfView[j][k] * mulCellFactor + fieldOfView[j + 1][k]) / 3;
					else right = -999;
					break;
				}
				else {
					//Left
					if (fieldOfView[j][i + 1] != -1 && fieldOfView[j][i + 1] != playerNum && Left) {						
						fieldOfView[j][i + 1] = (fieldOfView[j - 1][i] + fieldOfView[j][i] + fieldOfView[j + 1][i]) / 3;
					}
					//Right
					if (fieldOfView[j][k - 1] != -1 && fieldOfView[j][k - 1] != playerNum && Right) {
						fieldOfView[j][k - 1] = (fieldOfView[j - 1][k] + fieldOfView[j][k] + fieldOfView[j + 1][k]) / 3;
					}
				}		
			}
			size--;
			start++;
		}

		

		Up = true, Down = true, Right = true, Left = true;
		//Test
		/*cout << endl << endl;

		cout << "Up: " << up << endl;
		cout << "Down: " << down << endl;
		cout << "Right: " << right << endl;
		cout << "Left: " << left << endl;*/
	}

	void takeStep() {
		lastX = x;
		lastY = y;
		vector<float> directions{ up, down, right, left };
		vector<float> ways;
		float max = directions[0];
		int dir = -1, maxI = 0;

		for (int i = 1; i < directions.size(); i++) {
			if (directions[i] > max) {
				max = directions[i];
				maxI = i;
			}
		}
		ways.push_back(maxI);
		dir = ways[0];
		//Test
		for (int i = 0; i < directions.size(); i++) {
			if (i == maxI) continue;
			if (directions[i] == max)  ways.push_back(i);
		}
		srand(time(NULL));
		dir = ways[rand() % ways.size()];

		if (dir == 0) y--;
		if (dir == 1) y++;
		if (dir == 2) x++;
		if (dir == 3) x--;

		//switch (maxI)
		//{
		////Up
		//case 0:
		//	y--; break;
		////Down
		//case 1:
		//	y++; break;
		////Right
		//case 2:
		//	x++; break;
		////Left
		//case 3:
		//	x--; break;
		//default:
		//	break;
		//}
	}

	void moves(vector<vector<int>>& map) {
		createViewField(map);
		calculateNextStep();
		takeStep();
	}


	//Test

	void setCursor(int x, int y) {
		COORD cord;
		//1 unit = 0.5 width and 1 height
		cord.X = x * 2;
		cord.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord);
	}
	void showFeildOfView() {
		int w = 21;
		for (size_t i = 0; i < fieldOfView.size(); i++)	{
			setCursor(w, i + 1);

			for (size_t j = 0; j < fieldOfView[0].size(); j++) {
				if (fieldOfView[i][j] == -1) cout << "# ";
				else if (fieldOfView[i][j] == playerNum) cout << "* ";
				else if (fieldOfView[i][j] == -2 || fieldOfView.size() / 2 == i && fieldOfView[0].size() / 2 == j) cout << "0 ";
				else if (fieldOfView[i][j] == (float)voidNum) cout << "! ";
				else cout << "  ";
			}
			
		}

		setCursor(w, fieldOfView.size() + 1);
		cout << "Up: " << up << endl;
		setCursor(w, fieldOfView.size() + 2);
		cout << "Down: " << down << endl;
		setCursor(w, fieldOfView.size() + 3);
		cout << "Right: " << right << endl;
		setCursor(w, fieldOfView.size() + 4);
		cout << "Left: " << left << endl;
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