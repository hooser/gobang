#include"header_renju.h"
#include<stack>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <cmath>
#include "search.h"
using namespace std;

/* 保存走法的栈 */
stack<PIECEMOVE> stackMove;

extern int opening();
extern ans consider(char player, int(*currentThisBoard)[GRID_NUM]);

/* 棋盘示意图 */
int renjuBoard[GRID_NUM][GRID_NUM] =
{
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
};

/* 本机棋盘 */
int currentThisBoard[GRID_NUM][GRID_NUM] = {
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
	{ NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE,NO_PIECE },
};
int currentThisBoardBackUp[GRID_NUM][GRID_NUM] = { NO_PIECE };
/* 对方棋盘 */
char currentThatBoard[GRID_NUM][GRID_NUM];

/* 棋盘输出函数 */
void Display_0()
{
	for (int i = 0; i < GRID_NUM; i++)
		for (int j = 0; j < GRID_NUM; j++)
		{
			if (currentThisBoard[i][j] == BLACK)
			{
				currentThatBoard[i][j] = 'X';
			}
			else if (currentThisBoard[i][j] == WHITE)
			{
				currentThatBoard[i][j] = 'O';
			}
			else
			{
				currentThatBoard[i][j] = ' ';
			}
		}
}

/* 棋盘输出函数 */
void Display_1()
{
	for (int i = 0; i < GRID_NUM; i++)
		for (int j = 0; j < GRID_NUM; j++)
		{
			if (currentThisBoard[i][j] == BLACK)
			{
				currentThatBoard[i][j] = '0';
			}
			else if (currentThisBoard[i][j] == WHITE)
			{
				currentThatBoard[i][j] = 'X';
			}
			else
			{
				currentThatBoard[i][j] = ' ';
			}
		}
}

void backUp() {
	for (int i = 0; i < GRID_NUM; i++) {
		for (int j = 0; j < GRID_NUM; j++) {
			currentThisBoardBackUp[i][j] = currentThisBoard[i][j];
		}
	}
}

void backUpReverse() {
	for (int i = 0; i < GRID_NUM; i++) {
		for (int j = 0; j < GRID_NUM; j++) {
			currentThisBoard[i][j] = currentThisBoardBackUp[i][j];
		}
	}
}

/* 棋盘输出函数 */
void Display_Board(char a[][GRID_NUM])
{
	/* 列标 */
	cout << "   ";
	for (int i = 0; i < GRID_NUM; i++)
	{
		printf("%3d", i);
	}
	cout << endl;

	/* 行标 */
	for (int i = 0; i < GRID_NUM; i++)
	{
		printf("%3d", i);
		for (int j = 0; j < GRID_NUM; j++)
		{
			printf("  %c", a[i][j]);
		}
		cout << endl;
	}
}

