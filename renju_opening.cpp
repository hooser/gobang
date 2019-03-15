// This is for I take BLACK.
#include <conio.h>
#include <iostream>
#include <fstream>
#include <stack>
#include "header_renju.h"
#include "header_opening.h"
using namespace std;

PIECEPOS input_pos;
bool needInput;
PIECEPOS output_pos;
stack<int> stackLevel;
int Rotation_type;
bool rotationHasSet = false;
int Reading_Signal;

extern int currentThisBoard[GRID_NUM][GRID_NUM];
extern char currentThatBoard[GRID_NUM][GRID_NUM];
extern void Display_0();
extern void Display_1();
extern void Display_Board(char a[][GRID_NUM]);

int opening() {
	cout << "正在调用开局库……" << endl << endl;
	Reading_Signal = READING_IN_PROGRESS;

	// ----------------------------------------

	// Naming Variants.

	Renju_File_Status status;
	status.haveChore = false;

	int round = 0;	// Round of 'while' looping.
	int step = 1;	// Step of output.
	int level = 1;	// Step of the next output method to be traversed.
	LevelPos temp_pos;

	// Read two hex num as 2 chars each time from the renju opening txt file.
	char read_pos[3];
	char read_sign[3];
	char read_chore[3];
	char read_chore_next[3];

	fstream input_file;
	input_file.open("RenjuOpeningData.txt", ios::in);


	// ----------------------------------------


	while (level >= step) {
		round++;

		// ----------------------------------------

		// Dealing chores.
		if (status.haveChore == true) {
			input_file >> read_chore >> read_chore_next;
			while (true) {
				if (strcmp(read_chore, "00") == 0 && strcmp(read_chore_next, "00") != 0)
					break;
				strcpy_s(read_chore, read_chore_next);
				input_file >> read_chore_next;
			}
			strcpy_s(read_pos, read_chore_next);
		}

		else
			input_file >> read_pos;

		// ----------------------------------------

		// Position to coordinate.
		// (x,y): Row x Column y.
		input_file >> read_sign;
		int x_temp = 0, y_temp = 0;
		// Coordinate has been offset from (5,5).
		Adjust_Pos(read_pos, x_temp, y_temp);

		// ----------------------------------------

		// Dealing coordinate and tree sign.
		status = Check_Text(read_sign);
		temp_pos = Set_Pos_Level(status, x_temp, y_temp, level);

		// ----------------------------------------

		// Analysing.

		// First black
		if (step == 1) {
			output_pos.color = BLACK;
			output_pos.x_coordinate = temp_pos.piecePos.x_coordinate;
			output_pos.y_coordinate = temp_pos.piecePos.y_coordinate;
			system("cls");
			currentThisBoard[5][5] = WHITE;
			Display_1();
			Display_Board(currentThatBoard);
			cout << "本机落子坐标为（" << output_pos.x_coordinate + 5 << "，" << output_pos.y_coordinate + 5 << "）" << endl;
			step++;
			needInput = true;
			continue;
		}

		// First white
		else if (step == 2) {
			if (needInput) {
				input_pos = Input_White_Pos();
				system("cls");
				currentThisBoard[input_pos.x_coordinate + 5][input_pos.y_coordinate + 5] = BLACK;
				Display_1();
				Display_Board(currentThatBoard);
			}
			needInput = false;

			if (temp_pos.level == 2 && (input_pos.x_coordinate > 1 || input_pos.y_coordinate > 1 || input_pos.x_coordinate < -1 || input_pos.y_coordinate < -1)) {
				if (temp_pos.piecePos.x_coordinate == 1 && temp_pos.piecePos.y_coordinate == -1) {
					Rotation_type = DEG_0;
					rotationHasSet = true;
					step++;
				}
				continue;
			}

			else if (temp_pos.piecePos.x_coordinate == 0 && temp_pos.piecePos.y_coordinate == -1 && temp_pos.level == 2) {
				if (input_pos.x_coordinate == 0 && input_pos.y_coordinate == -1) {
					Rotation_type = DEG_0;
					rotationHasSet = true;
					step++;
				}
				else if (input_pos.x_coordinate == -1 && input_pos.y_coordinate == 0) {
					Rotation_type = DEG_90;
					rotationHasSet = true;
					step++;
				}
				else if (input_pos.x_coordinate == 0 && input_pos.y_coordinate == 1) {
					Rotation_type = DEG_180;
					rotationHasSet = true;
					step++;
				}
				else if (input_pos.x_coordinate == 1 && input_pos.y_coordinate == 0) {
					Rotation_type = DEG_270;
					rotationHasSet = true;
					step++;
				}
				continue;
			}

			else if (temp_pos.piecePos.x_coordinate == 1 && temp_pos.piecePos.y_coordinate == -1 && temp_pos.level == 2) {
				if (input_pos.x_coordinate == 1 && input_pos.y_coordinate == -1) {
					Rotation_type = DEG_0;
					rotationHasSet = true;
					step++;
					continue;
				}
				else if (input_pos.x_coordinate == -1 && input_pos.y_coordinate == -1) {
					Rotation_type = DEG_90;
					rotationHasSet = true;
					step++;
					continue;
				}
				else if (input_pos.x_coordinate == -1 && input_pos.y_coordinate == 1) {
					Rotation_type = DEG_180;
					rotationHasSet = true;
					step++;
					continue;
				}
				else if (input_pos.x_coordinate == 1 && input_pos.y_coordinate == 1) {
					Rotation_type = DEG_270;
					rotationHasSet = true;
					step++;
					continue;
				}
				else break;
			}

			else continue;
		}

		// Black
		else if (step % 2 == 1 && rotationHasSet) {
			output_pos = Output_Rotate(temp_pos);
			system("cls");
			currentThisBoard[output_pos.x_coordinate + 5][output_pos.y_coordinate + 5] = WHITE;
			Display_1();
			Display_Board(currentThatBoard);
			cout << "本机落子坐标为（" << output_pos.x_coordinate + 5 << "，" << output_pos.y_coordinate + 5 << "）" << endl;
			step++;
			needInput = true;
			continue;
		}

		// White
		else if (step % 2 == 0 && rotationHasSet) {
			// Getting Input pos.
			if (needInput) {
				input_pos = Input_White_Pos();
				system("cls");
				currentThisBoard[input_pos.x_coordinate + 5][input_pos.y_coordinate + 5] = BLACK;
				Display_1();
				Display_Board(currentThatBoard);
				input_pos = Input_Rotate(input_pos);
			}
			needInput = false;

			if (step == temp_pos.level && temp_pos.piecePos.x_coordinate == input_pos.x_coordinate && temp_pos.piecePos.y_coordinate == input_pos.y_coordinate) {
				// Position met in traversing.
				step++;
				continue;
			}

			// Position didn't meet.
			else continue;
		}

	}


	// ----------------------------------------

	Reading_Signal = READING_ACCOMPLISHED;
	cout << "开局库调用结束，进入剪枝阶段." << endl;
	system("pause");
	return 0;
}

