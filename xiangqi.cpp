#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "xiangqi.h"
#include "utility.h"

Board::Board() {
	redAtkPieces = {   
		Red | Cannon, 
		Red | Knight, 
		Red | Pawn, 
		Red | Rook
	};
	blackAtkPieces = { 
		Black | Cannon, 
		Black | Knight, 
		Black | Pawn, 
		Black | Rook
	};
	directMoves = { -9, -1, +9, +1 };
	slidingMoves = { -10, +8, +10, -8 };
	knightPosMoves = { -17, -19, -11, +7, +17, +19, +11, -7 };
	reverseKnightBlocks = { -8, -10, -10, +8, +8, +10, +10, -8 };
	initSquare();
}

bool Board::isSameColour(int chessA, int chessB) {
	return (chessA & GetColour) == (chessB & GetColour);
}

std::vector<int> Board::getSquares() {
	return squares;
}

char Board::getChessOnPos(int index) {
	return squares[index];
}

// initialise board to the start of game
void Board::initSquare() {
	squares.resize(90, None);
}

// read content in `squares` to map `chessPos`
void Board::initChessPosMap() {
	for (int i = 0; i < squares.size(); ++i) {
		if (squares[i] != 0) {
			chessPos[squares[i]].insert(i);
		}
	}
}

// set `squares` and `chessPos`
// set chess=None to remove chess on index
void Board::setChessPos(int index, int chess) {
	// thisChess is the chess on index originally
	int thisChess = squares[index];
	squares[index] = chess;
	if (chess != None) {
		chessPos[thisChess].erase(index);
		chessPos[chess].insert(index);
	}
	// remove index position
	else {
		chessPos[thisChess].erase(index);
	}
}

std::vector<int> Board::moveGeneration(int index, int chess) {
	const int piece = chess & GetPiece;
	switch (piece) {
	case Advisor: return advisorMoves(index, chess);
	case Bishop: return bishopMoves(index, chess);
	case Cannon: return cannonMoves(index, chess);
	case King: return kingMoves(index, chess);
	case Knight: return knightMoves(index, chess);
	case Pawn: return pawnMoves(index, chess);
	case Rook: return rookMoves(index, chess);
	default: return {};
	}
}

std::vector<int> Board::advisorMoves(int index, int chess) {
	std::vector<int> validMoves;
	for (int move : slidingMoves) {
		int target = index + move;
		// out of bound conditions
		if (target < 0 || target > 89) continue;
		if (target % 9 > 5 || target % 9 < 3) continue;
		if (target / 9 > 2 && target / 9 < 7) continue;
		// friendly pieces block
		if (isSameColour(squares[target], chess)) continue;
		validMoves.push_back(target);
	}
	return validMoves;
}

std::vector<int> Board::bishopMoves(int index, int chess) {
	std::vector<int> validMoves;
	for (int move : slidingMoves) {
		int target = index + 2 * move;
		int block = index + move;
		// out of bound conditions
		if (target < 0 || target > 89) continue;
		if ((chess & GetColour) == Black && target > 44) continue;
		if ((chess & GetColour) == Red && target < 45) continue;
		// pieces block conditions
		if (squares[block] != None) continue;
		// friendly pieces block
		if (isSameColour(squares[target], chess)) continue;
		validMoves.push_back(target);
	}
	return validMoves;
}

std::vector<int> Board::cannonMoves(int index, int chess) {
	std::vector<int> validMoves;
	for (int move : directMoves) {
		int target = index + move;
		bool mount = false;
		while (true) {
			// out of bound conditions
			if (target < 0 || target > 89) break;
			if (target / 9 != index / 9 && target % 9 != index % 9) break;
			if (!mount && squares[target] != None) {
				mount = true;
				target += move;
				continue;
			}
			if (mount) {
				// friendly pieces block conditions
				if (isSameColour(squares[target], chess)) break;
				if (squares[target] != None) {
					validMoves.push_back(target);
					break;
				}
			}
			else {
				validMoves.push_back(target);
			}
			target += move;
		}
	}
	return validMoves;
}

std::vector<int> Board::kingMoves(int index, int chess) {
	std::vector<int> validMoves;
	for (int move : directMoves) {
		int target = index + move;
		// out of bound conditions
		if (target < 0 || target > 89) continue;
		if (target % 9 > 5 || target % 9 < 3) continue;
		if (target / 9 > 2 && target / 9 < 7) continue;
		// friendly pieces block conditions
		if (isSameColour(squares[target], chess)) continue;
		validMoves.push_back(target);
	}
	return validMoves;
}

std::vector<int> Board::knightMoves(int index, int chess) {
	std::vector<int> validMoves;
	for (int i = 0; i < knightPosMoves.size(); ++i) {
		int target = index + knightPosMoves[i];
		int block = index + directMoves[i / 2];
		// out of bound conditions
		if (target < 0 || target > 89) continue;
		if (abs(target % 9 - index % 9) > 2) continue;
		// pieces block conditions
		if (block > 0 && block < 90 && squares[block] != None) continue;
		// friendly pieces block conditions
		if (isSameColour(squares[target], chess)) continue;
		validMoves.push_back(target);
	}
	return validMoves;
}

