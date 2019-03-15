#include "header_renju.h"
#include <string>
//#include <iostream>

using namespace std;

int position_1;                //威胁出现在行的位置
int position_2;
int position_3;
int flag_ = 0;

#define FIVE     0
#define FOUR     1
#define S_FOUR   2 
#define THREE    3
#define S_THREE  4
#define TWO      5
#define S_TWO    6
#define NONE     7


#define ANALYSED 0
#define UNANALYSED  10


#define WIN  1
#define LOSE -1
#define TIE  0

ans suggestpos;

/*
威胁空间：
四连
冲四：x0000_, _0000x         （1）
三连
冲四：000_0, 0_000           （1）
活三：_000_, x_000_x         （2）
二连
冲四：x00_00, 00_00x         （1）
跳活三：_00_0_, _0_00_       （3）
“_“代表的点为威胁点，即：当对手下在这些点时，我方必输
括号内为处理的对策数目，威胁空间返回一个坐标值
对于一种对策的情况，该坐标为阻止对手获胜的唯一解；
对于多种对策的情况，该坐标为阻止对手获胜的最优解
*/

/*
行扫描函数
输入:  player 棋子类型
line   一组棋盘状态
内部操作 ： 扫描状态行 ，得到棋牌局势（冲四，活四...）
无输出
*/
void threatSearch(char player, string line)
{
	char enemy = (player == BLACK) ? WHITE : BLACK;

	int len = line.size();

	int nodeState[GRID_NUM];  //
	for (int i = 0; i < GRID_NUM; i++)
		nodeState[i] = UNANALYSED;
	int leftEdge, rightEdge;  //标志一下搜索的过程中连续的左右边界
							  //memset(nodeState, UNANALYSED, sizeof(nodeState));  //初始化为未被分析过


	for (int i = 0; i < len; i++)
	{
		if (line[i] == enemy&&nodeState[i] == UNANALYSED)
		{

			leftEdge = i;
			rightEdge = i;
			while (leftEdge >= 0)
			{
				if (line[leftEdge] != enemy)
					break;
				leftEdge--;
			}
			while (rightEdge < len)
			{
				if (line[rightEdge] != enemy)
					break;
				rightEdge++;
			}
			++leftEdge;
			rightEdge--;
			for (int k = leftEdge; k <= rightEdge; k++)
			{
				nodeState[k] = ANALYSED;
			}

			if (leftEdge == rightEdge)
				nodeState[leftEdge] = UNANALYSED;

			//四连
			else if (rightEdge - leftEdge == 3)
			{
				//冲四   _0000X  X0000_
				if (((leftEdge == 0 ||
					(leftEdge > 0 && line[leftEdge - 1] == player)) &&
					(rightEdge < len - 1 &&
						line[rightEdge + 1] == NO_PIECE)) ||
					((rightEdge == len - 1 ||
						(rightEdge < len - 1 &&
							line[rightEdge + 1] == player)) &&
						(leftEdge > 0 && line[leftEdge - 1] == NO_PIECE)))
				{

					if (leftEdge == 0)
						position_1 = rightEdge + 1;
					else if ((leftEdge > 0 && line[leftEdge - 1] == player) &&
						(rightEdge < len - 1 &&
							line[rightEdge + 1] == NO_PIECE))
						position_1 = rightEdge + 1;
					else
						position_1 = leftEdge - 1;

				}

			}


			//三连
			else if (rightEdge - leftEdge == 2)
			{
				//冲四
				//000_0   0_000
				if ((leftEdge > 1 && line[leftEdge - 2] == enemy&&
					nodeState[leftEdge - 2] == UNANALYSED &&
					line[leftEdge - 1] == NO_PIECE) ||
					(rightEdge < len - 2 && line[rightEdge + 1] == NO_PIECE &&
						line[rightEdge + 2] == enemy &&
						nodeState[rightEdge + 2] == UNANALYSED))
				{
					if ((leftEdge > 1 && line[leftEdge - 2] == enemy &&
						nodeState[leftEdge - 2] == UNANALYSED &&
						line[leftEdge - 1] == NO_PIECE))
					{
						position_1 = leftEdge - 1;
					}
					else
						position_1 = rightEdge + 1;
				}

				//活三  __000__  _000__
				else if (leftEdge > 0 && rightEdge < len - 1 &&
					line[leftEdge - 1] == NO_PIECE && line[rightEdge + 1] == NO_PIECE &&
					((leftEdge > 1 && line[leftEdge - 2] == NO_PIECE) ||
						(rightEdge < len - 2 &&
							line[rightEdge + 2] == NO_PIECE)))
				{
					position_1 = leftEdge - 1;
					position_2 = rightEdge + 1;
				}
				else
				{
					//x_000_x
					if (leftEdge > 0 && line[leftEdge - 1] == NO_PIECE &&
						rightEdge < len - 1 && line[rightEdge + 1] == NO_PIECE &&
						(leftEdge == 1 ||
							(leftEdge > 1 && line[leftEdge - 2] == player)) &&
						(rightEdge == len - 2 ||
							(rightEdge < len - 2 &&
								line[rightEdge + 2] == player))) {
						position_1 = leftEdge - 1;
						position_2 = rightEdge + 1;
					}

				}




			}

			//二连
			else if (rightEdge - leftEdge == 1)
			{
				//冲四x00_00  00_00x
				if (
					(
						rightEdge < len - 3 &&
						line[rightEdge + 1] == NO_PIECE &&
						line[rightEdge + 2] == enemy &&
						nodeState[rightEdge + 2] == UNANALYSED &&
						line[rightEdge + 3] == enemy &&
						nodeState[rightEdge + 3] == UNANALYSED)
					||
					(
						leftEdge > 2 && line[leftEdge - 1] == NO_PIECE &&
						line[leftEdge - 2] == enemy &&
						nodeState[leftEdge - 2] == UNANALYSED &&
						line[leftEdge - 3] == enemy &&
						nodeState[leftEdge - 3] == UNANALYSED))
				{
					if (
						rightEdge < len - 3 &&
						line[rightEdge + 1] == NO_PIECE &&
						line[rightEdge + 2] == enemy &&
						nodeState[rightEdge + 2] == UNANALYSED &&
						line[rightEdge + 3] == enemy &&
						nodeState[rightEdge + 3] == UNANALYSED)
					{
						position_1 = rightEdge + 1;
					}
					else
						position_1 = leftEdge - 1;
				}
				//  跳活三  _00_0_   _0_00_
				else if ((leftEdge > 0 && line[leftEdge - 1] == NO_PIECE &&
					line[rightEdge + 1] == NO_PIECE &&
					rightEdge < len - 3 && line[rightEdge + 2] == enemy &&
					nodeState[rightEdge + 2] == UNANALYSED &&
					line[rightEdge + 3] == NO_PIECE) ||
					(leftEdge > 2 && line[leftEdge - 2] == enemy &&
						nodeState[leftEdge - 2] == UNANALYSED &&
						line[leftEdge - 3] == NO_PIECE&&rightEdge<len - 1 && line[rightEdge + 1] == NO_PIECE))
				{

					if (line[rightEdge + 2] == enemy)

					{
						position_1 = leftEdge - 1;
						position_2 = rightEdge + 1;
						position_3 = rightEdge + 3;
					}
					else
					{
						position_1 = leftEdge - 3;
						position_2 = leftEdge - 1;
						position_3 = rightEdge + 1;
					}
				}


			}

		}//if (line[i] == player&&nodeState[i] == UNANALYSED)

	}//for (int i = 0;i < len;i++)
}

