#include"header_renju.h"

historyHeuristic::historyHeuristic()
{
	resetHistoryTable();
}

historyHeuristic::~historyHeuristic()
{

}

/* 重置历史得分表 */
void historyHeuristic::resetHistoryTable()
{
	for (int i = 0; i<GRID_NUM; i++)
		for (int j = 0; j < GRID_NUM; j++)
		{
			historyTable[i][j] = HISTORY_TABLE_INIT_VALUE;
		}
}

/* 获取给定步骤得分 */
int historyHeuristic::getHistoryScore(pieceMove move)
{
	return historyTable[move.piecePos.y_coordinate][move.piecePos.x_coordinate];
}

/* 将指定走法加入历史记录 */
void historyHeuristic::insertHistoryScore(pieceMove move, int depth)
{
	historyTable[move.piecePos.y_coordinate][move.piecePos.x_coordinate] += 2 << depth;
}

/* 将所得数据排序 */
void historyHeuristic::Q_Sort(pieceMove *source, int low, int high)
{
	int i;
	if (low < high)
	{
		i = Q_Sort_partition(source, low, high);
		Q_Sort(source, low, i - 1);
		Q_Sort(source, i + 1, high);
	}
}

/* 快排辅助函数 */
int historyHeuristic::Q_Sort_partition(pieceMove *source, int low, int high)
{
	pieceMove temp;
	temp = source[low];
	int score = temp.score;
	while (low < high)
	{
		while (source[high].score <= score&&low < high)
			high--;
		source[low] = source[high];
		while (source[low].score > score&&low < high)
			low++;
		source[high] = source[low];
	}
	source[low] = temp;
	return low;
}
