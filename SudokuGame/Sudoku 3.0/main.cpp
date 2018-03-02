#include "console.h"
#define Number 7
#define Level 5
#define Open 2
struct LIST{
	char Name[20];
	int Time;
};
LIST Player[5];
int count = 0;
clock_t temp_time = 0;
char Level_Dif[15];
static int Signed[9][9]; //Mảng đánh dấu đường xuyên
static int Matrix[9][9];
static int Start[9][9]; // Mảng khởi tạo
static int Temp[9][9]; // Mảng trung gian
int Result[9][9]; // Mảng chứa kết quả
int Color[9][9]; //Mảng màu
typedef char Func[31];
Func Menu[] = { "RESUME", "NEW GAME", "OPEN GAME", "HIGHT SCORE", "HELP", "ABOUT", "QUIT" };
Func Menu_Level[] = { "Easy", "Medium", "Hard", "Very Hard", "Oblique Line" };
Func Menu_Open[] = { "Oblique Line", "Grid Available" };
bool checkNumber(int i, int j, int x); // Kiểm tra số 
void RandomVal(); // Random số từ 1 -> 9 trên đường chéo chính
void InitMatrix_Oblique(int num);
bool RandomVal_Oblique_Line(); // Random cho đường xuyên
bool Check_Cell_Next(int i, int j);
void Cell_Next(int &i, int &j);
bool is_Full_Cell(int i, int j);
int Solve_for_Init(int k); // Hàm giải ô số cho phần khởi tạo dữ kiện
int findLast_for_Init();
bool checkNumber_for_Init(int i, int j, int x);
void InitMatrix(int num); // Khởi tạo ô số
int findLast();
bool SolutionSudoku(int k);
void DrawGrid(int a[][9], int x, int y);
void ResetMatrix(int a[][9]);
bool isNumber(char c);
void NewGame();
bool isFillFull();
bool Win_or_Lost();
void Hint();
void CopyMatrix();
void SaveResult();
int end_of_line_or_file(char c);
int readLine(char line[], int MaxLine, FILE *FileIn);
void WelcomeGame(char *fileName);
int MenuGame(char *fileName, int x, int y);
int MenuLevel(char *fileName, int x, int y);
void HelpGame(char *fileName);
void AboutGame(int x, int y, char *fileName);
void SaveGame(int a[][9], char *fileName);
bool openSaveGame(int a[][9], char *fileName);
void SaveResult_for_Resum();
void ResumGame();
void OpenHightScore(char *fileName);
bool OpenGrid(int a[][9], char *patch); // Mở lưới Sudoku có sẵn
void OpenGame(); // Chơi lưới Sudoku mở sẵn
bool Open_Oblique(char *patch);
void Random_Oblique();
int Menu_Open_ObliqueLine_or_Grid(int x, int y);
void Play_Oblique();
//Hàm chính
int main(){
	resizeConsole(700, 600);
	WelcomeGame("Welcome.txt");
	system("cls");
	while (1){
		int choose = MenuGame("Welcome.txt", 10, 10);
		if (choose == 0){
			system("cls");
			ResumGame();
		}
		else if (choose == 1){
			system("cls");
			NewGame();
		}
		else if (choose == 2){
			system("cls");
			int ch = Menu_Open_ObliqueLine_or_Grid(15, 15);
			if (ch == 0){
				system("cls");
				Play_Oblique();
			}
			else if (ch == 1){
				temp_time = 0;
				ResetMatrix(Start);
				ShowCur(true);
				system("cls");
				gotoXY(20, 5);
				printf("Input patch: ");
				char s[30];
				gets(s);
				bool check = OpenGrid(Start, s);
				system("cls");
				if (check){
					OpenGame();
				}
				else{
					continue;
				}
			}
		}
		else if (choose == 3){
			LIST *l;
			system("cls");
			OpenHightScore("Player.txt");
			getch();
			system("cls");
		}
		else if (choose == 4){
			system("cls");
			HelpGame("Guide.txt");
			getch();
			system("cls");
		}
		else if (choose == 5){
			system("cls");
			AboutGame(5, 2, "About.txt");
			getch();
			system("cls");
		}
		else if (choose == 6){
			exit(0);
		}
	}
	getch();
	return 0;
}
int end_of_line_or_file(char c){
	return c == '\n' || c == EOF;
}
int readLine(char line[], int MaxLine, FILE *FileIn){
	char c;
	int index = 0;
	while (!end_of_line_or_file(c = fgetc(FileIn)) && index < MaxLine)
		line[index++] = c;
	line[index] = 0;
	if (index == MaxLine - 1){
		while ((c = fgetc(FileIn)) != '\n');
	}
	return index;
}
void WelcomeGame(char *fileName){
	FILE *FileIn = fopen(fileName, "rt");
	if (!FileIn){
		printf("File not found!!");
		getch();
		return;
	}
	char Line[100];
	int y = 5, x = 22;
	while (!feof(FileIn)){
		readLine(Line, 100, FileIn);
		TextColor(10);
		gotoXY(x, y++);
		printf("%s\n", Line);
	}
	int k = 0;
	int percent = 0;
	y += 5;
	x += 18;
	while (percent < 100){
		gotoXY(x, y);
		percent += 4;
		k++;
		printf("%d%c", percent, 37);
		for (int t = 0; t < k; ++t) {
			gotoXY(x - 12 + t, y + 2);
			TextColor(190);
			printf(" ");
			TextColor(15);
		}
		Sleep(150);
	}
	fclose(FileIn);
}
void HelpGame(char *fileName){
	FILE *FileIn = fopen(fileName, "rt");
	if (!FileIn){
		printf("Not found File!!");
		getch();
		return;
	}
	char Line[100];
	while (!feof(FileIn)){
		readLine(Line, 100, FileIn);
		printf("%s\n", Line);
	}
	fclose(FileIn);
}
void AboutGame(int x, int y, char *fileName){
	FILE *FileIn = fopen(fileName, "rt");
	if (!FileIn){
		printf("Not found File!!");
		getch();
		return;
	}
	char Line[100];
	while (!feof(FileIn)){
		readLine(Line, 100, FileIn);
		gotoXY(x, y++);
		printf("%s\n", Line);
	}
	fclose(FileIn);
}
int MenuGame(char *fileName, int x, int y){
	FILE *FileIn = fopen(fileName, "rt");
	if (!FileIn){
		printf("File not found!!");
		getch();
		return 0;
	}
	char Line[100];
	int b = 10, a = 30;
	while (!feof(FileIn)){
		readLine(Line, 100, FileIn);
		TextColor(10);
		gotoXY(a, b++);
		printf("%s\n", Line);
	}
	int Mau[Number];
	int choose;
	for (int i = 0; i < Number; ++i){
		Mau[i] = 9;
	}
	Mau[0] = 14; // Thao tác 1
	choose = 0; // Thao tác 1 được chọn
	int key;
	while (1){
		ShowCur(false);
		for (int i = 0; i < Number; ++i){
			gotoXY(x, y + i);
			TextColor(Mau[i]);
			printf("%s", Menu[i]);
			TextColor(15);
		}
		key = inputKey();
		switch (key){
		case key_Up:{
						if (choose == 0){
							choose = Number - 1;
							break;
						}
						else{
							choose--;
							break;
						}
		}
		case key_Down:{
						  if (choose == Number - 1){
							  choose = 0;
							  break;
						  }
						  else{
							  choose++;
							  break;
						  }
		}
		case key_Enter:{
						   return choose;
		}
		}
		//Reset màu sau khi có 1 lựa chọn
		for (int i = 0; i < Number; ++i){
			Mau[i] = 9;
		}
		Mau[choose] = 14;
	}
}
int MenuLevel(char *fileName, int x, int y){
	FILE *FileIn = fopen(fileName, "rt");
	if (!FileIn){
		printf("File not found!!");
		getch();
		return 0;
	}
	char Line[100];
	int b = 4, a = 10;
	while (!feof(FileIn)){
		readLine(Line, 100, FileIn);
		int tmp = rand() % 15 + 1;
		TextColor(tmp);
		gotoXY(a, b++);
		printf("%s\n", Line);
	}
	int Mau[Level];
	int choose = 0;
	//Khởi tạo màu và giá trị được chọn
	for (int i = 0; i < Level; ++i){
		Mau[i] = 14;
	}
	choose = 0; // Chức năng đầu tiên được chọn làm mặc định
	Mau[0] = 12;
	int key;
	while (1){
		//Hiển thị menu
		ShowCur(false);
		for (int i = 0; i < Level; ++i){
			gotoXY(x, y + i);
			TextColor(Mau[i]);
			printf("%s", Menu_Level[i]);
			TextColor(15);
		}
		key = inputKey();
		switch (key){
		case key_Up:{
						if (choose == 0){
							choose = Level - 1;
						}
						else{
							choose--;
						}
						break;
		}
		case key_Down:{
						  if (choose == Level - 1){
							  choose = 0;
						  }
						  else{
							  choose++;
						  }
						  break;
		}
		case key_Enter:{
						   return choose;
		}
		}
		//Reset lại màu và vị trí được chọn
		for (int i = 0; i < Level; ++i){
			Mau[i] = 14;
		}
		Mau[choose] = 12;
	}
}
bool checkNumber(int i, int j, int x){
	// kiểm tra trên dòng
	for (int t = 0; t < 9; ++t){
		if (Matrix[i][t] == x)
			return false;
	}
	// kiểm tra trên cột
	for (int t = 0; t < 9; ++t){
		if (Matrix[t][j] == x)
			return false;
	}
	// kiểm tra trong ô 3x3
	/*Bằng cách tìm chỉ số phần tử góc đầu tiên*/
	int temp_i = i % 3;
	int temp_j = j % 3;
	for (int t = i - temp_i; t <= i - temp_i + 2; ++t){
		for (int k = j - temp_j; k <= j - temp_j + 2; ++k){
			if (Matrix[t][k] == x)
				return false;
		}
	}
	return true;
}
void RandomVal(){
	srand(time(0));
	for (int i = 0; i < 9; ++i){
		int x = rand() % 9 + 1;
		for (int j = i; j >= 0; --j){
			if (x != Matrix[j][j]){
				Matrix[i][i] = x;
			}
			else{
				--i;
				break;
			}
		}
	}
}
void InitMatrix(int num){
	srand(time(0));
	int i, j;
	int index = 0;

	while (index < num){
		i = rand() % 9;
		j = rand() % 9;
		while (Start[i][j] == 0){
			i = rand() % 9;
			j = rand() % 9;
		}
		Temp[i][j] = 0;
		int k = Solve_for_Init(0);
		count = 0;
		if (k == 1){
			Start[i][j] = 0;
			CopyMatrix();
			index++;
		}
		else{
			CopyMatrix();
		}
	}
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			if (Start[i][j] != 0){
				Color[i][j] = 12;
			}
			else{
				Color[i][j] = 15;
			}
		}
	}
}
int findLast(){
	// Tìm ô cuối cùng có giá trị là 0, tức là không phải gợi ý
	for (int i = 8; i >= 0; --i){
		for (int j = 8; j >= 0; --j){
			if (Matrix[i][j] == 0){
				return i * 9 + j;
			}
		}
	}
}
//-----for init----------
int findLast_for_Init(){
	// Tìm ô cuối cùng có giá trị là 0, tức là không phải gợi ý
	for (int i = 8; i >= 0; --i){
		for (int j = 8; j >= 0; --j){
			if (Temp[i][j] == 0){
				return i * 9 + j;
			}
		}
	}
}
bool checkNumber_for_Init(int i, int j, int x){
	// kiểm tra trên dòng
	for (int t = 0; t < 9; ++t){
		if (Temp[i][t] == x)
			return false;
	}
	// kiểm tra trên cột
	for (int t = 0; t < 9; ++t){
		if (Temp[t][j] == x)
			return false;
	}
	// kiểm tra trong ô 3x3
	/*Bằng cách tìm chỉ số phần tử góc đầu tiên*/
	int temp_i = i % 3;
	int temp_j = j % 3;
	for (int t = i - temp_i; t <= i - temp_i + 2; ++t){
		for (int k = j - temp_j; k <= j - temp_j + 2; ++k){
			if (Temp[t][k] == x)
				return false;
		}
	}
	return true;
}
int Solve_for_Init(int k){
	int last = findLast_for_Init();
	int i, j;
	//Các số đề bài cho thì bỏ qua hết
	while (Temp[k / 9][k % 9] != 0){
		k++;
	}
	//Hiện tại k nằm ngay số 0 đầu tiên
	i = k / 9;
	j = k % 9;
	for (int t = 1; t <= 9; ++t){
		//Các số có thể điền vào ô
		if (checkNumber_for_Init(i, j, t)){
			Temp[i][j] = t;
			if (k == last){
				SaveResult_for_Resum();
				count++;
				return count;
			}
			else{
				Solve_for_Init(k + 1);
			}
			Temp[i][j] = 0;
		}
	}
	return count;
}
//-----------------
bool SolutionSudoku(int k){
	int last = findLast();
	int i, j;
	//Các số đề bài cho thì bỏ qua hết
	while (Matrix[k / 9][k % 9] != 0){
		k++;
	}
	//Hiện tại k nằm ngay số 0 đầu tiên
	i = k / 9;
	j = k % 9;
	for (int t = 1; t <= 9; ++t){
		//Các số có thể điền vào ô
		if (checkNumber(i, j, t)){
			Matrix[i][j] = t;
			if (k == last){
				SaveResult();
				return true;
			}
			else{
				SolutionSudoku(k + 1);
			}
			Matrix[i][j] = 0;
		}
	}
	return false;
}
bool isNumber(char c){
	for (int i = 1; i <= 9; ++i){
		char ch = i + 48;
		if (c == ch){
			return true;
		}
	}
	return false;
}
void ResetMatrix(int a[][9]){
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			a[i][j] = 0;
		}
	}
}
void SaveResult_for_Resum(){
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			Result[i][j] = Temp[i][j];
		}
	}
}
void ResumGame(){
	if (!openSaveGame(Start, "loggame.txt")){
		system("cls");
		return;
	}
	CopyMatrix();
	ResetMatrix(Result);
	Solve_for_Init(0);
	int x = 7, y = 6; // tọa độ ô thứ nhất
	int key;
	gotoXY(15, 2);
	printf("Let's start. Press Any Key!!");
	getch();
	system("cls");
	gotoXY(5, 2);
	TextColor(9);
	printf("Difficulty: %s", Level_Dif);
	TextColor(15);
	gotoXY(50, 5);
	printf("Move: UP, DOWN, LEFT, RIGHT");
	gotoXY(50, 6);
	printf("Save Game: S or s");
	gotoXY(50, 7);
	printf("Show Result: F or f");
	gotoXY(50, 8);
	printf("Hint: W or w");
	gotoXY(50, 9);
	printf("Check: C or c");
	gotoXY(50, 10);
	printf("Back: Backspace");
	gotoXY(50, 11);
	printf("Delete: Delete");
	gotoXY(50, 12);
	printf("Time:    (s)");
	FILE *FileIn = fopen("Theme.txt", "rt");
	if (!FileIn){
		printf("File not found!!");
		getch();
		return;
	}
	char Line[100];
	int b = 15, a = 50;
	while (!feof(FileIn)){
		readLine(Line, 100, FileIn);
		int tmp = rand() % 15 + 1;
		TextColor(tmp);
		gotoXY(a, b++);
		printf("%s\n", Line);
	}
	DrawGrid(Start, 5, 5);
	ShowCur(false);
	clock_t s, f;
	while (1){
		s = clock();
		key = inputKey();
		if (key == key_Left){
			if (x > 7){
				x -= 4;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (key == key_Right){
			if (x < 39){
				x += 4;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (key == key_Up){
			if (y > 6){
				y -= 2;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (key == key_Down){
			if (y < 21){
				y += 2;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (isNumber(key)){
			if (Color[(y - 6) / 2][(x - 7) / 4] != 12 && Color[(y - 6) / 2][(x - 7) / 4] != 10){
				Start[(y - 6) / 2][(x - 7) / 4] = key - 48;
				putchar(key);
				gotoXY(x, y);
			}
		}
		else if (key == 'w' || key == 'W'){
			Hint();
		}
		else if (key == key_Back){
			system("cls");
			return;
		}
		else if (key == key_Del){
			if (Color[(y - 6) / 2][(x - 7) / 4] != 12 && Color[(y - 6) / 2][(x - 7) / 4] != 10){
				Start[(y - 6) / 2][(x - 7) / 4] = 0;
				putchar(' ');
			}
		}
		else if (key == 'f' || key == 'F'){
			DrawGrid(Result, 5, 5);
			gotoXY(x, y);
		}
		else if (key == 's' || key == 'S'){
			SaveGame(Start, "loggame.txt");
		}
		else if (key == 'c' || key == 'C'){
			if (isFillFull()){
				bool check = Win_or_Lost();
				if (check){
					system("cls");
					gotoXY(20, 2);
					printf("YOU-WIN");
					gotoXY(30, 5);
					char Name[20];
					printf("Your Name: ");
					gets(Name);
					gotoXY(30, 6);
					printf("Time: %d  (s)", temp_time / 1000);
					FILE *FileOut = fopen("Player.txt", "a");
					fprintf(FileOut, "%s\n", Name);
					fprintf(FileOut, "%d\n", temp_time / 1000);
					fclose(FileOut);
					getch();
					system("cls");
					break;
				}
			}
			else{
				gotoXY(20, 25);
				printf("Some cells are empty. Please continue!");
				gotoXY(x, y);
			}
		}
		Sleep(50);
		f = clock();
		gotoXY(56, 12);
		printf("%d", (f - s + temp_time) / 1000);
		temp_time = f - s + temp_time;
		gotoXY(x, y);
	}
}
bool isFillFull(){
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			if (Start[i][j] == 0){
				return false;
			}
		}
	}
	return true;
}
bool Win_or_Lost(){
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			if (Start[i][j] != Result[i][j]){
				TextColor(13);
				gotoXY(4 * j + 7, 2 * i + 6);
				printf("%d", Start[i][j]);
				gotoXY(4 * j + 7, 2 * i + 6);
				TextColor(15);
				return false;
			}
		}
	}
	return true;
}
void NewGame(){
	ResetMatrix(Matrix);
	ResetMatrix(Start);
	ResetMatrix(Temp);
	ResetMatrix(Color);
	
	RandomVal(); // Random số trong đường chéo chính
	SolutionSudoku(0);
	int level = MenuLevel("Play.txt", 35, 25);
	system("cls");

	if (level == 0){ // Mức dễ
		InitMatrix(49); // xóa 30 ô
		strcpy(Level_Dif, "Easy");
	}
	else if (level == 1){
		InitMatrix(51);
		strcpy(Level_Dif, "Medium");
	}
	else if (level == 2){
		InitMatrix(54);
		strcpy(Level_Dif, "Hard");
	}
	else if (level == 3){
		InitMatrix(55);
		strcpy(Level_Dif, "Very Hard");
	}
	else if (level == 4){
		ResetMatrix(Matrix);
		ResetMatrix(Start);
		ResetMatrix(Temp);
		ResetMatrix(Color);
		bool check = RandomVal_Oblique_Line();
		int k = 0;
		int u = 5, v = 5;
		while (!check){
			k++;
			ResetMatrix(Matrix);
			ResetMatrix(Color);
			ResetMatrix(Signed);
			clrscr();
			TextColor(190);
			gotoXY(u++, v);
			printf(" ");
			TextColor(15);
			check = RandomVal_Oblique_Line();	
			Sleep(100);
		}
		system("cls");
		SolutionSudoku(0);
		InitMatrix_Oblique(45);
		strcpy(Level_Dif, "Oblique Line");
	}
	int x = 7, y = 6; // tọa độ ô thứ nhất
	int key;
	gotoXY(15, 2);
	printf("Let's start. Press Any Key!!");
	getch();
	system("cls");
	gotoXY(5, 2);
	TextColor(9);
	printf("Difficulty: %s", Level_Dif);
	TextColor(15);
	gotoXY(50, 5);
	printf("Move: UP, DOWN, LEFT, RIGHT");
	gotoXY(50, 6);
	printf("Save Game: S or s");
	gotoXY(50, 7);
	printf("Show Result: F or f");
	gotoXY(50, 8);
	printf("Hint: W or w");
	gotoXY(50, 9);
	printf("Check: C or c");
	gotoXY(50, 10);
	printf("Back: Backspace");
	gotoXY(50, 11);
	printf("Delete: Delete");
	gotoXY(50, 12);
	printf("Time:    (s)");
	FILE *FileIn = fopen("Theme.txt", "rt");
	if (!FileIn){
		printf("File not found!!");
		getch();
		return;
	}
	char Line[100];
	int b = 15, a = 50;
	while (!feof(FileIn)){
		readLine(Line, 100, FileIn);
		int tmp = rand() % 15 + 1;
		TextColor(tmp);
		gotoXY(a, b++);
		printf("%s\n", Line);
	}
	DrawGrid(Start, 5, 5);
	ShowCur(false);
	clock_t s, f;
	temp_time = 0;
	while (1){
		s = clock();
		key = inputKey();
		if (key == key_Left){
			if (x > 7){
				x -= 4;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (key == key_Right){
			if (x < 39){
				x += 4;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (key == key_Up){
			if (y > 6){
				y -= 2;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (key == key_Down){
			if (y < 21){
				y += 2;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (isNumber(key)){
			if (Color[(y - 6) / 2][(x - 7) / 4] != 12 && Color[(y - 6) / 2][(x - 7) / 4] != 10){
				Start[(y - 6) / 2][(x - 7) / 4] = key - 48;
				putchar(key);
				gotoXY(x, y);
			}
		}
		else if (key == 'w' || key == 'W'){
			Hint();
		}
		else if (key == key_Back){
			system("cls");
			return;
		}
		else if (key == key_Del){
			if (Color[(y - 6) / 2][(x - 7) / 4] != 12 && Color[(y - 6) / 2][(x - 7) / 4] != 10){
				Start[(y - 6) / 2][(x - 7) / 4] = 0;
				putchar(' ');
			}
		}
		else if (key == 'f' || key == 'F'){
			DrawGrid(Result, 5, 5);
			gotoXY(x, y);
		}
		else if (key == 's' || key == 'S'){
			SaveGame(Start, "loggame.txt");
		}
		else if (key == 'c' || key == 'C'){
			if (isFillFull()){
				bool check = Win_or_Lost();
				if (check){
					system("cls");
					gotoXY(20, 2);
					printf("YOU-WIN");
					gotoXY(30, 5);
					char Name[20];
					printf("Your Name: ");
					gets(Name);
					gotoXY(30, 6);
					printf("Time: %d  (s)", temp_time / 1000);
					FILE *FileOut = fopen("Player.txt", "a");
					fprintf(FileOut, "%s\n", Name);
					fprintf(FileOut, "%d\n", temp_time / 1000);
					fclose(FileOut);
					getch();
					system("cls");
					break;
				}
			}
			else{
				gotoXY(20, 25);
				TextColor(169);
				printf("Some cells are empty. Please continue!");
				TextColor(15);
				gotoXY(x, y);
			}
		}
		Sleep(50);
		f = clock();
		gotoXY(56, 12);
		printf("%d", (f - s + temp_time) / 1000);
		temp_time = f - s + temp_time;
		gotoXY(x, y);
	}
}

void DrawGrid(int a[][9], int x, int y){
	int m = x, n = y;
	gotoXY(x, y); // đưa tới vị trị cần vẽ
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 10; j++){
			if (i == 0){
				if (j == 0){
					putchar(213); // vẽ ký tự ở góc
					for (int k = 0; k < 3; ++k)
						putchar(205);
				}
				else if (j == 9){
					putchar(184);
				}
				else if (j % 3 != 0){
					putchar(209);
					for (int k = 0; k < 3; ++k)
						putchar(205);
				}
				else if (j % 3 == 0){
					putchar(203);
					for (int k = 0; k < 3; ++k)
						putchar(205);
				}
			}
			else{
				if (j == 0){
					if (i % 3 == 0 && i != 0 && i != 9){
						putchar(198);
					}
					else {
						printf("%c", 195);
					}
				}
				else{
					for (int x = 0; x < 3; ++x){
						if (i % 3 != 0)
							printf("%c", 196);
						else{
							printf("%c", 205);
						}
					}
					if (j != 9){
						if (i % 3 != 0){
							if (j % 3 != 0){
								putchar(197);
							}
							if (j % 3 == 0){
								putchar(215);
							}
						}
						else{
							if (j % 3 != 0){
								putchar(216);
							}
							else{
								putchar(206);
							}
						}
					}
					else{
						if (i % 3 != 0){
							putchar(180);
						}
						else{
							putchar(181);
						}
					}
				}
			}
		}
		y = y + 1;
		gotoXY(x, y);
		int k = x;
		for (int t = 0; t < 10; ++t){

			if (t == 0 || t == 9){
				printf("%c", 179);
				k += 4;
				gotoXY(k, y);
			}
			else if (t % 3 != 0){
				printf("%c", 179);
				k += 4;
				gotoXY(k, y);
			}
			else{
				putchar(186);
				k += 4;
				gotoXY(k, y);
			}
		}
		if (i == 8){
			gotoXY(x, y + 1);
			int t;
			for (t = 0; t < 10; ++t){
				if (t == 0){
					putchar(212);
					for (int k = 0; k < 3; ++k){
						putchar(205);
					}
				}
				else if (t == 9){
					putchar(190);
				}
				else if (t % 3 != 0){
					putchar(207);
					for (int k = 0; k < 3; ++k){
						putchar(205);
					}
				}
				else if (t % 3 == 0){
					putchar(202);
					for (int k = 0; k < 3; ++k){
						putchar(205);
					}
				}
			}

		}
		y = y + 1;
		gotoXY(x, y);
	}
	//Hết hàm vẽ lưới
	m += 2; // hoành độ
	n += 1; // tung độ
	int h;
	gotoXY(m, n); // Đưa con trỏ về vị trí ban đầu ô đầu tiên
	for (int i = 0; i < 9; ++i){
		h = m;
		for (int j = 0; j < 9; ++j){
			gotoXY(h, n);
			if (a[i][j] != 0){
				TextColor(Color[i][j]);
				printf("%d", a[i][j]);
				TextColor(15);
			}
			h = h + 4;
		}
		n = n + 2;
	}
}
void SaveResult(){
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			Result[i][j] = Matrix[i][j];
			Start[i][j] = Matrix[i][j];
			Temp[i][j] = Matrix[i][j];
		}
	}
}
void CopyMatrix(){
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			Temp[i][j] = Start[i][j];
		}
	}
}
void Hint(){
	int x = whereX();
	int y = whereY();
	if (Color[(y - 6) / 2][(x - 7) / 4] != 12 && Color[(y - 6) / 2][(x - 7) / 4] != 9){
		Start[(y - 6) / 2][(x - 7) / 4] = Result[(y - 6) / 2][(x - 7) / 4];
		Color[(y - 6) / 2][(x - 7) / 4] = 10;
		TextColor(10);
		printf("%d", Start[(y - 6) / 2][(x - 7) / 4]);
		TextColor(15);
		gotoXY(x, y);
	}
}

void SaveGame(int a[][9], char *fileName){
	FILE *FileOut = fopen(fileName, "wt");
	//Ghi giá trị khởi tạo
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			fprintf(FileOut, "%d ", a[i][j]);
		}
		fprintf(FileOut, "\n");
	}
	//Ghi giá trị màu
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			fprintf(FileOut, "%d ", Color[i][j]);
		}
		fprintf(FileOut, "\n");
	}
	//Ghi giá trị thời gian chơi
	fprintf(FileOut, "%d\n", temp_time);
	fprintf(FileOut, "%s", Level_Dif);
	fclose(FileOut);
}
//Hàm mở bảng Sudoku lần chơi trước
bool openSaveGame(int a[][9], char *fileName){
	FILE *FileIn = fopen(fileName, "rt");
	if (!FileIn){
		printf("File not found!!");
		getch();
		return false;
	}
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			fscanf(FileIn, "%d", &a[i][j]);
		}
	}
	//Đọc màu
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			fscanf(FileIn, "%d", &Color[i][j]);
		}
	}
	fscanf(FileIn, "%d", &temp_time);
	fgetc(FileIn);
	fgets(Level_Dif, 15, FileIn);
	fclose(FileIn);
	return true;
}