int main()
{
	/* 设置计时时钟 */
	clock_t startPoint, endPoint;
	startPoint = clock();

	/* 对方落子坐标 */
	int x_input, y_input;
	/* 初始搜索深度 */
	int depth = INIT_DEPTH;
	/* 初始化各个类 */
	negaScouter nega_scouter;
	historyHeuristic history_heuristic;
	evaluation evaluation_;
	moveGenerator move_generator;
	ans ans_;
	ans_.x = -1;
	ans_.y = -1;

	nega_scouter.setSearchDepth(depth);	//设置搜索深度
	nega_scouter.setHistoryHeuristic(&history_heuristic);	//设置历史启发器
	nega_scouter.setEvaluation(&evaluation_);	//设置评估函数
	nega_scouter.setMoveGenerator(&move_generator);	//设置走法器
	memset(currentThisBoard, NO_PIECE, sizeof(currentThisBoard));//初始化棋盘
	char ch;
	cout << "请选择先后手：0->本机先手，1->本机后手:";
	while (1) {
		ch = _getch();
		if (ch == '0' || ch == '1')
			break;
	}
	cout << endl;
	system("cls");

	int t = 0;
	int tss = 0;
	int x_best = 0, y_best = 0;
	if (ch == '1') {
		Display_0();
		Display_Board(currentThatBoard);
		while (!t)
		{
			cout << "请输入落子坐标:";
			while (1) {
				cin >> x_input >> y_input;
				if (x_input >= 0 && x_input < GRID_NUM && y_input >= 0 && y_input < GRID_NUM&&currentThisBoard[x_input][y_input] == NO_PIECE)
					break;
				else
					cout << "输入坐标不合法，请重新输入:";
			}
			currentThisBoard[x_input][y_input] = BLACK;
			system("cls");
			Display_0();
			Display_Board(currentThatBoard);

			backUp();
			ans_ = consider(WHITE, currentThisBoardBackUp);
			cout << "威胁空间：" << ans_.x << " " << ans_.y << endl;
			if (ans_.x != -1 && ans_.y != -1 && (currentThisBoard[ans_.x][ans_.y] == NO_PIECE)) {
				currentThisBoard[ans_.x][ans_.y] = WHITE;
				x_best = ans_.x;
				y_best = ans_.y;
				tss = 1;
				ans_.x = -1;
				ans_.y = -1;
			}
			else {
				t = nega_scouter.searchNiceMove(currentThisBoard, WHITE, x_best, y_best);
			}
			system("cls");
			Display_0();
			Display_Board(currentThatBoard);
			if (tss == 1) {
				cout << "进入威胁空间搜索！" << endl;
				tss = 0;
			}
			cout << "本机落子坐标为（" << x_best << "，" << y_best << "）" << endl;
			if (fabs(evaluation_.evaluate(currentThisBoard, WHITE)) >= GAME_OVER_VALUE)
				break;
		}
	}
	else {
		opening();
		goto OPENER;
		while (!t)
		{
			cout << "请输入落子坐标:";
			while (1) {
				cin >> x_input >> y_input;
				if (x_input >= 0 && x_input < GRID_NUM && y_input >= 0 && y_input < GRID_NUM && currentThisBoard[x_input][y_input] == NO_PIECE)
					break;
				else
					cout << "输入坐标不合法，请重新输入:";
			}
			currentThisBoard[x_input][y_input] = BLACK;
OPENER:  	system("cls");
			Display_1();
			Display_Board(currentThatBoard);

			backUp();
			ans_ = consider(WHITE, currentThisBoardBackUp);
			cout << "威胁空间：" << ans_.x << " " << ans_.y << endl;
			if (ans_.x != -1 && ans_.y != -1 && (currentThisBoard[ans_.x][ans_.y] == NO_PIECE)) {
				currentThisBoard[ans_.x][ans_.y] = WHITE;
				x_best = ans_.x;
				y_best = ans_.y;
				tss = 1;
				ans_.x = -1;
				ans_.y = -1;
			}
			else {
				t = nega_scouter.searchNiceMove(currentThisBoard, WHITE, x_best, y_best);
			}
			system("cls");
			Display_1();
			Display_Board(currentThatBoard);
			if (tss == 1) {
				cout << "进入威胁空间搜索！" << endl;
				tss = 0;
			}
			cout << "本机落子坐标为（" << x_best << "，" << y_best << "）" << endl;
			if (fabs(evaluation_.evaluate(currentThisBoard, WHITE)) >= GAME_OVER_VALUE)
				break;
		}
	}
	if (t == 0)
		cout << "本机获胜！" << endl;
	else
		cout << "恭喜获胜！" << endl;
	endPoint = clock();
	cout << "花费总时间为：";
	if ((endPoint - startPoint) > 60000)
		cout << (endPoint - startPoint) / 60000 << " 分 ";
	cout << (endPoint - startPoint) % 60000 / 1000 << " 秒" << endl;
	system("pause");
	return 0;
}