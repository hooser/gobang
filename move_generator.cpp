#include"header_renju.h"

moveGenerator::moveGenerator()
{

}

moveGenerator::~moveGenerator()
{

}

/* 产生走法 */
int moveGenerator::addMove(int x_coordinate, int y_coordinate, int layer)
{
	MoveList[layer][moveCount].piecePos.x_coordinate = x_coordinate;
	MoveList[layer][moveCount].piecePos.y_coordinate = y_coordinate;
	MoveList[layer][moveCount].score = positionValue[y_coordinate][x_coordinate];
	moveCount++;
	return moveCount;
}

/* 产生走法数量 */
int moveGenerator::createMove(int position[GRID_NUM][GRID_NUM], int layer, int player)
{

	moveCount = 0;
	for (int i = 0; i < GRID_NUM; i++)
		for (int j = 0; j < GRID_NUM; j++)
		{
			if (position[i][j] == NO_PIECE)
				addMove(j, i, layer);
		}
	return moveCount;
}