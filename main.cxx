#include <iostream>
#include "graphics.h"
using namespace std;
#define CELLNUMBER 30
#define CELLSIZE 30
enum modes { MENU, GAME, EDITOR };
//глобальные переменные
int mode = MENU;
int flag = 0;
int row1Line, col1Line, row2Line, col2Line;
int editMap[30][30] = { 0 };
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
void showEditor() {
	setfillstyle(1, BLACK);
	bar(0, 0, 1600, 1000);
	settextstyle(0, 0, 20);
	setcolor(GREEN);
	outtextxy(450, 0, "MAP EDITOR");
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
//РИСУЕТ ЛИНИЮ ИЗ КУБИКОВ
void drawLine(int row1, int col1, int row2, int col2) {
	if (row2 > row1) {
		for (int i = row1; i <=row2; i++) {
			wall(i, col1);
			editMap[i][col1] = 1;
		}
	}else if (row2 < row1) {
		for (int i = row2; i <= row1; i++) {
			wall(i, col1);
			editMap[i][col1] = 1;
		}
	}else if (col2 > col1) {
		for (int i = col1; i <= col2; i++) {
			wall(row1, i);
			editMap[row1][i] = 1;
		}
	}else if (col2 < col1) {
		for (int i = col2; i <= col1; i++) {
			wall(row1,i);
			editMap[row1][i] = 1;
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
				showEditor();

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
		getmouseclick(WM_RBUTTONDOWN, x, y);
		cout << x << " " << y;
		if (flag == 0) {
			row1Line = (y - 80) / CELLSIZE;
			col1Line = (x - 80) / CELLSIZE;
			wall(row1Line, col1Line);
			editMap[row1Line][col1Line] = 1;
			flag = 1;
		}
		else {
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
		delay(200);
	}
	getch();
	return 0;
}