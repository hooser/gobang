#include"header_renju.h"
#include<stack>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <cmath>
#include "search.h"
using namespace std;

/* �����߷���ջ */
stack<PIECEMOVE> stackMove;

extern int opening();
extern ans consider(char player, int(*currentThisBoard)[GRID_NUM]);

/* ����ʾ��ͼ */
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

/* �������� */
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
/* �Է����� */
char currentThatBoard[GRID_NUM][GRID_NUM];

/* ����������� */
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

/* ����������� */
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

/* ����������� */
void Display_Board(char a[][GRID_NUM])
{
	/* �б� */
	cout << "   ";
	for (int i = 0; i < GRID_NUM; i++)
	{
		printf("%3d", i);
	}
	cout << endl;

	/* �б� */
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
	/* ���ü�ʱʱ�� */
	clock_t startPoint, endPoint;
	startPoint = clock();

	/* �Է��������� */
	int x_input, y_input;
	/* ��ʼ������� */
	int depth = INIT_DEPTH;
	/* ��ʼ�������� */
	negaScouter nega_scouter;
	historyHeuristic history_heuristic;
	evaluation evaluation_;
	moveGenerator move_generator;
	ans ans_;
	ans_.x = -1;
	ans_.y = -1;

	nega_scouter.setSearchDepth(depth);	//�����������
	nega_scouter.setHistoryHeuristic(&history_heuristic);	//������ʷ������
	nega_scouter.setEvaluation(&evaluation_);	//������������
	nega_scouter.setMoveGenerator(&move_generator);	//�����߷���
	memset(currentThisBoard, NO_PIECE, sizeof(currentThisBoard));//��ʼ������
	char ch;
	cout << "��ѡ���Ⱥ��֣�0->�������֣�1->��������:";
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
			cout << "��������������:";
			while (1) {
				cin >> x_input >> y_input;
				if (x_input >= 0 && x_input < GRID_NUM && y_input >= 0 && y_input < GRID_NUM&&currentThisBoard[x_input][y_input] == NO_PIECE)
					break;
				else
					cout << "�������겻�Ϸ�������������:";
			}
			currentThisBoard[x_input][y_input] = BLACK;
			system("cls");
			Display_0();
			Display_Board(currentThatBoard);

			backUp();
			ans_ = consider(WHITE, currentThisBoardBackUp);
			cout << "��в�ռ䣺" << ans_.x << " " << ans_.y << endl;
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
				cout << "������в�ռ�������" << endl;
				tss = 0;
			}
			cout << "������������Ϊ��" << x_best << "��" << y_best << "��" << endl;
			if (fabs(evaluation_.evaluate(currentThisBoard, WHITE)) >= GAME_OVER_VALUE)
				break;
		}
	}
	else {
		opening();
		goto OPENER;
		while (!t)
		{
			cout << "��������������:";
			while (1) {
				cin >> x_input >> y_input;
				if (x_input >= 0 && x_input < GRID_NUM && y_input >= 0 && y_input < GRID_NUM && currentThisBoard[x_input][y_input] == NO_PIECE)
					break;
				else
					cout << "�������겻�Ϸ�������������:";
			}
			currentThisBoard[x_input][y_input] = BLACK;
OPENER:  	system("cls");
			Display_1();
			Display_Board(currentThatBoard);

			backUp();
			ans_ = consider(WHITE, currentThisBoardBackUp);
			cout << "��в�ռ䣺" << ans_.x << " " << ans_.y << endl;
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
				cout << "������в�ռ�������" << endl;
				tss = 0;
			}
			cout << "������������Ϊ��" << x_best << "��" << y_best << "��" << endl;
			if (fabs(evaluation_.evaluate(currentThisBoard, WHITE)) >= GAME_OVER_VALUE)
				break;
		}
	}
	if (t == 0)
		cout << "������ʤ��" << endl;
	else
		cout << "��ϲ��ʤ��" << endl;
	endPoint = clock();
	cout << "������ʱ��Ϊ��";
	if ((endPoint - startPoint) > 60000)
		cout << (endPoint - startPoint) / 60000 << " �� ";
	cout << (endPoint - startPoint) % 60000 / 1000 << " ��" << endl;
	system("pause");
	return 0;
}