Status Adjust_Pos(char *text, int &x, int &y) {
	// To adjust the position from text to coordinate.
	if (text[0] >= '0' && text[0] <= '1') {
		y = -1;
		x = -1;
		return POS_INVALID;
	}
	else if (text[0] >= '2' && text[0] <= '9') {
		y = int(text[0] - '2');
	}
	else {
		y = int(text[0] - 'A' + 8);
	}

	if (text[1] >= '1' && text[1] <= '2') {
		y = -1;
		x = -1;
		return POS_INVALID;
	}
	else if (text[1] >= '3' && text[1] <= '9') {
		x = int(text[1] - '3');
	}
	else {
		x = int(text[1] - 'A' + 7);
	}
	// Offset from (5,5).
	x -= 5;
	y -= 5;

	return GOOD_POS_ADJUST;
}

Renju_File_Status Check_Text(char *text) {
	// To check the status of the text read.
	Renju_File_Status status;

	if (text[0] == '0')
		status.tree_status = HAVE_SON_NO_RBRO;
	if (text[0] == '8')
		status.tree_status = HAVE_SON_HAVE_RBRO;
	if (text[0] == 'C')
		status.tree_status = NO_SON_HAVE_RBRO;
	if (text[0] == '4')
		status.tree_status = LAST_LEAF;

	if (text[1] == '0')
		status.haveChore = false;
	else
		status.haveChore = true;

	return status;
}

