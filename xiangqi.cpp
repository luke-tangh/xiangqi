#include "utility.h"
#include "xiangqi.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

Board::Board() {
	gameTurn = Red;
	lastMove = -1;
	holdChessVal = -1;
	holdChessPos = -1;

	redPieces = {
		Red | Advisor,
		Red | Bishop,
		Red | Cannon,
		Red | King,
		Red | Knight,
		Red | Pawn,
		Red | Rook
	};

	blackPieces = {
		Black | Advisor,
		Black | Bishop,
		Black | Cannon,
		Black | King,
		Black | Knight,
		Black | Pawn,
		Black | Rook
	};

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

	fenCharToInt = {
		{'A', Red | Advisor}, {'a', Black | Advisor},
		{'B', Red | Bishop}, {'b', Black | Bishop},
		{'C', Red | Cannon}, {'c', Black | Cannon},
		{'K', Red | King}, {'k', Black | King},
		{'N', Red | Knight}, {'n', Black | Knight},
		{'P', Red | Pawn}, {'p', Black | Pawn},
		{'R', Red | Rook}, {'r', Black | Rook}
	};

	fenIntToChar = {
		{Red | Advisor, 'A'}, {Black | Advisor, 'a'},
		{Red | Bishop, 'B'}, {Black | Bishop, 'b'},
		{Red | Cannon, 'C'}, {Black | Cannon, 'c'},
		{Red | King, 'K'}, {Black | King, 'k'},
		{Red | Knight, 'N'}, {Black | Knight, 'n'},
		{Red | Pawn, 'P'}, {Black | Pawn, 'p'},
		{Red | Rook, 'R'}, {Black | Rook, 'r'}
	};
}

void Board::readFromFEN(std::string fen) {
	std::vector<std::string> fs = split(fen, Space);

	// chess board
	int file = 0, rank = 0;
	for (char sym : fs[0]) {
		if (sym == '/') {
			file = 0;
			rank++;
		}
		else {
			if (isdigit(sym)) {
				file += sym - '0';
			}
			else {
				squares[rank * 9 + file] = fenCharToInt[sym];
				file++;
			}
		}
	}
}

std::string Board::convertToFEN() {
	// chess board
	std::string fen = "";
	for (int i = 0; i < 10; ++i) {
		int acc = 0;
		for (int j = 0; j < 9; ++j) {
			int index = i * 9 + j;
			if (squares[index] == 0) {
				acc++;
			}
			else {
				if (acc != 0) {
					fen += acc + '0';
					acc = 0;
				}
				fen += fenIntToChar[squares[index]];
			}
		}
		if (acc != 0) {
			fen += acc + '0';
		}
		fen += '/';
	}

	// remove redundant '/' at the end
	fen.pop_back();

	return fen;
}

bool Board::isTurnToMove(int index) {
	return (squares[index] & GetColour) == gameTurn;
}

void Board::pickUpChess(int index) {
	holdChessPos = index;
	holdChessVal = squares[index];

	squares[index] = None;
}

void Board::dropChess() {
	squares[holdChessPos] = holdChessVal;

	validMoves.clear();
	holdChessVal = -1;
	holdChessPos = -1;
}

void Board::makeMove(Move m) {
	const int to = m & GetMove;
	m >>= 8;
	const int from = m & GetMove;
	
	if (holdChessVal != -1) {
		// place holding chess back to the board
		// before make move - essential
		dropChess();
		lastMove = from;
		validMoves.clear();
	}

	const int piece = squares[from];
	const int eat = squares[to];

	squares[from] = None;
	squares[to] = piece;
	chessPos[piece].erase(from);
	chessPos[piece].insert(to);
	if (eat != None) {
		chessPos[eat].erase(to);
	}
	
	deadPieces.push(eat);
	swapGameTurn();
}

void Board::undoMove(Move m) {
	const int to = m & GetMove;
	m >>= 8;
	const int from = m & GetMove;

	const int piece = squares[to];
	const int lastCapture = deadPieces.top();
	deadPieces.pop();

	squares[from] = piece;
	squares[to] = lastCapture;

	chessPos[piece].insert(from);
	chessPos[piece].erase(to);
	if (lastCapture != None) {
		chessPos[lastCapture].insert(to);
	}

	lastMove = -1;
	swapGameTurn();
}

