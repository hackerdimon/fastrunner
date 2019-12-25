#include <iostream>
#include "graphics.h"
#include <fstream>
#include<string>
using namespace std;
#define CELLNUMBER 30
#define CELLSIZE 30
#define UP 1
#define RIGHT 2
#define DOWN 3 
#define LEFT 4
#define UPARROW 72
#define RIGHTARROW 77
#define DOWNARROW 80 
#define LEFTARROW 75
enum modes { MENU, GAME, EDITOR };
//глобальные переменные
int mode = MENU;
int flag = 0;
int row1Line, col1Line, row2Line, col2Line;
int editMap[30][30] = { 0 };
int map[30][30] = { 0 };
int mapNumber = 1;
int direction = RIGHT;
int speed = 150;
int monRow = 0, monCol = 0;
bool fexists(const char* filename)
{
	ifstream ifile(filename);
	return (bool)ifile;
}
template<typename T, int height, int width>
std::ostream& writemap(std::ostream& os, T(&map)[height][width])
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			os << map[i][j] << " ";
		}
		os << "\n";
	}
	return os;
}

void showMenu() {
	setfillstyle(6, GREEN);
	bar(0, 0, 1600, 1000);
	settextstyle(0, 0, 20);
	setcolor(MAGENTA);
	outtextxy(550, 100, "SPRINTER");

	setcolor(LIGHTGRAY);
	settextstyle(0, 0, 17);
	outtextxy(550, 250, "START GAME");


	outtextxy(550, 350, "MAP CREATOR");
	outtextxy(550, 450, "EXIT");
}
void showEditMap() {
	for (int i = 0; i < CELLSIZE; i++) {
		for (int j = 0; j < CELLSIZE; j++) {
			cout << editMap[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl << endl;
}
void showMap(char* header) {
	setfillstyle(1, BLACK);
	bar(0, 0, 1600, 1000);
	settextstyle(0, 0, 20);
	setcolor(GREEN);
	outtextxy(450, 0, header);
	for (int i = 0; i < CELLNUMBER+1; i++) {
		line(80+(i*CELLNUMBER),80,80+(i*CELLNUMBER),80+CELLNUMBER*CELLSIZE);
		line(80, 80+(i*CELLNUMBER), 80+CELLNUMBER*CELLSIZE, 80 + (i*CELLNUMBER));
	}
}
//РИСУЕТ ЗАКРАШЕЕНЫЙ КВАДРАТИК
void wall(int row, int col) {
	int x1, y1,x2,y2;
	x1 = (col * CELLSIZE) + 81;
	y1 = (row * CELLSIZE) + 81;
	setfillstyle(1, YELLOW);
	bar(x1, y1, x1 + 29, y1 + 29);
}
void fill2DArrayFromTxt() {
	ifstream file("map1.txt");
	if (!file.is_open())return;
	for (int i = 0; i < CELLNUMBER; ++i) {
		for (int j = 0; j < CELLNUMBER; ++j) {
			file >> map[i][j];
		}
	}


}

void blackWall(int row, int col) {
	int x1, y1, x2, y2;
	x1 = (col * CELLSIZE) + 81;
	y1 = (row * CELLSIZE) + 81;
	setfillstyle(1, BLACK);
	bar(x1, y1, x1 + 29, y1 + 29);
}
void drawMonster(int row,int col) {
	int x1, y1, x2, y2;
	x1 = (col * CELLSIZE) + 81;
	y1 = (row * CELLSIZE) + 81;
	switch (direction) {
	case UP:
		readimagefile("monsterUp.jpg", x1, y1, x1 + 28, y1 + 28);
		break;
	case RIGHT:
		readimagefile("monsterRight.jpg", x1, y1, x1 + 28, y1 + 28);
		break;
	case DOWN:
		readimagefile("monsterDown.jpg", x1, y1, x1 + 28, y1 + 28);
		break;
	case LEFT:
		readimagefile("monsterLeft.jpg", x1, y1, x1 + 28, y1 + 28);
		break;
	}
}
void initialMonsterPlace() {
	for (int i = 0; i < CELLNUMBER; i++) {
		for (int j = 0; j < CELLNUMBER; j++) {
			if (map[i][j] == 9) {
				monRow = i;
				monCol = j;
				map[i][j] = 0;
				return;
			}
		}
	}
}
//РИСУЕТ ЛИНИЮ ИЗ КУБИКОВ
void drawLine(int row1, int col1, int row2, int col2) {
	if (row2 > row1 && col2==col1) {
		   for (int i = row1; i <= row2; i++) {
			wall(i, col1);
			editMap[i][col1] = 1;
		    }
	}else if (row2 < row1 && col2==col1) {
		for (int i = row2; i <= row1; i++) {
			wall(i, col1);
			editMap[i][col1] = 1;
		}
	}else if (col2 > col1 && row2==row1) {
		for (int i = col1; i <= col2; i++) {
			wall(row1, i);
			editMap[row1][i] = 1;
		}
	}else if (col2 < col1 && row2 == row1) {
		for (int i = col2; i <= col1; i++) {
			wall(row1,i);
			editMap[row1][i] = 1;
		}
	}
	else if (row2 + col2 == row1 + col1) {
		if (row1 > row2) {
			swap(row1, row2);
			swap(col1, col2);
		}
		for (int i = row1, j = col1; i <= row2; i++, j--) {
			wall(i, j);
			editMap[i][j] = 1;
		}
	}
	else if (col1-row1==col2-row2) {
		if (row1 > row2) {
			swap(row1, row2);
			swap(col1, col2);
		}
			
		for (int i = row1, j = col1; i <= row2; i++, j++) {
			wall(i, j);
			editMap[i][j] = 1;
		}
	}
}
void fillMap() {
	for (int i = 0; i < CELLSIZE; i++) {
		for (int j = 0; j < CELLSIZE; j++) {
			if (map[i][j] == 1) {
				wall(i, j);
			}
		}
	}
}
//ОБРАБОТЧИК КЛИКА
void clickChecker() {
	if (ismouseclick(WM_LBUTTONDOWN)) {
		int x, y;
		getmouseclick(WM_LBUTTONDOWN, x, y);
		cout << x << " " << y << endl;
		if (mode == MENU) {
			//MAP CREATOR CLICK
			if (x >= 545 && x <= 940 && y >= 352 && y <= 390) {
				mode = EDITOR;
				showMap("MAP EDITOR");
			}
			else if (x >= 545 && x <= 940 && y >= 250 && y <= 300) {
				mode = GAME;
				showMap("SPRINTER");
				fill2DArrayFromTxt();
				fillMap();
				initialMonsterPlace();
				drawMonster(monRow, monCol);
				getch();
			}
		}
		else if (mode == EDITOR) {
			int row, col;
			row = (y - 80) / CELLSIZE;
			col = (x - 80) / CELLSIZE;
			if ((row <= CELLSIZE-1 && row>=0)&& (col <= CELLSIZE-1 && col>=0)) {
				wall(row, col);
				editMap[row][col] = 1;
			}
		}
		showEditMap();
	}

	else if (ismouseclick(WM_RBUTTONDOWN)) {
		int x, y;
		getmouseclick(WM_RBUTTONDOWN,x,y);
		if (flag == 0) {
			cout << "startLine";
			row1Line = (y - 80) / CELLSIZE;
			col1Line = (x - 80) / CELLSIZE;
			wall(row1Line, col1Line);
			editMap[row1Line][col1Line] = 1;
			flag = 1;
		}
		else {
			cout << "EndLine";
			row2Line = (y - 80) / CELLSIZE;
			col2Line = (x - 80) / CELLSIZE;
			drawLine(row1Line,col1Line,row2Line,col2Line);
			flag = 0;
		}
		
		showEditMap();
	}
}
int main()
{
	initwindow(1600, 1000, "SPRINTER");
	showMenu();

	while (1) {
		clickChecker();
		if (kbhit()) {
			int code = getch();
			if (code == 0) {
				code = getch();
				cout << code << " ";
			}
			while (kbhit()) {
				getch();
			}
			if (mode==EDITOR) {
				if (code == 32) {
					cout << "MAP SAVING" << endl;
					std::fstream of("map" + to_string(mapNumber) + ".txt", std::ios::out | std::ios::app);
					mapNumber++;
					if (of.is_open())
					{
						writemap(of, editMap);
						writemap(std::cout, editMap);
						of.close();
					}
				}
			}
			else if (mode == GAME) {
				if (code == RIGHTARROW) {
					direction = RIGHT;
				}else if (code == UPARROW) {
					direction = UP;
				}else if (code == LEFTARROW) {
					direction = LEFT;
				}else if (code == DOWNARROW) {
					direction = DOWN;
				}
			}
			if (code == 27) {
				return 0;
			}

		}
		if (mode == GAME) {
			if (direction == RIGHT && monCol<CELLNUMBER-1 && map[monRow][monCol+1] == 0){
				blackWall(monRow,monCol);
				monCol++;
				drawMonster(monRow, monCol);
			}else if (direction == UP && monRow > 0 && map[monRow-1][monCol] == 0) {
				blackWall(monRow, monCol);
				monRow--;
				drawMonster(monRow, monCol);
			}
			else if (direction == LEFT && monCol > 0 && map[monRow][monCol-1] == 0) {
				blackWall(monRow, monCol);
				monCol--;
				drawMonster(monRow, monCol);
			}
			else if (direction == DOWN && monRow < CELLNUMBER - 1 && map[monRow + 1][monCol] == 0) {
					blackWall(monRow, monCol);
					monRow++;
					drawMonster(monRow, monCol);
			}
		}
		delay(speed);
	}
	return 0;
}