LevelPos Set_Pos_Level(Renju_File_Status status, int x_temp, int y_temp, int &level) {

	LevelPos temp_pos;
	if (status.tree_status == HAVE_SON_NO_RBRO) {
		temp_pos.piecePos.x_coordinate = x_temp;
		temp_pos.piecePos.y_coordinate = y_temp;
		temp_pos.level = level++;
	}

	if (status.tree_status == NO_SON_HAVE_RBRO) {
		temp_pos.piecePos.x_coordinate = x_temp;
		temp_pos.piecePos.y_coordinate = y_temp;
		temp_pos.level = level;
	}

	if (status.tree_status == HAVE_SON_HAVE_RBRO) {
		temp_pos.piecePos.x_coordinate = x_temp;
		temp_pos.piecePos.y_coordinate = y_temp;
		temp_pos.level = level;
		stackLevel.push(level++);
	}

	if (status.tree_status == LAST_LEAF) {
		temp_pos.piecePos.x_coordinate = x_temp;
		temp_pos.piecePos.y_coordinate = y_temp;
		temp_pos.level = level;
		level = stackLevel.top();
		stackLevel.pop();
	}

	return temp_pos;
}

PIECEPOS Input_White_Pos() {
	PIECEPOS out;
	int x, y;
	cout << "请输入落子坐标:";
	while (1) {
		cin >> x >> y;
		if (x >= 0 && x < GRID_NUM && y >= 0 && y < GRID_NUM&&currentThisBoard[x][y] == NO_PIECE)
			break;
		else
			cout << "输入坐标不合法，请重新输入:";
	}
	out.color = WHITE;
	out.x_coordinate = x - 5;
	out.y_coordinate = y - 5;
	return out;
}

PIECEPOS Output_Rotate(LevelPos in) {
	PIECEPOS out;
	out.color = BLACK;
	if (Rotation_type == DEG_0) {
		out.x_coordinate = in.piecePos.x_coordinate;
		out.y_coordinate = in.piecePos.y_coordinate;
	}
	else if (Rotation_type == DEG_90) {
		out.x_coordinate = in.piecePos.y_coordinate;
		out.y_coordinate = 0 - in.piecePos.x_coordinate;
	}
	else if (Rotation_type == DEG_180) {
		out.x_coordinate = 0 - in.piecePos.x_coordinate;
		out.y_coordinate = 0 - in.piecePos.y_coordinate;
	}
	else if (Rotation_type == DEG_270) {
		out.x_coordinate = 0 - in.piecePos.y_coordinate;
		out.y_coordinate = in.piecePos.x_coordinate;
	}

	return out;
}

PIECEPOS Input_Rotate(PIECEPOS in) {
	PIECEPOS out;
	out.color = WHITE;
	if (Rotation_type == DEG_0) {
		out.x_coordinate = in.x_coordinate;
		out.y_coordinate = in.y_coordinate;
	}
	else if (Rotation_type == DEG_90) {
		out.x_coordinate = 0 - in.y_coordinate;
		out.y_coordinate = in.x_coordinate;
	}
	else if (Rotation_type == DEG_180) {
		out.x_coordinate = 0 - in.x_coordinate;
		out.y_coordinate = 0 - in.y_coordinate;
	}
	else if (Rotation_type == DEG_270) {
		out.x_coordinate = in.y_coordinate;
		out.y_coordinate = 0 - in.x_coordinate;
	}

	return out;
}