/*
返回威胁点坐标，
如果没有威胁点，返回（-1，-1）
否则，返回威胁点的坐标
坐标是指在currentThisBoard[][]的行列值[0-11]
*/
ans consider(char player, int(*currentThisBoard)[GRID_NUM])
{
	bool judge1(int, int, char, int(*)[GRID_NUM]);
	bool judge2(int, int, char, int(*)[GRID_NUM]);
	bool judge3(int, int, char, int(*)[GRID_NUM]);
	bool tjudge1(int, int, char, int(*)[GRID_NUM]);
	bool tjudge2(int, int, char, int(*)[GRID_NUM]);
	bool tjudge3(int, int, char, int(*)[GRID_NUM]);
	suggestpos.x = -1;
	suggestpos.y = -1;
	position_1 = -1;
	position_2 = -1;
	position_3 = -1;
	for (int i = 0; i < GRID_NUM; ++i) {
		string src_row = "";
		for (int j = 0; j <GRID_NUM; ++j)
		{
			src_row += currentThisBoard[i][j];
		} //for(int j=0;j<GRID_NUM；++j)
		threatSearch(player, src_row);
		if (position_1 != -1)
		{
			if (position_2 != -1)
			{
				if (position_3 != -1)                    //1,2,3
				{

					if (tjudge1(i, position_1, player, currentThisBoard) == true)
					{
						suggestpos.x = i;
						suggestpos.y = position_1;
					}
					else if (tjudge1(i, position_3, player, currentThisBoard) == true)
					{
						suggestpos.x = i;
						suggestpos.y = position_3;
					}
					else
					{
						suggestpos.x = i;
						suggestpos.y = position_2;
					}
				}//if(position_3!=-1) 
				else                                //1,2
				{


					if (judge1(i, position_1, player, currentThisBoard) == true)
					{
						suggestpos.x = i;
						suggestpos.y = position_1;
					}
					else
					{
						suggestpos.x = i;
						suggestpos.y = position_2;
					}
				}
			}//if(position_2!=-1)
			else                                    //1
			{


				suggestpos.x = i;
				suggestpos.y = position_1;
			}

		}//if(position_1!=-1)
		if (suggestpos.x != -1 && suggestpos.y != -1)
		{

			return suggestpos;
		}

	}//for(int i=0; i<GRID_NUM;i++)

	for (int i = 0; i < GRID_NUM; ++i) {
		string src_col = "";
		for (int j = 0; j <GRID_NUM; ++j)
		{

			src_col += currentThisBoard[j][i];
		} //for(int j=0;j<GRID_NUM；++j)

		threatSearch(player, src_col);
		if (position_1 != -1)
		{
			if (position_2 != -1)
			{
				if (position_3 != -1)                    //1,2,3
				{

					if (tjudge2(position_1, i, player, currentThisBoard) == true)
					{
						suggestpos.x = position_1;
						suggestpos.y = i;
					}
					else if (tjudge2(position_3, i, player, currentThisBoard) == true)
					{
						suggestpos.x = position_3;
						suggestpos.y = i;
					}
					else
					{
						suggestpos.x = position_2;
						suggestpos.y = i;
					}
				}//if(position_3!=-1) 
				else                                //1,2
				{

					if (judge2(position_1, i, player, currentThisBoard) == true)
					{
						suggestpos.x = position_1;
						suggestpos.y = i;
					}
					else
					{
						suggestpos.x = position_2;
						suggestpos.y = i;
					}

				}
			}//if(position_2!=-1)
			else                                    //1
			{

				suggestpos.x = position_1;
				suggestpos.y = i;
			}
		}//if(position_1!=-1)
		if (suggestpos.x != -1 && suggestpos.y != -1)
		{

			return suggestpos;
		}
	}//for(int i=0; i<GRID_NUM;i++)
	 //左下-右上搜索
	for (int sum = 4; sum <= 19; ++sum) {
		string src = "";
		for (int i = (sum <= 11) ? sum : 11, j = sum - i; i >= 0 && j <= 11;
		--i, ++j) {
			src += currentThisBoard[i][j];
		}
		threatSearch(player, src);
		if (position_1 != -1)
		{
			if (position_2 != -1)
			{
				if (position_3 != -1)                    //1,2,3
				{
					if (sum <= 11)
					{
						if (tjudge3(sum - position_1, position_1, player, currentThisBoard) == true)
						{
							suggestpos.x = sum - position_1;
							suggestpos.y = position_1;
						}
						else if (tjudge3(sum - position_3, position_3, player, currentThisBoard) == true)
						{
							suggestpos.x = sum - position_3;
							suggestpos.y = position_3;
						}
						else
						{
							suggestpos.x = sum - position_2;
							suggestpos.y = position_2;
						}
					} //if(sum<=11)
					else    //sum>11
					{
						if (tjudge3(11 - position_1, sum - 11 + position_1, player, currentThisBoard) == true)
						{
							suggestpos.x = 11 - position_1;
							suggestpos.y = sum - 11 + position_1;
						}
						else if (tjudge3(11 - position_3, sum - 11 + position_3, player, currentThisBoard) == true)
						{
							suggestpos.x = 11 - position_3;
							suggestpos.y = sum - 11 + position_3;
						}
						else
						{
							suggestpos.x = 11 - position_2;
							suggestpos.y = sum - 11 + position_2;
						}
					} //else
				}//if(position_3!=-1) 
				else                                //(position_3==-1)1,2
				{
					if (sum <= 11)
					{
						if (judge3(sum - position_1, position_1, player, currentThisBoard) == true)
						{
							suggestpos.x = sum - position_1;
							suggestpos.y = position_1;
						}
						else
						{
							suggestpos.x = sum - position_2;
							suggestpos.y = position_2;
						}
					} //if(sum<=11)
					else       //sum>11
					{
						if (judge3(11 - position_1, sum - 11 + position_1, player, currentThisBoard) == true)
						{
							suggestpos.x = 11 - position_1;
							suggestpos.y = sum - 11 + position_1;
						}
						else
						{
							suggestpos.x = 11 - position_2;
							suggestpos.y = sum - 11 + position_2;
						}
					}//else(sum>11)
				}
			}//if(position_2!=-1)
			else                                    //1
			{
				if (sum <= 11)
				{
					suggestpos.x = sum - position_1;
					suggestpos.y = position_1;
				}//if(sum<11)
				else
				{
					suggestpos.x = 11 - position_1;
					suggestpos.y = sum - 11 + position_1;
				} //else(sum>11)
			}
		}//if(position_1!=-1)
		if (suggestpos.x != -1 && suggestpos.y != -1)
			return suggestpos;
	}
	//左上-右下搜索
	for (int sub = 7; sub >= -7; --sub) {
		string src = "";
		for (int i = (sub >= 0) ? sub : 0, j = i - sub; i <= 11 && j <= 11;
		++i, ++j) {
			src += currentThisBoard[i][j];
		}
		threatSearch(player, src);
		if (position_1 != -1)
		{
			if (position_2 != -1)
			{
				if (position_3 != -1)                    //1,2,3
				{
					if (sub >= 0)
					{
						if (tjudge3(sub + position_1, position_1, player, currentThisBoard) == true)
						{
							suggestpos.x = sub + -position_1;
							suggestpos.y = position_1;
						}
						else if (tjudge3(sub + position_3, position_3, player, currentThisBoard) == true)
						{
							suggestpos.x = sub + position_3;
							suggestpos.y = position_3;
						}
						else
						{
							suggestpos.x = sub + position_2;
							suggestpos.y = position_2;
						}
					} //if(sum<=11)
					else    //sub<0
					{
						if (tjudge3(position_1, position_1 - sub, player, currentThisBoard) == true)
						{
							suggestpos.x = position_1;
							suggestpos.y = position_1 - sub;
						}
						else if (tjudge3(position_3, position_3 - sub, player, currentThisBoard) == true)
						{
							suggestpos.x = position_3;
							suggestpos.y = position_3 - sub;
						}
						else
						{
							suggestpos.x = position_2;
							suggestpos.y = position_2 - sub;
						}
					} //else
				}//if(position_3!=-1) 
				else                                //(position_3==-1)1,2
				{
					if (sub >= 0)
					{
						if (judge3(sub + position_1, position_1, player, currentThisBoard) == true)
						{
							suggestpos.x = sub + position_1;
							suggestpos.y = position_1;
						}
						else
						{
							suggestpos.x = sub + position_2;
							suggestpos.y = position_2;
						}
					} //if(sum<=11)
					else       //sum>11
					{
						if (judge3(position_1, position_1 - sub, player, currentThisBoard) == true)
						{
							suggestpos.x = position_1;
							suggestpos.y = position_1 - sub;
						}
						else
						{
							suggestpos.x = position_2;
							suggestpos.y = position_2 - sub;
						}
					}//else(sum>11)
				}
			}//if(position_2!=-1)
			else                                    //1
			{
				if (sub >= 0)
				{
					suggestpos.x = sub + position_1;
					suggestpos.y = position_1;
				}//if(sum<11)
				else
				{
					suggestpos.x = position_1;
					suggestpos.y = position_1 - sub;
				} //else(sum>11)
			}
		}//if(position_1!=-1)
		if (suggestpos.x != -1 && suggestpos.y != -1)
			return suggestpos;
	}//for (int sub = 7; sub >= -7; --sub)
	return suggestpos;
}

