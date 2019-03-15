#pragma once
#include <iostream>
#define TRUE 1
#define FALSE 0
#define GRID_NUM 12
#define GRID_COUNT GRID_NUM * GRID_NUM
#define BLACK 1
#define WHITE 0
#define NO_PIECE -1
#define INIT_DEPTH 4
#define HISTORY_TABLE_INIT_VALUE 10
#define GAME_OVER_VALUE 88888
#define HASH_NON_EXISTENCE_VALUE 233333
#define isValidPos(x,y) ((x>=0 && x<GRID_NUM) && (y>=0 && y<GRID_NUM))
using namespace std;
extern int renjuBoard[GRID_NUM][GRID_NUM];
extern int positionValue[GRID_NUM][GRID_NUM];

/* 哈希表中储存数据类型 */
enum ENTRY_TYPE
{
	exact_value, lower_bound, upper_bound
};

/* 12x12的位棋盘储存棋盘状态 */
/* (2,1)是第三行第二列 */
class bitBoard
{
	public:
		int board[GRID_NUM][GRID_NUM];
		int update = false;
		void setPiece(int x_coordinate, int y_coordinate, int color) {
			board[x_coordinate][y_coordinate] = color;
		}
		void setBoard() {
			for (int i = 0; i < GRID_COUNT; i++) {
				for (int j = 0; j < GRID_COUNT; j++) {
					board[i][j] = renjuBoard[i][j];
				}
			}
		}
};

/* 表示棋子位置 */
typedef struct piecePosition
{
	int x_coordinate;
	int y_coordinate;
	int color = NO_PIECE;
}PIECEPOS;

/* 描述棋子走法 */
typedef struct pieceMove
{
	PIECEPOS piecePos;  //棋子位置
	int score;  //走法得分
}PIECEMOVE;

/* 单个哈希项 */
typedef struct hashItem
{
	/* 校验码（64位） */
	long long check_sum;
	/* 对应存储数据类型 */
	ENTRY_TYPE entry_type;
	/* 对应递归层度 */
	short depth;
	/* 对应节点的值 */
	short evaluation;
}HASHITEM;

typedef struct ans
{
	int x, y;
}ans;

/*
class evaluation {
public:
	evaluation();
	~evaluation();
	int evaluate(int position[][GRID_NUM], int player, int *result);
private:
	void analyze(int player, string line);
	void findState(int position[][GRID_NUM]);
};*/

class evaluation {
public:
	evaluation();
	~evaluation();
	int evaluate(int position[][GRID_NUM], int isWhite);
protected:
	int analyzeHorizonal(int position[][GRID_NUM], int i, int j);
	int analyzeVertical(int position[][GRID_NUM], int i, int j);
	int analyzeLeftSlide(int position[][GRID_NUM], int i, int j);
	int analyzeRightSlide(int position[][GRID_NUM], int i, int j);
	int analyzer(int position[], int Num, int p);
	int LinearSample[GRID_NUM];
	int TypeRecord[GRID_NUM][GRID_NUM][4];
	int TypeCount[2][20];
};

class moveGenerator {
public:
	moveGenerator();
	~moveGenerator();
	/* 产生走法数量 */
	int createMove(int position[GRID_NUM][GRID_NUM], int layer, int player);
	/* 保存走法的数组 */
	pieceMove MoveList[10][144];
private:
	/* 记录每一层所能走的步数 */
	int moveCount;
	/* 产生走法 */
	int addMove(int x_coordinate, int y_coordinate, int layer);
};

class historyHeuristic {
public:
	historyHeuristic();
	~historyHeuristic();
	/* 重置历史得分表 */
	void resetHistoryTable();
	/* 获取历史得分 */
	int getHistoryScore(pieceMove move);
	/* 将指定走法加入历史记录 */
	void insertHistoryScore(pieceMove move, int depth);
	/* 将所得数据排序 */
	static void Q_Sort(pieceMove *source, int low, int high);
private:
	/* 快排辅助函数 */
	static int Q_Sort_partition(pieceMove *source, int low, int high);
	/* 当前棋盘的历史得分 */
	int historyTable[GRID_NUM][GRID_NUM];
};

class transpositionTable
{
public:
	transpositionTable();
	virtual ~transpositionTable();
	/* 计算当前棋盘哈希值 */
	void HASH_calculateInitKey(int current_position[][GRID_NUM]);
	/* 哈希值增量 */
	void HASH_makeMove(pieceMove *move, int current_position[][GRID_NUM]);
	/* 撤销所给的走法的哈希值，还原成走过之前的 */
	void HASH_unmakeMove(pieceMove *move, int current_position[][GRID_NUM]);
	/* 查询哈希表中当前节点数据 */
	int HASH_searchNode(int alpha, int beta, int depth, int table);
	/* 将当前节点的值存入哈希表 */
	void HASH_insertNode(ENTRY_TYPE entry_type, short evaluation, short depth, int table);
	/* 初始化Hash表 */
	void HASH_initTable();
	/* 生成32位哈希值 */
	unsigned int m_nHashKey32[2][12][12];
	/* 生成64位哈希值 */
	unsigned long long m_nHashKey64[2][12][12];

	long long rand64();
	long rand32();
	/* 置换表的头指针 */
	hashItem *m_pTT[2];
	/* 32位哈希值 */
	unsigned int m_HashKey32;
	/* 64位哈希值 */
	unsigned long long m_HashKey64;
};

class negaScouter
{
public:
	negaScouter();
	~negaScouter();
	/* 查找好的下子位置 */
	int searchNiceMove(int position[][GRID_NUM], int type, int &x_best, int &y_best);
	/* 确定最大搜索深度 */
	void setSearchDepth(int depth);
	/* 判断游戏是否结束 */
	int isGameOver(int position[][GRID_NUM], int type);
	/* 设置走法产生器 */
	void setMoveGenerator(moveGenerator *move_generator);
	/* 设置历史启发类 */
	void setHistoryHeuristic(historyHeuristic *history_heuristic);
	/* 设置评估函数 */
	void setEvaluation(evaluation *evaluation);
private:
	/* 根据某一走法产生之后的棋盘 */
	void makeBoard(pieceMove *move, int type);
	/* 恢复之前的棋盘 */
	void unmakeBoard(pieceMove *move);
	/* 搜索当前最佳路径 */
	int negaScout(int depth, int alpha, int beta);
	/* 保存当前的棋盘状态 */
	int currentBoard[GRID_NUM][GRID_NUM];
	/* 记录最佳走法的变量 */
	pieceMove bestMove;
	/* 走法产生器指针 */
	moveGenerator *pointer_moveGenerator;
	/* 估值核心指针 */
	evaluation *pointer_evaluation;
	/* 历史启发表指针 */ 
	historyHeuristic *pointer_historyHeuristic;
	/* 最大搜索深度 */ 
	int maxDepth;
	/* 当前搜索深度 */
	int currentDepth;
	/* 历史置换表 */
	historyHeuristic history_heuristic;
	transpositionTable transposition_table;
};


