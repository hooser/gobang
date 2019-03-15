#include "header_renju.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

#define S_TWO 1
#define S_THREE 2
#define S_FOUR 3
#define TWO 4
#define THREE 5
#define FOUR 6
#define FIVE 7
#define UNANALYSIED 0
#define ANALYSIED GRID_NUM * GRID_NUM

int count = 0;

int positionValue[GRID_NUM][GRID_NUM] =
{
	{ 0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,2,2,2,2,2,2,2,2,1,0 },
	{ 0,1,2,3,3,3,3,3,3,2,1,0 },
	{ 0,1,2,3,4,4,4,4,3,2,1,0 },
	{ 0,1,2,3,4,5,5,4,3,2,1,0 },
	{ 0,1,2,3,4,5,5,4,3,2,1,0 },
	{ 0,1,2,3,4,4,4,4,3,2,1,0 },
	{ 0,1,2,3,3,3,3,3,3,2,1,0 },
	{ 0,1,2,2,2,2,2,2,2,2,1,0 },
	{ 0,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0 }
};

evaluation::evaluation()
{

}

evaluation::~evaluation()
{

}

int evaluation::evaluate(int position[][GRID_NUM], int isWhite)
{
	int patternType;
	/* 初始化 */
	memset(TypeRecord, UNANALYSIED, sizeof(TypeRecord));
	memset(TypeCount, 0, sizeof(TypeCount));

	/* 分析棋盘 */
	for (int i = 0; i < GRID_NUM; i++)
		for (int j = 0; j < GRID_NUM; j++)
		{
			if (position[i][j] != NO_PIECE)
			{
				if (TypeRecord[i][j][0] == UNANALYSIED)
				{
					analyzeHorizonal(position, i, j);
				}

				if (TypeRecord[i][j][1] == UNANALYSIED)
				{
					analyzeVertical(position, i, j);
				}

				if (TypeRecord[i][j][2] == UNANALYSIED)
				{
					analyzeLeftSlide(position, i, j);
				}

				if (TypeRecord[i][j][3] == UNANALYSIED)
				{
					analyzeRightSlide(position, i, j);
				}
			}
		}
	//对结果进行分析
	for (int i = 0; i < GRID_NUM; i++)
		for (int j = 0; j < GRID_NUM; j++)
		{
			patternType = position[i][j];
			for (int k = 0; k < 4; k++)
			{

				if (patternType != NO_PIECE)
				{
					switch (TypeRecord[i][j][k])
					{
					case FIVE:
						TypeCount[patternType][FIVE]++;
						break;
					case FOUR:
						TypeCount[patternType][FOUR]++;
						break;
					case S_FOUR:
						TypeCount[patternType][S_FOUR]++;
						break;
					case THREE:
						TypeCount[patternType][THREE]++;
						break;
					case S_THREE:
						TypeCount[patternType][S_THREE]++;
						break;
					case TWO:
						TypeCount[patternType][TWO]++;
						break;
					case S_TWO:
						TypeCount[patternType][S_TWO]++;
						break;
					default:
						break;
					}
				}
			}
		}


	/* 对于NegaScout注意正负值替换 */
	if (isWhite == WHITE)
	{
		if (TypeCount[BLACK][FIVE])
		{
			return 200000;
		}

		if (TypeCount[WHITE][FIVE])
		{
			return -300000;
		}
	}
	else
	{
		if (TypeCount[WHITE][FIVE])
		{
			return 200000;
		}

		if (TypeCount[BLACK][FIVE])
		{
			return -300000;
		}

	}

	/* 预处理棋形 */
	if (TypeCount[WHITE][S_FOUR] > 1)
	{
		TypeCount[WHITE][FOUR]++;
	}

	if (TypeCount[BLACK][S_FOUR] > 1)
	{
		TypeCount[BLACK][FOUR]++;
	}

	if (TypeCount[WHITE][S_FOUR] > 1)
		TypeCount[WHITE][FOUR]++;

	if (TypeCount[BLACK][S_FOUR] > 1)
		TypeCount[BLACK][FOUR]++;

	int white_score = 0;
	int black_score = 0;

	/* 计算棋形分数 */
	if (isWhite == WHITE)
	{
		if (TypeCount[BLACK][FOUR])
		{
			black_score += 16000;
		}

		if (TypeCount[BLACK][S_FOUR])
		{
			black_score += 12000;
		}

		if (TypeCount[WHITE][FOUR])
		{
			white_score += 14000;
		}

		if (TypeCount[WHITE][S_FOUR])
		{
			white_score += 10000;
		}

		
		if (TypeCount[BLACK][THREE])
		{
			black_score += TypeCount[BLACK][THREE] * 500;
		}

		if (TypeCount[WHITE][THREE])
		{
			white_score += TypeCount[WHITE][THREE] * 100;
		}

		
		if (TypeCount[BLACK][S_THREE])
		{
			black_score += TypeCount[BLACK][S_THREE] * 200;
		}

		if (TypeCount[WHITE][S_THREE])
		{
			white_score += TypeCount[BLACK][S_THREE] * 20;
		}


		
		if (TypeCount[WHITE][TWO])
		{
			white_score += TypeCount[WHITE][TWO] * 6;
		}

		if (TypeCount[BLACK][TWO])
		{
			black_score += TypeCount[BLACK][TWO] * 6;
		}

		
		if (TypeCount[WHITE][S_TWO])
		{
			white_score += TypeCount[WHITE][S_TWO] * 2;
		}

		if (TypeCount[BLACK][S_TWO])
		{
			black_score += TypeCount[BLACK][S_TWO] * 2;
		}
	}
	else
	{
		
		if (TypeCount[WHITE][FOUR])
		{
			white_score += 16000;
		}

		if (TypeCount[WHITE][S_FOUR])
		{
			white_score += 12000;
		}

		if (TypeCount[BLACK][FOUR])
		{
			black_score += 14000;
		}

		if (TypeCount[BLACK][S_FOUR])
		{
			black_score += 10000;
		}

		
		if (TypeCount[WHITE][THREE])
		{
			white_score += TypeCount[WHITE][THREE] * 500;
		}

		if (TypeCount[BLACK][THREE])
		{
			black_score += TypeCount[BLACK][THREE] * 100;
		}
		

		if (TypeCount[WHITE][S_THREE])
		{
			white_score += TypeCount[WHITE][S_THREE] * 200;
		}
		if (TypeCount[BLACK][S_THREE])
		{
			black_score += TypeCount[BLACK][S_THREE] * 20;
		}

		
		if (TypeCount[BLACK][TWO])
		{
			black_score += TypeCount[BLACK][TWO] * 6;
		}
		if (TypeCount[WHITE][TWO])
		{
			white_score += TypeCount[WHITE][TWO] * 6;
		}

		
		if (TypeCount[BLACK][S_TWO])
		{
			black_score += TypeCount[BLACK][S_TWO] * 2;
		}
		if (TypeCount[WHITE][S_TWO])
		{
			white_score += TypeCount[WHITE][S_TWO] * 2;;
		}
	}

	/* 附加落子位价值 */
	for (int i = 0; i<GRID_NUM; i++)
		for (int j = 0; j < GRID_NUM; j++)
		{
			patternType = position[i][j];
			if (patternType != NO_PIECE)
			{
				if (patternType == BLACK)
					black_score += positionValue[i][j];
				else
					white_score += positionValue[i][j];
			}
		}

	if (isWhite == WHITE)
	{
		return black_score - white_score;
	}
	else
	{
		return white_score - black_score;
	}
}