//判断行情况
//输入要判断的点的坐标
//如果当前点满足，输出true；否则，输出false
bool judge1(int i, int j, char player, int(*currentThisBoard)[GRID_NUM])
{
	char enemy = (player == BLACK) ? WHITE : BLACK;
	if (i == 0)
	{
		if (currentThisBoard[i + 1][j] == enemy && currentThisBoard[i + 2][j] == enemy && currentThisBoard[i + 3][j] == enemy)        //竖直方向

			return true;
		if (j >= 4)                                                                                 //左下右上
		{
			if (currentThisBoard[i + 1][j - 1] == enemy && currentThisBoard[i + 2][j - 2] == enemy && currentThisBoard[i + 3][j - 3] == enemy)
				return true;
		}
		if (j <= 7)                                                                                //左上右下
		{
			if (currentThisBoard[i + 1][j + 1] == enemy && currentThisBoard[i + 2][j + 2] == enemy && currentThisBoard[i + 3][j + 3] == enemy)
				return true;
		}
		return false;
	} //if(i==0)
	else if (i == 1)
	{
		if ((currentThisBoard[i - 1][j] == enemy &&currentThisBoard[i + 1][j] == enemy&&currentThisBoard[i + 2][j] == enemy) ||               //竖直
			(currentThisBoard[i + 1][j] == enemy && currentThisBoard[i + 2][j] == enemy && currentThisBoard[i + 3][j] == enemy) ||
			(currentThisBoard[i + 1][j] == enemy && currentThisBoard[i + 2][j] == enemy))
			return true;
		else
			return false;
	} //else if(i==1)
	else if (i == 2)
	{

		if ((currentThisBoard[i - 1][j] == enemy &&currentThisBoard[i + 1][j] == enemy&&currentThisBoard[i + 2][j] == enemy) ||
			(currentThisBoard[i + 1][j] == enemy && currentThisBoard[i + 2][j] == enemy && currentThisBoard[i + 3][j] == enemy) ||
			(currentThisBoard[i - 1][j] == enemy &&currentThisBoard[i - 2][j] == enemy&&currentThisBoard[i + 2][j] == enemy) ||
			(currentThisBoard[i + 1][j] == enemy && currentThisBoard[i + 2][j] == enemy) ||
			(currentThisBoard[1][j] == enemy && currentThisBoard[3][j] == enemy))            //竖直
			return true;
		else
			return false;
	}// else if(i==2)
	else if (i == 9)
	{
		if ((currentThisBoard[6][j] == enemy && currentThisBoard[7][j] == enemy && currentThisBoard[8][j] == enemy) ||
			(currentThisBoard[7][j] == enemy && currentThisBoard[8][j] == enemy && currentThisBoard[10][j] == enemy) ||
			(currentThisBoard[8][j] == enemy && currentThisBoard[10][j] == enemy && currentThisBoard[11][j] == enemy) ||
			(currentThisBoard[7][j] == enemy && currentThisBoard[8][j] == enemy) ||
			(currentThisBoard[8][j] == enemy && currentThisBoard[10][j] == enemy))
			return true;
		else
			return false;
	}
	else if (i == 10)
	{
		if ((currentThisBoard[9][j] == enemy && currentThisBoard[7][j] == enemy && currentThisBoard[8][j] == enemy) ||
			(currentThisBoard[9][j] == enemy && currentThisBoard[8][j] == enemy && currentThisBoard[11][j] == enemy) ||
			(currentThisBoard[9][j] == enemy && currentThisBoard[8][j] == enemy))
			return true;
		else
			return false;
	}
	else if (i == 11)
	{
		if ((currentThisBoard[8][j] == enemy && currentThisBoard[9][j] == enemy && currentThisBoard[10][j] == enemy))
			return true;
		else
			return false;
	}
	else
	{
		if ((currentThisBoard[i - 3][j] == enemy && currentThisBoard[i - 2][j] == enemy && currentThisBoard[i - 1][j] == enemy) ||
			(currentThisBoard[i - 2][j] == enemy && currentThisBoard[i - 1][j] == enemy && currentThisBoard[i + 1][j] == enemy) ||
			(currentThisBoard[i - 1][j] == enemy && currentThisBoard[i + 1][j] == enemy && currentThisBoard[i + 2][j] == enemy) ||
			(currentThisBoard[i + 1][j] == enemy && currentThisBoard[i + 2][j] == enemy && currentThisBoard[i + 3][j] == enemy) ||
			(currentThisBoard[i + 1][j] == enemy && currentThisBoard[i + 2][j] == enemy) ||
			(currentThisBoard[i - 2][j] == enemy && currentThisBoard[i - 1][j] == enemy) ||
			(currentThisBoard[i - 1][j] == enemy && currentThisBoard[i + 1][j] == enemy))
			return true;
		else
			return false;
	}
}

