#include"header_renju.h"

historyHeuristic::historyHeuristic()
{
	resetHistoryTable();
}

historyHeuristic::~historyHeuristic()
{

}

/* ������ʷ�÷ֱ� */
void historyHeuristic::resetHistoryTable()
{
	for (int i = 0; i<GRID_NUM; i++)
		for (int j = 0; j < GRID_NUM; j++)
		{
			historyTable[i][j] = HISTORY_TABLE_INIT_VALUE;
		}
}

/* ��ȡ��������÷� */
int historyHeuristic::getHistoryScore(pieceMove move)
{
	return historyTable[move.piecePos.y_coordinate][move.piecePos.x_coordinate];
}

/* ��ָ���߷�������ʷ��¼ */
void historyHeuristic::insertHistoryScore(pieceMove move, int depth)
{
	historyTable[move.piecePos.y_coordinate][move.piecePos.x_coordinate] += 2 << depth;
}

/* �������������� */
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

/* ���Ÿ������� */
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
