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

/* ��ϣ���д����������� */
enum ENTRY_TYPE
{
	exact_value, lower_bound, upper_bound
};

/* 12x12��λ���̴�������״̬ */
/* (2,1)�ǵ����еڶ��� */
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

/* ��ʾ����λ�� */
typedef struct piecePosition
{
	int x_coordinate;
	int y_coordinate;
	int color = NO_PIECE;
}PIECEPOS;

/* ���������߷� */
typedef struct pieceMove
{
	PIECEPOS piecePos;  //����λ��
	int score;  //�߷��÷�
}PIECEMOVE;

/* ������ϣ�� */
typedef struct hashItem
{
	/* У���루64λ�� */
	long long check_sum;
	/* ��Ӧ�洢�������� */
	ENTRY_TYPE entry_type;
	/* ��Ӧ�ݹ��� */
	short depth;
	/* ��Ӧ�ڵ��ֵ */
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
	/* �����߷����� */
	int createMove(int position[GRID_NUM][GRID_NUM], int layer, int player);
	/* �����߷������� */
	pieceMove MoveList[10][144];
private:
	/* ��¼ÿһ�������ߵĲ��� */
	int moveCount;
	/* �����߷� */
	int addMove(int x_coordinate, int y_coordinate, int layer);
};

class historyHeuristic {
public:
	historyHeuristic();
	~historyHeuristic();
	/* ������ʷ�÷ֱ� */
	void resetHistoryTable();
	/* ��ȡ��ʷ�÷� */
	int getHistoryScore(pieceMove move);
	/* ��ָ���߷�������ʷ��¼ */
	void insertHistoryScore(pieceMove move, int depth);
	/* �������������� */
	static void Q_Sort(pieceMove *source, int low, int high);
private:
	/* ���Ÿ������� */
	static int Q_Sort_partition(pieceMove *source, int low, int high);
	/* ��ǰ���̵���ʷ�÷� */
	int historyTable[GRID_NUM][GRID_NUM];
};

class transpositionTable
{
public:
	transpositionTable();
	virtual ~transpositionTable();
	/* ���㵱ǰ���̹�ϣֵ */
	void HASH_calculateInitKey(int current_position[][GRID_NUM]);
	/* ��ϣֵ���� */
	void HASH_makeMove(pieceMove *move, int current_position[][GRID_NUM]);
	/* �����������߷��Ĺ�ϣֵ����ԭ���߹�֮ǰ�� */
	void HASH_unmakeMove(pieceMove *move, int current_position[][GRID_NUM]);
	/* ��ѯ��ϣ���е�ǰ�ڵ����� */
	int HASH_searchNode(int alpha, int beta, int depth, int table);
	/* ����ǰ�ڵ��ֵ�����ϣ�� */
	void HASH_insertNode(ENTRY_TYPE entry_type, short evaluation, short depth, int table);
	/* ��ʼ��Hash�� */
	void HASH_initTable();
	/* ����32λ��ϣֵ */
	unsigned int m_nHashKey32[2][12][12];
	/* ����64λ��ϣֵ */
	unsigned long long m_nHashKey64[2][12][12];

	long long rand64();
	long rand32();
	/* �û����ͷָ�� */
	hashItem *m_pTT[2];
	/* 32λ��ϣֵ */
	unsigned int m_HashKey32;
	/* 64λ��ϣֵ */
	unsigned long long m_HashKey64;
};

class negaScouter
{
public:
	negaScouter();
	~negaScouter();
	/* ���Һõ�����λ�� */
	int searchNiceMove(int position[][GRID_NUM], int type, int &x_best, int &y_best);
	/* ȷ������������ */
	void setSearchDepth(int depth);
	/* �ж���Ϸ�Ƿ���� */
	int isGameOver(int position[][GRID_NUM], int type);
	/* �����߷������� */
	void setMoveGenerator(moveGenerator *move_generator);
	/* ������ʷ������ */
	void setHistoryHeuristic(historyHeuristic *history_heuristic);
	/* ������������ */
	void setEvaluation(evaluation *evaluation);
private:
	/* ����ĳһ�߷�����֮������� */
	void makeBoard(pieceMove *move, int type);
	/* �ָ�֮ǰ������ */
	void unmakeBoard(pieceMove *move);
	/* ������ǰ���·�� */
	int negaScout(int depth, int alpha, int beta);
	/* ���浱ǰ������״̬ */
	int currentBoard[GRID_NUM][GRID_NUM];
	/* ��¼����߷��ı��� */
	pieceMove bestMove;
	/* �߷�������ָ�� */
	moveGenerator *pointer_moveGenerator;
	/* ��ֵ����ָ�� */
	evaluation *pointer_evaluation;
	/* ��ʷ������ָ�� */ 
	historyHeuristic *pointer_historyHeuristic;
	/* ���������� */ 
	int maxDepth;
	/* ��ǰ������� */
	int currentDepth;
	/* ��ʷ�û��� */
	historyHeuristic history_heuristic;
	transpositionTable transposition_table;
};