/* 水平分析 */
int evaluation::analyzeHorizonal(int position[][GRID_NUM], int i, int j)
{
	analyzer(position[i], GRID_NUM, j);
	for (int s = 0; s < GRID_NUM; s++)
	{
		if (LinearSample[s] != UNANALYSIED)
		{
			TypeRecord[i][s][0] = LinearSample[s];
		}
	}
	return TypeRecord[i][j][0];
}

/* 垂直分析 */
int evaluation::analyzeVertical(int position[][GRID_NUM], int i, int j)
{
	int temp[GRID_NUM];
	for (int s = 0; s < GRID_NUM; s++)
	{
		temp[s] = position[s][j];
	}
	analyzer(temp, GRID_NUM, i);

	for (int s = 0; s < GRID_NUM; s++)
	{
		if (LinearSample[s] != UNANALYSIED)
			TypeRecord[s][j][1] = LinearSample[s];
	}
	return TypeRecord[i][j][1];
}

/* 左斜分析 */
int evaluation::analyzeLeftSlide(int position[][GRID_NUM], int i, int j)
{
	int temp[GRID_NUM];
	int x, y, k;
	if (i < j)
	{
		y = 0;
		x = j - i;
	}
	else
	{
		y = i - j;
		x = 0;
	}

	for (k = 0; k < GRID_NUM; k++)
	{
		if (y + k > GRID_NUM - 1 || x + k > GRID_NUM - 1)
			break;
		temp[k] = position[y + k][x + k];
	}
	analyzer(temp, k, j - x);

	for (int s = 0; s < k; s++)
	{
		if (LinearSample[s] != UNANALYSIED)
		{
			TypeRecord[y + s][x + s][2] = LinearSample[s];
		}
	}

	return TypeRecord[i][j][2];
}

