#include "agent.h"

int Agent::countMaterial(int colour) {
	int material = 0;

	for (int p : pM->pieces) {
		std::unordered_set<int> pos = pB->chessPos[colour | p];
		material += pos.size() * pM->pieceValues[p];
		
		// evaluate piece position
		for (int index : pos) {
			if (colour == Red) {
				material += pM->piecePosValues[p][index];
			}
			else {
				material += pM->piecePosValues[p][254 - index];
			}
		}
	}
	
	return material;
}

Agent::Agent(BitMap* pBitMap, Board* pBoard) {
	agentMove = 0;
	pM = pBitMap;
	pB = pBoard;
}

void Agent::resetAgentMove() {
	agentMove = 0;
}

int Agent::evaluate() {
	int selfEval = countMaterial(pB->gameTurn);
	int opponentEval = countMaterial(pB->gameTurn ^ GetColour);

	return selfEval - opponentEval;
}

int Agent::search(int depth, int alpha, int beta) {
	if (depth == 0 || pB->isKingDead()) {
		return searchCaptures(alpha, beta);
	}

	pB->generateAllMoves();
	std::vector<Move> moves = pB->validMoves;

	if (moves.size() == 0) {
		if (pB->isKingInCheck()) {
			return -INT_MAX;
		}
		return 0;
	}

	for (Move m : moves) {
		pB->makeMove(m);
		int evaluation = -search(depth - 1, -beta, -alpha);
		pB->undoMove(m);
		if (evaluation >= beta) {
			return beta;
		}
		if (evaluation > alpha) {
			alpha = evaluation;
			if (depth == SEARCH_DEPTH) {
				agentMove = m;
			}
		}
	}
	
	return alpha;
}

int Agent::searchCaptures(int alpha, int beta) {
	int evaluation = evaluate();
	if (evaluation >= beta) {
		return beta;
	}
	alpha = max(alpha, evaluation);

	pB->generateAllMoves(true);
	std::vector<Move> captureMoves = pB->validMoves;

	for (Move m : captureMoves) {
		pB->makeMove(m);
		evaluation = -searchCaptures(-beta, -alpha);
		pB->undoMove(m);

		if (evaluation >= beta) {
			return beta;
		}
		alpha = max(alpha, evaluation);
	}

	return alpha;
}
