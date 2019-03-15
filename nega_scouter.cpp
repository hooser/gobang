#include"header_renju.h"
#include <iostream>
#include<math.h>

negaScouter::negaScouter()
{
	transposition_table.HASH_initTable();
	history_heuristic.resetHistoryTable();
}

negaScouter::~negaScouter()
{

}

/* 根据某一走法产生之后的棋局 */
void negaScouter::makeBoard(pieceMove *move, int type)
{
	currentBoard[move->piecePos.y_coordinate][move->piecePos.x_coordinate] = type;
}

/* 恢复之前的棋盘 */
void negaScouter::unmakeBoard(pieceMove *move)
{
	currentBoard[move->piecePos.y_coordinate][move->piecePos.x_coordinate] = NO_PIECE;
}

/* 判断游戏是否结束 */
int negaScouter::isGameOver(int position[][GRID_NUM], int type)
{
	int score, result_0 = 0;
	int *result = &result_0;
	//score = pointer_evaluation->evaluate(position, type, result);
	score = pointer_evaluation->evaluate(position, type);
	if (fabs(*result) >= GAME_OVER_VALUE)
	{
		return *result;//结束，返回极值
	}
	return 0;
}

/* 确定最大搜索深度 */
void negaScouter::setSearchDepth(int depth)
{
	this->maxDepth = depth;
}

/* 设置走法产生器 */
void negaScouter::setMoveGenerator(moveGenerator *move_generator)
{
	this->pointer_moveGenerator = move_generator;
}

/* 设置历史启发类 */
void negaScouter::setHistoryHeuristic(historyHeuristic *history_heuristic)
{
	this->pointer_historyHeuristic = history_heuristic;
}

/* 设置评估函数 */
void negaScouter::setEvaluation(evaluation *evaluation)
{
	this->pointer_evaluation = evaluation;
}

/* 查找好的下子位置 */
int negaScouter::searchNiceMove(int position[][GRID_NUM], int type, int &x_best, int &y_best)
{
	int Score, over = 0;
	int *is_over = &over;
	//over = -pointer_evaluation->evaluate(position, type, is_over);
	over = pointer_evaluation->evaluate(position, type);
	//cout << over << endl;
	if (fabs(*is_over) >= GAME_OVER_VALUE)
	{
		return *is_over;
	}
	transposition_table.HASH_calculateInitKey(position);//初始化哈希表
	history_heuristic.resetHistoryTable();
	memcpy(currentBoard, position, sizeof(int) * GRID_COUNT);
	currentDepth = maxDepth;
	Score = negaScout(currentDepth, -INT_MAX, INT_MAX);
	makeBoard(&bestMove, type);
	memcpy(position, currentBoard, sizeof(int) * GRID_COUNT);
	x_best = bestMove.piecePos.y_coordinate;
	y_best = bestMove.piecePos.x_coordinate;
	if (fabs(Score) >= GAME_OVER_VALUE)
	{
		return Score;
	}
	return 0;
}

/* 搜索当前最佳路径 */
int negaScouter::negaScout(int depth, int alpha, int beta)
{
	//int type, i;
	int a, b, t, side, score;

	side = (maxDepth - depth) % 2;

	score = transposition_table.HASH_searchNode(alpha, beta, depth, side);

	if (score != HASH_NON_EXISTENCE_VALUE)
		return score;

	if (depth == 0)
	{
		int over = 0;
		int *is_over = &over;
		//score = pointer_evaluation->evaluate(currentBoard, !side, is_over);
		score = pointer_evaluation->evaluate(currentBoard, !side);
		transposition_table.HASH_insertNode(exact_value, score, depth, side);
		return score;
	}

	int Count = pointer_moveGenerator->createMove(currentBoard, depth, side);

	for (int i = 0; i < Count; i++)
	{
		pointer_moveGenerator->MoveList[depth][i].score = history_heuristic.getHistoryScore(pointer_moveGenerator->MoveList[depth][i]);
	}
	history_heuristic.Q_Sort(pointer_moveGenerator->MoveList[depth], 0, Count);

	int best = -1;

	a = alpha;
	b = beta;
	int is_evaluation_exact = 0;
	for (int i = 0; i < Count; i++)
	{
		makeBoard(&pointer_moveGenerator->MoveList[depth][i], side);
		transposition_table.HASH_makeMove(&pointer_moveGenerator->MoveList[depth][i], currentBoard);
		t = -negaScout(depth - 1, -b, -a);

		if (t > a&&t < beta&&i>0)
		{
			is_evaluation_exact = 1;
			a = -negaScout(depth - 1, -beta, -t);
			if (depth == maxDepth)
				bestMove = pointer_moveGenerator->MoveList[depth][i];
			best = i;
		}
		transposition_table.HASH_unmakeMove(&pointer_moveGenerator->MoveList[depth][i], currentBoard);
		unmakeBoard(&pointer_moveGenerator->MoveList[depth][i]);
		if (a < t)
		{
			is_evaluation_exact = 1;
			a = t;
			if (depth == maxDepth)
				bestMove = pointer_moveGenerator->MoveList[depth][i];

		}
		if (a >= beta)
		{
			transposition_table.HASH_insertNode(lower_bound, a, depth, side);
			history_heuristic.insertHistoryScore(pointer_moveGenerator->MoveList[depth][i], depth);
			return a;
		}
		b = a + 1;
	}

	if (best != -1)
		history_heuristic.insertHistoryScore(pointer_moveGenerator->MoveList[depth][best], depth);
	if (is_evaluation_exact == 1)
		transposition_table.HASH_insertNode(exact_value, a, depth, side);
	else
		transposition_table.HASH_insertNode(upper_bound, a, depth, side);
	/*
	if(a>0)cout << a << endl;
	
	getchar();
	getchar();
	*/
	return a;
}