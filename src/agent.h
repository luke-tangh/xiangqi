#pragma once
#include "bitmap.h"
#include "xiangqi.h"

class Agent {
private:
	BitMap* pM;
	Board* pB;

	int countMaterial(int colour);

public:
	Move agentMove;

	Agent(BitMap* pBitMap, Board* pBoard);
	void resetAgentMove();
	int evaluate();
	int search(int depth, int alpha, int beta);
	int searchCaptures(int alpha, int beta);
};
