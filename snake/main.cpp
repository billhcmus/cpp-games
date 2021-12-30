#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>
using namespace std;

bool gameOver;
const int Width = 50, Height = 25;
int x, y, fruitX, fruitY, score;
int prevX, prevY;
int length;
int SnakeX[100], SnakeY[100];
enum Status { STOP = 0,  LEFT, RIGHT, UP, DOWN };
Status stt;
void clrscr() {
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

void ShowCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
	SetConsoleCursorInfo(handle, &cursor);
}

void Setup() {
	gameOver = false;
	stt = STOP;
	score = 0;
	x = 20, y = 20;
	SnakeX[0] = x;
	SnakeY[0] = y;
	length = 0;
	fruitX = rand() % (Width - 1) + 1;
	fruitY = rand() % (Height - 1) + 1;
}

void Draw() {
	clrscr();
	ShowCur(false);
	for (int i = 0; i < Width; i++) {
		cout << "#";
	}
	cout << endl;
	for (int i = 1; i < Height - 1; i++) {
		for (int j = 0; j < Width; j++) {
			if (j == 0) {
				cout << "#";
			}
			else if (j == Width - 1) {
				cout << "#";
			}
			else {
				if (i == y && j == x) {
					putchar('O');
				}
				else if (i == fruitY && j == fruitX) {
					putchar('X');
				}
				else {
					bool check = true;
					for (int t = 0; t < length; t++) {
						if (SnakeY[t] == i && SnakeX[t] == j) {
							check = false;
							putchar('o');
						}
					}
					if (check)
						putchar(' ');
				}
			}
		}
		cout << endl;
	}
	for (int i = 0; i < Width; i++) {
		cout << "#";
	}
	cout << endl << "SCORE: " << score;
}

void Input() {
	if (kbhit()) {
		switch (getch()) {
		case 'a':
			stt = LEFT;
			x--;
			break;
		case 'd':
			stt = RIGHT;
			x++;
			break;
		case 'w':
			stt = UP;
			y--;
			break;
		case 's':
			stt = DOWN;
			y++;
			break;
		case 'x':
			gameOver = true;
			break;
		}
	}
}

void Process() {
	prevX = SnakeX[0];
	prevY = SnakeY[0];
	SnakeX[0] = x;
	SnakeY[0] = y;
	int prev2X, prev2Y;
	for (int i = 1; i < length; i++) {
		prev2X = SnakeX[i];
		prev2Y = SnakeY[i];
		SnakeX[i] = prevX;
		SnakeY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	if (x == fruitX && y == fruitY) {
		score++;
		length++;
		fruitX = rand() % (Width - 1) + 1;
		fruitY = rand() % (Height - 1) + 1;
	}
	if (Height - 1 == y || 0 == y || Width - 1 == x || 0 == x) {
		gameOver = true;
	}
	switch (stt) {
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	}
}


int main() {
	Setup();
	while (!gameOver) {
		srand(time(0));
		Draw();
		Input();
		Process();
		Sleep(200);
	}
	system("pause");
	return 0;
}