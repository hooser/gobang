#include"header_renju.h"
#include<stdlib.h>

transpositionTable::transpositionTable()
{
	HASH_initTable();
}

transpositionTable::~transpositionTable()
{
	delete m_pTT[0];
	delete m_pTT[1];
}

/* ���һ��64λ�Ĺ�ϣֵ */
long long transpositionTable::rand64()
{
	return rand() ^ ((long long)rand() << 15) ^ ((long long)rand() << 30) ^ ((long long)rand() << 45) ^ ((long long)rand() << 60);
}

/* ���һ��32λ�Ĺ�ϣֵ */
long transpositionTable::rand32()
{
	return rand() ^ ((long)rand() << 15) ^ ((long)rand() << 30);
}

/* ���ɹ�ϣֵ���� */
void transpositionTable::HASH_initTable()
{
	/* ������������� */
	srand((unsigned)time_t(NULL));
	/* ���������� */
	for (int k = 0; k < 2; k++)
		for (int i = 0; i < GRID_NUM; i++)
			for (int j = 0; j < GRID_NUM; j++)
			{
				m_nHashKey64[k][i][j] = rand64();
				m_nHashKey32[k][i][j] = rand32();
			}
	/* �����û���ռ� */
	m_pTT[0] = new hashItem[1024 * 1024];
	m_pTT[1] = new hashItem[1024 * 1024];
}

/* �����ϣֵ */
void transpositionTable::HASH_calculateInitKey(int current_position[][GRID_NUM])
{
	int piecetype;
	m_HashKey32 = 0;
	m_HashKey64 = 0;
	/* �����ۼӷ���һ��ϣֵ */
	for (int i = 0; i<GRID_NUM; i++)
		for (int j = 0; j < GRID_NUM; j++)
		{
			piecetype = current_position[i][j];
			if (piecetype != NO_PIECE)
			{
				m_HashKey32 = m_HashKey32^m_nHashKey32[piecetype][i][j];
				m_HashKey64 = m_HashKey64^m_nHashKey64[piecetype][i][j];
			}
		}
}

/* �޸Ĺ�ϣֵ */
void transpositionTable::HASH_makeMove(pieceMove *move, int current_position[][GRID_NUM])
{
	int type;
	type = current_position[move->piecePos.y_coordinate][move->piecePos.x_coordinate];
	m_HashKey32 = m_HashKey32^m_nHashKey32[type][move->piecePos.y_coordinate][move->piecePos.x_coordinate];
	m_HashKey64 = m_HashKey64^m_nHashKey64[type][move->piecePos.y_coordinate][move->piecePos.x_coordinate];
}

/* �ָ���ϣֵ */
void transpositionTable::HASH_unmakeMove(pieceMove *move, int current_position[][GRID_NUM])
{
	int type;
	type = current_position[move->piecePos.y_coordinate][move->piecePos.x_coordinate];
	m_HashKey32 = m_HashKey32^m_nHashKey32[type][move->piecePos.y_coordinate][move->piecePos.x_coordinate];
	m_HashKey64 = m_HashKey64^m_nHashKey64[type][move->piecePos.y_coordinate][move->piecePos.x_coordinate];
}

/* ��ѯ��ϣ�� */
int transpositionTable::HASH_searchNode(int alpha, int beta, int depth, int table)
{
	int x;
	hashItem *pointer_hashItem;
	x = m_HashKey32 & 0xFFFFF;
	pointer_hashItem = &m_pTT[table][x];
	if (pointer_hashItem->depth >= depth && pointer_hashItem->check_sum == m_HashKey64)
	{
		switch (pointer_hashItem->entry_type)
		{
		case exact_value:
			return pointer_hashItem->evaluation;
		case lower_bound:
			if (pointer_hashItem->evaluation >= beta)
				return pointer_hashItem->evaluation;
			else
				break;
		case upper_bound:
			if (pointer_hashItem->evaluation <= alpha)
				return pointer_hashItem->evaluation;
			else
				break;
		}
	}
	return 233333;
}

/* ���뵱ǰֵ */
void transpositionTable::HASH_insertNode(ENTRY_TYPE entry_type, short evaluation, short depth, int table)
{
	int address;
	hashItem *pointer_hashItem;
	/* ��ȡ��ϣ��ַ */
	address = m_HashKey32 & 0xFFFFF;
	/* ȡ��ָ��λ�� */
	pointer_hashItem = &m_pTT[table][address];

	/* ����洢 */
	pointer_hashItem->check_sum = m_HashKey64;
	pointer_hashItem->entry_type = entry_type;
	pointer_hashItem->evaluation = evaluation;
	pointer_hashItem->depth = depth;
}