bool Board::isSameColour(int chessA, int chessB) {
	return (chessA & GetColour) == (chessB & GetColour);
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

void Board::swapGameTurn() {
	gameTurn ^= GetColour;
}

std::vector<Move> Board::moveGeneration(int index, int chess) {
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

std::vector<Move> Board::advisorMoves(int index, int chess) {
	std::vector<Move> validMoves;
	for (int move : slidingMoves) {
		int target = index + move;
		// out of bound conditions
		if (target < 0 || target > 89) continue;
		if (target % 9 > 5 || target % 9 < 3) continue;
		if (target / 9 > 2 && target / 9 < 7) continue;
		// friendly pieces block
		if (isSameColour(squares[target], chess)) continue;
		validMoves.push_back((index << 8) + target);
	}
	return validMoves;
}

std::vector<Move> Board::bishopMoves(int index, int chess) {
	std::vector<Move> validMoves;
	for (int move : slidingMoves) {
		int target = index + 2 * move;
		int block = index + move;
		// out of bound conditions
		if (target < 0 || target > 89) continue;
		if ((chess & GetColour) == Black) {
			if (target > 44 || (target / 9) % 2 == 1) continue;
		}
		else {
			if (target < 45 || (target / 9) % 2 == 0) continue;
		}
		// pieces block conditions
		if (squares[block] != None) continue;
		// friendly pieces block
		if (isSameColour(squares[target], chess)) continue;
		validMoves.push_back((index << 8) + target);
	}
	return validMoves;
}

std::vector<Move> Board::cannonMoves(int index, int chess) {
	std::vector<Move> validMoves;
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
					validMoves.push_back((index << 8) + target);
					break;
				}
			}
			else {
				validMoves.push_back((index << 8) + target);
			}
			target += move;
		}
	}
	return validMoves;
}

std::vector<Move> Board::kingMoves(int index, int chess) {
	std::vector<Move> validMoves;
	for (int move : directMoves) {
		int target = index + move;
		// out of bound conditions
		if (target < 0 || target > 89) continue;
		if (target % 9 > 5 || target % 9 < 3) continue;
		if (target / 9 > 2 && target / 9 < 7) continue;
		// friendly pieces block conditions
		if (isSameColour(squares[target], chess)) continue;
		validMoves.push_back((index << 8) + target);
	}
	return validMoves;
}

std::vector<Move> Board::knightMoves(int index, int chess) {
	std::vector<Move> validMoves;
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
		validMoves.push_back((index << 8) + target);
	}
	return validMoves;
}

std::vector<Move> Board::pawnMoves(int index, int chess) {
	std::vector<Move> validMoves;
	int target = 0;
	if ((chess & GetColour) == Black) {
		// not passing river
		if (index < 45) {
			target = index + 9;
			validMoves.push_back((index << 8) + target);
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
			validMoves.push_back((index << 8) + target);
		}
		return validMoves;
	}
	else {
		// not passing river
		if (index > 44) {
			target = index - 9;
			validMoves.push_back((index << 8) + target);
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
			validMoves.push_back((index << 8) + target);
		}
		return validMoves;
	}
}

std::vector<Move> Board::rookMoves(int index, int chess) {
	std::vector<Move> validMoves;
	for (int move : directMoves) {
		int target = index + move;
		while (true) {
			// out of bound conditions
			if (target < 0 || target > 89) break;
			if (target / 9 != index / 9 && target % 9 != index % 9) break;
			// friendly pieces block conditions
			if (isSameColour(squares[target], chess)) break;
			if (squares[target] != None) {
				validMoves.push_back((index << 8) + target);
				break;
			}
			else {
				validMoves.push_back((index << 8) + target);
			}
			target += move;
		}
	}
	return validMoves;
}

void Board::legalMoveGeneration(int index, int chess) {
	std::vector<Move> pseudoLegalMoves = moveGeneration(index, chess);
	std::vector<Move> legalMoves;
	int kingColour = chess & GetColour;
	int rivalColour = kingColour ^ GetColour;
	for (Move m : pseudoLegalMoves) {
		makeMove(m);
		if (isKingInCheck()) {
			// king captured
		}
		else {
			legalMoves.push_back(m);
		}
		undoMove(m);
	}
	validMoves = legalMoves;
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

bool Board::isKingInCheck() {
	int kingColour = gameTurn ^ GetColour;
	int rivalColour = gameTurn;
	int kingPos = *chessPos[King | kingColour].begin();

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