//判断列情况
bool judge2(int i, int j, char player, int(*currentThisBoard)[GRID_NUM])
{
	char enemy = (player == BLACK) ? WHITE : BLACK;
	if (j == 0)
	{
		if (currentThisBoard[i][1] == enemy && currentThisBoard[i][2] == enemy && currentThisBoard[i][3] == enemy)        //竖直方向
			return true;
		else
			return false;
	} //if(i==0)
	else if (j == 1)
	{
		if ((currentThisBoard[i][0] == enemy &&currentThisBoard[i][2] == enemy&&currentThisBoard[i][3] == enemy) ||               //竖直
			(currentThisBoard[i][2] == enemy && currentThisBoard[i][3] == enemy && currentThisBoard[i][4] == enemy) ||
			(currentThisBoard[i][2] == enemy && currentThisBoard[i][3] == enemy))
		{
			return true;
		}
		else
		{
			return false;
		}
	}//else if(j==1)
	else if (j == 2)
	{

		if ((currentThisBoard[i][0] == enemy &&currentThisBoard[i][1] == enemy&&currentThisBoard[i][3] == enemy) ||
			(currentThisBoard[i][1] == enemy && currentThisBoard[i][3] == enemy && currentThisBoard[i][4] == enemy) ||
			(currentThisBoard[i][3] == enemy &&currentThisBoard[i][4] == enemy&&currentThisBoard[i][5] == enemy) ||
			(currentThisBoard[i][3] == enemy &&currentThisBoard[i][4] == enemy) ||
			(currentThisBoard[i][3] == enemy &&currentThisBoard[i][1] == enemy))            //竖直
			return true;
		else
			return false;
	}// else if(i==2)
	else if (j == 9)
	{
		if ((currentThisBoard[i][8] == enemy && currentThisBoard[i][10] == enemy && currentThisBoard[i][11] == enemy) ||
			(currentThisBoard[i][7] == enemy && currentThisBoard[i][8] == enemy && currentThisBoard[i][10] == enemy) ||
			(currentThisBoard[i][6] == enemy && currentThisBoard[i][7] == enemy && currentThisBoard[i][8] == enemy) ||
			(currentThisBoard[i][8] == enemy && currentThisBoard[i][10] == enemy) ||
			(currentThisBoard[i][7] == enemy && currentThisBoard[i][8] == enemy))
			return true;
		else
			return false;
	}
	else if (j == 10)
	{
		if ((currentThisBoard[i][8] == enemy && currentThisBoard[i][9] == enemy && currentThisBoard[i][11] == enemy) ||
			(currentThisBoard[i][7] == enemy && currentThisBoard[i][8] == enemy && currentThisBoard[i][9] == enemy) ||
			(currentThisBoard[i][8] == enemy && currentThisBoard[i][9] == enemy))
			return true;
		else
			return false;
	}
	else if (j == 11)
	{
		if ((currentThisBoard[i][9] == enemy && currentThisBoard[i][10] == enemy && currentThisBoard[i][8] == enemy))
			return true;
		else
			return false;
	}
	else
	{
		if ((currentThisBoard[i][j - 3] == enemy && currentThisBoard[i][j - 2] == enemy && currentThisBoard[i][j - 1] == enemy) ||
			(currentThisBoard[i][j - 2] == enemy && currentThisBoard[i][j - 1] == enemy && currentThisBoard[i][j + 1] == enemy) ||
			(currentThisBoard[i][j - 1] == enemy && currentThisBoard[i][j + 1] == enemy && currentThisBoard[i][j + 2] == enemy) ||
			(currentThisBoard[i][j + 1] == enemy && currentThisBoard[i][j + 2] == enemy && currentThisBoard[i][j + 3] == enemy) ||
			(currentThisBoard[i][j - 2] == enemy && currentThisBoard[i][j - 1] == enemy) ||
			(currentThisBoard[i][j - 1] == enemy && currentThisBoard[i][j + 1] == enemy) ||
			(currentThisBoard[i][j + 1] == enemy && currentThisBoard[i][j + 2] == enemy))
			return true;
		else
			return false;
	}

}
bool judge3(int i, int j, char player, int(*currentThisBoard)[GRID_NUM])
{
	bool judge1(int, int, char, int(*)[GRID_NUM]);
	bool judge2(int, int, char, int(*)[GRID_NUM]);
	char enemy = (player == BLACK) ? WHITE : BLACK;
	if (judge1(i, j, player, currentThisBoard) == true || judge2(i, j, player, currentThisBoard) == true)
		return true;
	else
		return false;
}
bool tjudge1(int i, int j, char player, int(*currentThisBoard)[GRID_NUM])
{
	char enemy = (player == BLACK) ? WHITE : BLACK;
	if (i == 0)
	{
		if (currentThisBoard[i + 1][j] == enemy && currentThisBoard[i + 2][j] == enemy && currentThisBoard[i + 3][j] == enemy)        //竖直方向

			return true;
		if (j >= 4)                                                                                 //左下右上
		{
			if (currentThisBoard[i + 1][j - 1] == enemy && currentThisBoard[i + 2][j - 2] == enemy && currentThisBoard[i + 3][j - 3] == enemy)
				return true;
		}
		if (j <= 7)                                                                                //左上右下
		{
			if (currentThisBoard[i + 1][j + 1] == enemy && currentThisBoard[i + 2][j + 2] == enemy && currentThisBoard[i + 3][j + 3] == enemy)
				return true;
		}
		return false;
	} //if(i==0)
	else if (i == 1)
	{
		if ((currentThisBoard[i - 1][j] == enemy &&currentThisBoard[i + 1][j] == enemy&&currentThisBoard[i + 2][j] == enemy) ||               //竖直
			(currentThisBoard[i + 1][j] == enemy && currentThisBoard[i + 2][j] == enemy && currentThisBoard[i + 3][j] == enemy))
			return true;
		else
			return false;
	} //else if(i==1)
	else if (i == 2)
	{

		if ((currentThisBoard[i - 1][j] == enemy &&currentThisBoard[i + 1][j] == enemy&&currentThisBoard[i + 2][j] == enemy) ||
			(currentThisBoard[i + 1][j] == enemy && currentThisBoard[i + 2][j] == enemy && currentThisBoard[i + 3][j] == enemy) ||
			(currentThisBoard[i - 1][j] == enemy &&currentThisBoard[i - 2][j] == enemy&&currentThisBoard[i + 2][j] == enemy))        //竖直
			return true;
		else
			return false;
	}// else if(i==2)
	else if (i == 9)
	{
		if ((currentThisBoard[6][j] == enemy && currentThisBoard[7][j] == enemy && currentThisBoard[8][j] == enemy) ||
			(currentThisBoard[7][j] == enemy && currentThisBoard[8][j] == enemy && currentThisBoard[10][j] == enemy) ||
			(currentThisBoard[8][j] == enemy && currentThisBoard[10][j] == enemy && currentThisBoard[11][j] == enemy))
			return true;
		else
			return false;
	}
	else if (i == 10)
	{
		if ((currentThisBoard[9][j] == enemy && currentThisBoard[7][j] == enemy && currentThisBoard[8][j] == enemy) ||
			(currentThisBoard[9][j] == enemy && currentThisBoard[8][j] == enemy && currentThisBoard[11][j] == enemy))
			return true;
		else
			return false;
	}
	else if (i == 11)
	{
		if ((currentThisBoard[8][j] == enemy && currentThisBoard[9][j] == enemy && currentThisBoard[10][j] == enemy))
			return true;
		else
			return false;
	}
	else
	{
		if ((currentThisBoard[i - 3][j] == enemy && currentThisBoard[i - 2][j] == enemy && currentThisBoard[i - 1][j] == enemy) ||
			(currentThisBoard[i - 2][j] == enemy && currentThisBoard[i - 1][j] == enemy && currentThisBoard[i + 1][j] == enemy) ||
			(currentThisBoard[i - 1][j] == enemy && currentThisBoard[i + 1][j] == enemy && currentThisBoard[i + 2][j] == enemy) ||
			(currentThisBoard[i + 1][j] == enemy && currentThisBoard[i + 2][j] == enemy && currentThisBoard[i + 3][j] == enemy))
			return true;
		else
			return false;
	}
}
bool tjudge2(int i, int j, char player, int(*currentThisBoard)[GRID_NUM])
{
	char enemy = (player == BLACK) ? WHITE : BLACK;
	if (j == 0)
	{
		if (currentThisBoard[i][1] == enemy && currentThisBoard[i][2] == enemy && currentThisBoard[i][3] == enemy)        //竖直方向
			return true;
		else
			return false;
	} //if(i==0)
	else if (j == 1)
	{
		if ((currentThisBoard[i][0] == enemy &&currentThisBoard[i][2] == enemy&&currentThisBoard[i][3] == enemy) ||               //竖直
			(currentThisBoard[i][2] == enemy && currentThisBoard[i][3] == enemy && currentThisBoard[i][4] == enemy))
			return true;
		else
			return false;
	}//else if(j==1)
	else if (j == 2)
	{

		if ((currentThisBoard[i][0] == enemy &&currentThisBoard[i][1] == enemy&&currentThisBoard[i][3] == enemy) ||
			(currentThisBoard[i][1] == enemy && currentThisBoard[i][3] == enemy && currentThisBoard[i][4] == enemy) ||
			(currentThisBoard[i][3] == enemy &&currentThisBoard[i][4] == enemy&&currentThisBoard[i][5] == enemy))            //竖直
			return true;
		else
			return false;
	}// else if(i==2)
	else if (j == 9)
	{
		if ((currentThisBoard[i][8] == enemy && currentThisBoard[i][10] == enemy && currentThisBoard[i][11] == enemy) ||
			(currentThisBoard[i][7] == enemy && currentThisBoard[i][8] == enemy && currentThisBoard[i][10] == enemy) ||
			(currentThisBoard[i][6] == enemy && currentThisBoard[i][7] == enemy && currentThisBoard[i][8] == enemy))
			return true;
		else
			return false;
	}
	else if (j == 10)
	{
		if ((currentThisBoard[i][8] == enemy && currentThisBoard[i][9] == enemy && currentThisBoard[i][11] == enemy) ||
			(currentThisBoard[i][7] == enemy && currentThisBoard[i][8] == enemy && currentThisBoard[i][9] == enemy))
			return true;
		else
			return false;
	}
	else if (j == 11)
	{
		if ((currentThisBoard[i][9] == enemy && currentThisBoard[i][10] == enemy && currentThisBoard[i][8] == enemy))
			return true;
		else
			return false;
	}
	else
	{
		if ((currentThisBoard[i][j - 3] == enemy && currentThisBoard[i][j - 2] == enemy && currentThisBoard[i][j - 1] == enemy) ||
			(currentThisBoard[i][j - 2] == enemy && currentThisBoard[i][j - 1] == enemy && currentThisBoard[i][j + 1] == enemy) ||
			(currentThisBoard[i][j - 1] == enemy && currentThisBoard[i][j + 1] == enemy && currentThisBoard[i][j + 2] == enemy) ||
			(currentThisBoard[i][j + 1] == enemy && currentThisBoard[i][j + 2] == enemy && currentThisBoard[i][j + 3] == enemy))
			return true;
		else
			return false;
	}

}
bool tjudge3(int i, int j, char player, int(*currentThisBoard)[GRID_NUM])
{
	bool tjudge1(int, int, char, int(*)[GRID_NUM]);
	bool tjudge2(int, int, char, int(*)[GRID_NUM]);
	char enemy = (player == BLACK) ? WHITE : BLACK;
	if (tjudge1(i, j, player, currentThisBoard) == true || tjudge2(i, j, player, currentThisBoard) == true)
		return true;
	else
		return false;
}
