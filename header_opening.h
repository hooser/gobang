#include"header_renju.h"
typedef struct LevelPos
{
	PIECEPOS piecePos;  // 棋子位置
	int level;			// 第几手
}LevelPos;

typedef struct Renju_File_Status {
	int		tree_status;
	bool	haveChore;
}Renju_File_Status;
typedef int Status;
typedef int ReadSig;

// Tree status
#define HAVE_SON_NO_RBRO		0
#define HAVE_SON_HAVE_RBRO		1
#define NO_SON_HAVE_RBRO		2
#define LAST_LEAF				3

// Pos adjust status
#define GOOD_POS_ADJUST			4
#define POS_INVALID				5

// Reading signal
#define READING_IN_PROGRESS		6
#define READING_ACCOMPLISHED	7

// Rotation Type (Clockwise/LeftRight/UpDown)
#define DEG_0					8
#define DEG_90					9
#define DEG_180					10
#define DEG_270					11


Status Adjust_Pos(char *, int &, int &);
Renju_File_Status Check_Text(char *);
LevelPos Set_Pos_Level(Renju_File_Status, int, int, int &);
PIECEPOS Input_White_Pos();
PIECEPOS Output_Rotate(LevelPos);
PIECEPOS Input_Rotate(PIECEPOS);

#pragma once