std::vector<int> Board::pawnMoves(int index, int chess) {
	std::vector<int> validMoves;
	int target = 0;
	if ((chess & GetColour) == Black) {
		// not passing river
		if (index < 45) {
			target = index + 9;
			validMoves.push_back(target);
			return validMoves;
		}
		for (int move : directMoves) {
			target = move + index;
			// no backwards move
			if (move < -1) continue;
			// out of bound conditions
			if (target < 0 || target > 89) continue;
			if (target / 9 != index / 9 && target % 9 != index % 9) continue;
			// friendly pieces block conditions
			if (isSameColour(squares[target], chess)) continue;
			validMoves.push_back(target);
		}
		return validMoves;
	}
	else {
		// not passing river
		if (index > 44) {
			target = index - 9;
			validMoves.push_back(target);
			return validMoves;
		}
		for (int move : directMoves) {
			target = move + index;
			// no backwards move
			if (move > 1) continue;
			// out of bound conditions
			if (target < 0 || target > 89) continue;
			if (target / 9 != index / 9 && target % 9 != index % 9) continue;
			// friendly pieces block conditions
			if (isSameColour(squares[target], chess)) continue;
			validMoves.push_back(target);
		}
		return validMoves;
	}
}

std::vector<int> Board::rookMoves(int index, int chess) {
	std::vector<int> validMoves;
	for (int move : directMoves) {
		int target = index + move;
		while (true) {
			// out of bound conditions
			if (target < 0 || target > 89) break;
			if (target / 9 != index / 9 && target % 9 != index % 9) break;
			// friendly pieces block conditions
			if (isSameColour(squares[target], chess)) break;
			if (squares[target] != None) {
				validMoves.push_back(target);
				break;
			}
			else {
				validMoves.push_back(target);
			}
			target += move;
		}
	}
	return validMoves;
}

std::vector<int> Board::legalMoveGeneration(int index, int chess) {
	std::vector<int> pseudoLegalMoves = moveGeneration(index, chess);
	std::vector<int> legalMoves;
	int kingColour = chess & GetColour;
	int rivalColour = kingColour ^ GetColour;
	for (int move : pseudoLegalMoves) {
		int target = squares[move];
		setChessPos(move, chess);
		if (isKingInCheck(kingColour | King)) {
			// king captured
		}
		else {
			legalMoves.push_back(move);
		}
		setChessPos(move, target);
	}
	return legalMoves;
}

bool Board::isKingFaces() {
	int redKingPos = *chessPos[Red | King].begin();
	int blackKingPos = *chessPos[Black | King].begin();

	if (redKingPos % 9 != blackKingPos % 9) {
		return false;
	}

	for (int i = blackKingPos + 9; i < redKingPos; i += 9) {
		if (squares[i] != None) {
			return false;
		}
	}

	return true;
}

bool Board::isKingInCheck(int king) {
	int kingColour = king & GetColour;
	int rivalColour = kingColour ^ GetColour;
	int kingPos = *chessPos[king].begin();

	if (isKingFaces()) {
		return true;
	}


	// pawn attack
	for (int move : directMoves) {
		int target = kingPos + move;
		// no backward attack
		if (kingColour == Black && move < -1) continue;
		if (kingColour == Red && move > 1) continue;
		// attack condition
		if (squares[target] == (rivalColour | Pawn)) {
			return true;
		}
	}

	// knight attack
	for (int i = 0; i < knightPosMoves.size(); ++i) {
		int target = kingPos + knightPosMoves[i];
		int block = kingPos + reverseKnightBlocks[i];
		// out of bound conditions
		if (target < 0 || target > 89) continue;
		// pieces block conditions
		if (block > 0 && block < 90 && squares[block] != None) continue;
		// attack condition
		if (squares[target] == (rivalColour | Knight)) {
			return true;
		}
	}

	// rook attack
	for (int move : directMoves) {
		int target = kingPos + move;

		while (true) {
			// out of bound conditions
			if (target < 0 || target > 89) break;
			if (target / 9 != kingPos / 9 && target % 9 != kingPos % 9) break;
			// is rook find
			if (squares[target] == (rivalColour | Rook)) {
				return true;
			}
			// pieces block conditions
			if (squares[target] != None) {
				break;
			}
			target += move;
		}
	}

	// cannon attack
	for (int move : directMoves) {
		bool cannonFind = false;
		bool mount = false;
		int target = kingPos + move;

		while (true) {
			// out of bound conditions
			if (target < 0 || target > 89) break;
			if (target / 9 != kingPos / 9 && target % 9 != kingPos % 9) break;
			if (squares[target] == (rivalColour | Cannon)) {
				cannonFind = true;
				break;
			}
			if (squares[target] != None) {
				if (mount) {
					break;
				}
				else {
					mount = true;
				}
			}
			target += move;
		}

		if (cannonFind && mount) {
			return true;
		}
	}

	return false;
}