//Các hàm cho đường xuyên

bool Check_Cell_Next(int i, int j){
	int dem = 0;
	int m, n;
	m = i + 1;
	n = j + 1;
	if (Matrix[m][n] == 0){
		dem++;
	}
	m = i + 1;
	n = j - 1;
	if (Matrix[m][n] == 0){
		dem++;
	}
	m = i - 1;
	n = j - 1;
	if (Matrix[m][n] == 0){
		dem++;
	}
	m = i - 1;
	n = j + 1;
	if (Matrix[m][n] == 0){
		dem++;
	}
	if (dem >= 2)
		return true;
	return false;
}

void Cell_Next(int &i, int &j){
	int a = rand() % 20;
	int b = rand() % 20;
	if (a % 2 == 0){
		if (i < 8){
			i++;
		}
		else{
			i--;
		}
	}
	else{
		if (i > 0){
			i--;
		}
		else{
			i++;
		}
	}
	if (b % 2 == 0){
		if (j < 8){
			j++;
		}
		else{
			j--;
		}
	}
	else{
		if (j > 0){
			j--;
		}
		else{
			j++;
		}
	}
}

bool RandomVal_Oblique_Line(){
	srand(time(0));
	int index = 0, i, j;
	int A[9]; // Mang đánh dấu
	for (int i = 1; i <= 9; ++i){
		A[i] = 0; //Ban đầu chưa có số nào điền
	}
	//Random 1 ô bất kì
	i = rand() % 9;
	j = rand() % 9;
	int x;
	int t1, t2;
	while (index < 9){
		t1 = i;
		t2 = j;
		x = rand() % 9 + 1;
		while (A[x]){
			x = rand() % 9 + 1;
		}
		A[x] = 1;
		Matrix[i][j] = x;
		Signed[i][j] = 1;
		Color[i][j] = 9;
		//Ô tiếp theo
		Cell_Next(i, j);
		int idx = 0;
		while (!Check_Cell_Next(i, j) || Matrix[i][j] != 0){
			idx++;
			i = t1;
			j = t2;
			Cell_Next(i, j);
			if (idx > 3){
				return false;
			}
		}
		index++;
	}
	return true;
}
void InitMatrix_Oblique(int num){
	srand(time(0));
	int i, j;
	int index = 0;

	while (index < num){
		i = rand() % 9;
		j = rand() % 9;
		while (Start[i][j] == 0 || Signed[i][j] == 1){
			i = rand() % 9;
			j = rand() % 9;
		}
		Temp[i][j] = 0;
		int k = Solve_for_Init(0);
		count = 0;
		if (k == 1){
			Start[i][j] = 0;
			CopyMatrix();
			index++;
		}
		else{
			CopyMatrix();
		}
	}
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			if (Start[i][j] != 0){
				if (Color[i][j] == 0)
					Color[i][j] = 12;
			}
			else{
				if (Color[i][j] == 0)
					Color[i][j] = 15;
			}
		}
	}
}
void OpenHightScore(char *fileName){
	FILE *FileIn = fopen(fileName, "rt");
	if (!FileIn){
		printf("List is empty !!");
		return;
	}
	int index = 0;
	while (index != 6 && !feof(FileIn)){
		readLine(Player[index].Name, 20, FileIn);
		fscanf(FileIn, "%d", &Player[index].Time);
		fgetc(FileIn);
		index++;

	}
	for (int i = 0; i < index - 1; ++i){
		for (int j = i + 1; j < index; ++j){
			if (Player[i].Time > Player[j].Time){
				LIST temp = Player[i];
				Player[i] = Player[j];
				Player[j] = temp;
			}
		}
	}
	int x = 20, y = 5;
	gotoXY(x, y);
	printf("Player    : Time (s)\n");
	for (int i = 1; i < index ; ++i){
		gotoXY(20, y + i + 1);
		TextColor(6 + i);
		printf("%d) %s", i, Player[i].Name);
		gotoXY(x + 10, y + i + 1);
		printf(":  %d", Player[i].Time);
		TextColor(15);
	}
	printf("\n\n\nPress any key to continue...");
	fclose(FileIn);
}
void OpenGame(){
	CopyMatrix();
	ResetMatrix(Result);
	Solve_for_Init(0);
	int x = 7, y = 6; // tọa độ ô thứ nhất
	int key;
	gotoXY(15, 2);
	printf("Let's start. Press Any Key!!");
	getch();
	system("cls");
	gotoXY(5, 2);
	TextColor(9);
	printf("Open from file.");
	TextColor(15);
	gotoXY(50, 5);
	printf("Move: UP, DOWN, LEFT, RIGHT");
	gotoXY(50, 6);
	printf("Save Game: S or s");
	gotoXY(50, 7);
	printf("Computer Play: F or f");
	gotoXY(50, 8);
	printf("Hint: W or w");
	gotoXY(50, 9);
	printf("Check: C or c");
	gotoXY(50, 10);
	printf("Back: Backspace");
	gotoXY(50, 11);
	printf("Delete: Delete");
	gotoXY(50, 12);
	printf("Time:    (s)");
	FILE *FileIn = fopen("Theme.txt", "rt");
	if (!FileIn){
		printf("File not found!!");
		getch();
		return;
	}
	char Line[100];
	int b = 15, a = 50;
	while (!feof(FileIn)){
		readLine(Line, 100, FileIn);
		int tmp = rand() % 15 + 1;
		TextColor(tmp);
		gotoXY(a, b++);
		printf("%s\n", Line);
	}
	DrawGrid(Start, 5, 5);
	ShowCur(false);
	clock_t s, f;
	while (1){
		s = clock();
		key = inputKey();
		if (key == key_Left){
			if (x > 7){
				x -= 4;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (key == key_Right){
			if (x < 39){
				x += 4;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (key == key_Up){
			if (y > 6){
				y -= 2;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (key == key_Down){
			if (y < 21){
				y += 2;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (isNumber(key)){
			if (Color[(y - 6) / 2][(x - 7) / 4] != 12 && Color[(y - 6) / 2][(x - 7) / 4] != 10){
				Start[(y - 6) / 2][(x - 7) / 4] = key - 48;
				putchar(key);
				gotoXY(x, y);
			}
		}
		else if (key == 'w' || key == 'W'){
			Hint();
		}
		else if (key == key_Back){
			system("cls");
			return;
		}
		else if (key == key_Del){
			if (Color[(y - 6) / 2][(x - 7) / 4] != 12 && Color[(y - 6) / 2][(x - 7) / 4] != 10){
				Start[(y - 6) / 2][(x - 7) / 4] = 0;
				putchar(' ');
			}
		}
		else if (key == 'f' || key == 'F'){
			DrawGrid(Result, 5, 5);
			gotoXY(x, y);
		}
		else if (key == 's' || key == 'S'){
			SaveGame(Start, "loggame.txt");
		}
		else if (key == 'c' || key == 'C'){
			if (isFillFull()){
				bool check = Win_or_Lost();
				if (check){
					system("cls");
					gotoXY(20, 2);
					printf("YOU-WIN");
					gotoXY(30, 5);
					char Name[20];
					printf("Your Name: ");
					gets(Name);
					gotoXY(30, 6);
					printf("Time: %d  (s)", temp_time / 1000);
					FILE *FileOut = fopen("Player.txt", "a");
					fprintf(FileOut, "%s\n", Name);
					fprintf(FileOut, "%d\n", temp_time / 1000);
					fclose(FileOut);
					getch();
					system("cls");
					break;
				}
			}
			else{
				gotoXY(20, 25);
				printf("Some cells are empty. Please continue!");
				gotoXY(x, y);
			}
		}
		Sleep(50);
		f = clock();
		gotoXY(56, 12);
		printf("%d", (f - s + temp_time) / 1000);
		temp_time = f - s + temp_time;
		gotoXY(x, y);
	}
}
bool OpenGrid(int a[][9], char *patch){
	FILE *FileIn = fopen(patch, "rt");
	if (!FileIn){
		gotoXY(20, 7);
		printf("File not found!!");
		getch();
		return false;
	}
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			fscanf(FileIn, "%d", &a[i][j]);
			if (a[i][j] != 0){
				Color[i][j] = 12;
			}
			else{
				Color[i][j] = 15;
			}
		}
	}
	fclose(FileIn);
	return true;
}

//Hàm dành cho chơi ở chế độ mở đường xuyên

bool Open_Oblique(char *patch){
	FILE *FileIn = fopen(patch, "rt");
	if (!FileIn){
		gotoXY(20, 7);
		printf("File not found!!");
		getch();
		return false;
	}
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			fscanf(FileIn, "%d", &Matrix[i][j]);
			if (Matrix[i][j] != 0){
				Signed[i][j] = 1;
				Color[i][j] = 9;
			}
			else{
				Signed[i][j] = 0;
				Color[i][j] = 0;
			}
		}
	}
	fclose(FileIn);
	return true;
}
void Random_Oblique(){
	int M[9]; //Mảng đánh dấu các số đã điền
	for (int i = 1; i <= 9; ++i){
		M[i] = 0;
	}
	int index = 0;
	srand(time(0));
	int x;
	while (index != 9){
		x = rand() % 9 + 1;
		while (M[x]){
			x = rand() % 9 + 1;
		}
		M[x] = 1;
		int dem = 0;
		for (int i = 0; i < 9; ++i){
			for (int j = 0; j < 9; ++j){
				if (Matrix[i][j] == -1){
					dem++;
					index++;
					Matrix[i][j] = x;
					break;
				}
			}
			if (dem == 1){
				break;
			}
		}
	}
}
int Menu_Open_ObliqueLine_or_Grid(int x, int y){
	FILE *FileIn = fopen("OpenTheme.txt", "rt");
	if (!FileIn){
		printf("File not found!!");
		getch();
		return 0;
	}
	char Line[100];
	int b = 10, a = 30;
	while (!feof(FileIn)){
		readLine(Line, 100, FileIn);
		TextColor(10);
		gotoXY(a, b++);
		printf("%s\n", Line);
	}
	int Mau[Open];
	int choose;
	for (int i = 0; i < Open; ++i){
		Mau[i] = 9;
	}
	Mau[0] = 14; // Thao tác 1
	choose = 0; // Thao tác 1 được chọn
	int key;
	while (1){
		ShowCur(false);
		for (int i = 0; i < Open; ++i){
			gotoXY(x, y + i);
			TextColor(Mau[i]);
			printf("%s", Menu_Open[i]);
			TextColor(15);
		}
		key = inputKey();
		switch (key){
		case key_Up:{
						if (choose == 0){
							choose = Open - 1;
							break;
						}
						else{
							choose--;
							break;
						}
		}
		case key_Down:{
						  if (choose == Open - 1){
							  choose = 0;
							  break;
						  }
						  else{
							  choose++;
							  break;
						  }
		}
		case key_Enter:{
						   return choose;
		}
		}
		//Reset màu sau khi có 1 lựa chọn
		for (int i = 0; i < Open; ++i){
			Mau[i] = 9;
		}
		Mau[choose] = 14;
	}
}
void Play_Oblique(){
	ShowCur(true);
	ResetMatrix(Matrix);
	ResetMatrix(Start);
	ResetMatrix(Temp);
	ResetMatrix(Color);
	ResetMatrix(Signed);
	gotoXY(20, 5);
	printf("Input patch: ");
	char S[30];
	gets(S);
	system("cls");
	bool check = Open_Oblique(S);
	if (!check){
		system("cls");
		return;
	}
	Random_Oblique();
	SolutionSudoku(0);
	InitMatrix_Oblique(55);
	int x = 7, y = 6; // tọa độ ô thứ nhất
	int key;
	gotoXY(15, 2);
	printf("Let's start. Press Any Key!!");
	getch();
	system("cls");
	gotoXY(5, 2);
	TextColor(9);
	printf("Open Oblique from file !!");
	TextColor(15);
	gotoXY(50, 5);
	printf("Move: UP, DOWN, LEFT, RIGHT");
	gotoXY(50, 6);
	printf("Save Game: S or s");
	gotoXY(50, 7);
	printf("Show Result: F or f");
	gotoXY(50, 8);
	printf("Hint: W or w");
	gotoXY(50, 9);
	printf("Check: C or c");
	gotoXY(50, 10);
	printf("Back: Backspace");
	gotoXY(50, 11);
	printf("Delete: Delete");
	gotoXY(50, 12);
	printf("Time:    (s)");
	FILE *FileIn = fopen("Theme.txt", "rt");
	if (!FileIn){
		printf("File not found!!");
		getch();
		return;
	}
	char Line[100];
	int b = 15, a = 50;
	while (!feof(FileIn)){
		readLine(Line, 100, FileIn);
		int tmp = rand() % 15 + 1;
		TextColor(tmp);
		gotoXY(a, b++);
		printf("%s\n", Line);
	}
	DrawGrid(Start, 5, 5);
	ShowCur(false);
	clock_t s, f;
	temp_time = 0;
	while (1){
		s = clock();
		key = inputKey();
		if (key == key_Left){
			if (x > 7){
				x -= 4;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (key == key_Right){
			if (x < 39){
				x += 4;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (key == key_Up){
			if (y > 6){
				y -= 2;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (key == key_Down){
			if (y < 21){
				y += 2;
				gotoXY(x, y);
				ShowCur(true);
			}
		}
		else if (isNumber(key)){
			if (Color[(y - 6) / 2][(x - 7) / 4] != 12 && Color[(y - 6) / 2][(x - 7) / 4] != 10){
				Start[(y - 6) / 2][(x - 7) / 4] = key - 48;
				putchar(key);
				gotoXY(x, y);
			}
		}
		else if (key == 'w' || key == 'W'){
			Hint();
		}
		else if (key == key_Back){
			system("cls");
			return;
		}
		else if (key == key_Del){
			if (Color[(y - 6) / 2][(x - 7) / 4] != 12 && Color[(y - 6) / 2][(x - 7) / 4] != 10){
				Start[(y - 6) / 2][(x - 7) / 4] = 0;
				putchar(' ');
			}
		}
		else if (key == 'f' || key == 'F'){
			DrawGrid(Result, 5, 5);
			gotoXY(x, y);
		}
		else if (key == 's' || key == 'S'){
			SaveGame(Start, "loggame.txt");
		}
		else if (key == 'c' || key == 'C'){
			if (isFillFull()){
				bool check = Win_or_Lost();
				if (check){
					system("cls");
					gotoXY(20, 2);
					printf("YOU-WIN");
					gotoXY(30, 5);
					char Name[20];
					printf("Your Name: ");
					gets(Name);
					gotoXY(30, 6);
					printf("Time: %d  (s)", temp_time / 1000);
					FILE *FileOut = fopen("Player.txt", "a");
					fprintf(FileOut, "%s\n", Name);
					fprintf(FileOut, "%d\n", temp_time / 1000);
					fclose(FileOut);
					getch();
					system("cls");
					break;
				}
			}
			else{
				gotoXY(20, 25);
				TextColor(169);
				printf("Some cells are empty. Please continue!");
				TextColor(15);
				gotoXY(x, y);
			}
		}
		Sleep(50);
		f = clock();
		gotoXY(56, 12);
		printf("%d", (f - s + temp_time) / 1000);
		temp_time = f - s + temp_time;
		gotoXY(x, y);
	}
}