/* 右斜分析 */
int evaluation::analyzeRightSlide(int position[][GRID_NUM], int i, int j)
{
	int temp[GRID_NUM];
	int x, y, k;
	if (i + j >GRID_NUM - 1)
	{
		y = GRID_NUM - 1;
		x = i + j - GRID_NUM + 1;
	}
	else
	{
		x = 0;
		y = i + j;
	}

	for (k = 0; k < GRID_NUM; k++)
	{
		if (y - k <0 || x + k > GRID_NUM - 1)
			break;
		temp[k] = position[y - k][x + k];
	}
	analyzer(temp, k, j - x);

	for (int s = 0; s < k; s++)
	{
		if (LinearSample[s] != UNANALYSIED)
		{
			TypeRecord[y - s][x + s][3] = LinearSample[s];
		}
	}

	return TypeRecord[i][j][3];
}

int evaluation::analyzer(int position[], int Length, int pos)
{
	int patternType;
	int AnalyLine[GRID_NUM];		//
	int AnalyPos;
	int LeftEdge, RightEdge;
	int LeftRange, rightRange;
	int leftfour;
	if (Length < 5)
	{
		memset(LinearSample, ANALYSIED, sizeof(int)*Length);
		return 0;
	}

	AnalyPos = pos;
	memset(LinearSample, UNANALYSIED, sizeof(int) * GRID_NUM);
	memcpy(AnalyLine, position, sizeof(int)*Length);
	Length--;
	patternType = AnalyLine[pos];
	LeftEdge = AnalyPos;
	RightEdge = AnalyPos;

	while (LeftEdge > 0)
	{
		if (AnalyLine[LeftEdge - 1] != patternType)
			break;
		LeftEdge--;
	}

	while (RightEdge < Length)
	{
		if (AnalyLine[RightEdge + 1] != patternType)
			break;
		RightEdge++;
	}

	LeftRange = LeftEdge;
	rightRange = RightEdge;

	while (LeftRange > 0)
	{
		if (AnalyLine[LeftRange - 1] == !patternType)
			break;
		LeftRange--;
	}

	while (rightRange < Length)
	{
		if (AnalyLine[rightRange + 1] == !patternType)
			break;
		rightRange++;
	}

	if (rightRange - LeftRange < 4)
	{
		for (int i = LeftRange; i <= rightRange; i++)
			LinearSample[i] = ANALYSIED;
		return FALSE;
	}

	for (int k = LeftEdge; k <= RightEdge; k++)
	{
		LinearSample[k] = ANALYSIED;
	}

	if (RightEdge - LeftEdge == 4)
	{
		LinearSample[pos] = FIVE;
		return FIVE;
	}

	leftfour = FALSE;
	if (RightEdge - LeftEdge == 3)
	{
		if (LeftEdge > 0)
		{
			if (AnalyLine[LeftEdge - 1] == NO_PIECE)
				leftfour = TRUE;
		}

		if (RightEdge < Length)
		{
			if (AnalyLine[RightEdge + 1] == NO_PIECE)
			{
				if (leftfour)
					LinearSample[pos] = FOUR;
				else
					LinearSample[pos] = S_FOUR;
			}
			else
			{
				if (leftfour)
					LinearSample[pos] = S_FOUR;
			}
		}
		else
		{
			if (leftfour)
				LinearSample[pos] = S_FOUR;
		}

		return LinearSample[pos];
	}

	if (RightEdge - LeftEdge == 2)
	{
		int leftthree = FALSE;
		if (LeftEdge > 0)
		{
			if (AnalyLine[LeftEdge - 1] == NO_PIECE)
			{
				if (LeftEdge > 1 && AnalyLine[LeftEdge - 2] == patternType)
				{
					LinearSample[LeftEdge] = S_FOUR;
					LinearSample[LeftEdge - 2] = ANALYSIED;
				}
				else
				{
					leftthree = TRUE;
				}
			}
			else
			{
				if (RightEdge == Length || AnalyLine[RightEdge + 1] != NO_PIECE)
					return FALSE;
			}
		}

		if (RightEdge < Length)
		{
			if (AnalyLine[RightEdge + 1] == NO_PIECE)
			{
				if (RightEdge < Length - 1 && AnalyLine[RightEdge + 2] == AnalyLine[RightEdge])
				{
					LinearSample[RightEdge] = S_FOUR;
					LinearSample[RightEdge + 2] = ANALYSIED;
				}
				else
				{
					if (leftthree)
					{
						LinearSample[RightEdge] = THREE;
					}
					else
					{
						LinearSample[RightEdge] = S_THREE;
					}
				}
			}
			else
			{

				if (LinearSample[LeftEdge] == S_FOUR)
				{
					return LinearSample[LeftEdge];
				}

				if (leftthree)
				{
					LinearSample[pos] = S_THREE;
				}
			}

			return LinearSample[pos];
		}
		else
		{
			if (LinearSample[LeftEdge] == S_FOUR)
			{
				return LinearSample[LeftEdge];
			}

			if (leftthree)
			{
				LinearSample[pos] = S_THREE;
			}
		}

		return LinearSample[pos];
	}

	if (RightEdge - LeftEdge == 1)
	{
		int lefttwo = FALSE;
		int leftthree = FALSE;
		if (LeftEdge > 2)
		{
			if (AnalyLine[LeftEdge - 1] == NO_PIECE)
			{
				if (AnalyLine[LeftEdge - 2] == AnalyLine[LeftEdge])
				{
					if (AnalyLine[LeftEdge - 3] == patternType)
					{
						AnalyLine[LeftEdge - 3] = ANALYSIED;
						AnalyLine[LeftEdge - 2] = ANALYSIED;
						AnalyLine[LeftEdge] = S_FOUR;
					}
					{
						LinearSample[LeftEdge - 2] = ANALYSIED;
						LinearSample[LeftEdge] = S_THREE;
					}
				}
				else
				{
					lefttwo = TRUE;
				}
			}
		}

		if (RightEdge < Length - 2)
		{
			if (AnalyLine[RightEdge + 1] == NO_PIECE)
			{
				if (AnalyLine[RightEdge + 2] == AnalyLine[RightEdge])
				{
					if (AnalyLine[RightEdge + 3] == AnalyLine[RightEdge])
					{
						LinearSample[RightEdge + 3] = ANALYSIED;
						LinearSample[RightEdge + 2] = ANALYSIED;
						LinearSample[RightEdge] = S_FOUR;
					}
					else
					{
						LinearSample[RightEdge + 2] = ANALYSIED;
						if (lefttwo)
						{
							LinearSample[RightEdge] = S_THREE;
						}
						else
						{
							if (AnalyLine[RightEdge + 3] == NO_PIECE)
							{
								LinearSample[RightEdge] = S_THREE;
							}
						}

					}
				}
				else
				{
					if (LinearSample[LeftEdge] == S_FOUR)
					{
						return LinearSample[LeftEdge];
					}

					if (LinearSample[LeftEdge] == S_THREE)
					{
						return LinearSample[LeftEdge];
					}

					if (lefttwo)
					{
						LinearSample[pos] = TWO;
					}
					else
					{
						LinearSample[pos] = S_TWO;
					}
				}
			}
			else
			{
				if (LinearSample[LeftEdge] == S_FOUR)
				{
					return LinearSample[LeftEdge];
				}

				if (LinearSample[LeftEdge] == S_THREE)
				{
					return LinearSample[LeftEdge];
				}
				if (lefttwo)
				{
					LinearSample[pos] = S_TWO;
				}
			}
		}

		return LinearSample[pos];
	}

	return 0;
}
