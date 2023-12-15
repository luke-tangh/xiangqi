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
	initSquare();
}

bool Board::isSameColour(int chessA, int chessB) {
	return (chessA & GetColour) == (chessB & GetColour);
}

void Board::reverseGameTurn() {
	if (gameTurn == BlackTurn) {
		gameTurn = RedTurn;
	}
	else {
		gameTurn = BlackTurn;
	}
}

int Board::getCurrentGameTurn() {
	if (gameTurn == BlackTurn) {
		return Black;
	}
	else {
		return Red;
	}
}

int Board::getReversedPiece(int chess) {
	return chess & GetColour ^ GetColour | chess & GetPiece;
}

// if the chess clicked is turn to move
bool Board::turnCheck(int index) {
	if (gameTurn == BlackTurn) {
		return (squares[index] & GetColour) == Black;
	}
	else {
		return (squares[index] & GetColour) == Red;
	}
}

// initialise board to the start of game
void Board::initSquare() {
	squares.clear();
	squares.resize(90, None);
	gameTurn = RedTurn;
	holdingChess = { NotClicked, None };
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

void Board::setHoldingChess(int index) {
	holdingChess = { index, squares[index] };
}

void Board::clearHoldingChess() {
	holdingChess = { NotClicked, None };
}

Chess Board::getHoldingChess() {
	return holdingChess;
}

void Board::readFromFEN(std::string fen) {
	std::unordered_map<char, int> fenCharToInt {
		{'A', Red | Advisor}, {'a', Black | Advisor},
		{'B', Red | Bishop}, {'b', Black | Bishop},
		{'C', Red | Cannon}, {'c', Black | Cannon},
		{'Q', Red | King}, {'q', Black | King},
		{'K', Red | Knight}, {'k', Black | Knight},
		{'P', Red | Pawn}, {'p', Black | Pawn},
		{'R', Red | Rook}, {'r', Black | Rook}
	};

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

	// modify game turn
	gameTurn = fs[1][0];
}

std::string Board::convertToFEN() {
	std::unordered_map<int, char> fenIntToChar {
	{Red | Advisor, 'A'}, {Black | Advisor, 'a'},
	{Red | Bishop, 'B'}, {Black | Bishop, 'b'},
	{Red | Cannon, 'C'}, {Black | Cannon, 'c'},
	{Red | King, 'Q'}, {Black | King, 'q'},
	{Red | Knight, 'K'}, {Black | Knight, 'k'},
	{Red | Pawn, 'P'}, {Black | Pawn, 'p'},
	{Red | Rook, 'R'}, {Black | Rook, 'r'}
	};

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

	// game turn
	fen += Space;
	fen += gameTurn;

	// holding chess
	if (holdingChess.value != None) {
		fen += Space;
		fen += fenIntToChar[holdingChess.value];
	}
	else {
		fen += Space;
		fen += NotAttached;
	}

	return fen;
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
		// attacking moves
		if (squares[target] != None) {
			validMoves.push_back(target + AttackingMove);
		}
		else {
			validMoves.push_back(target);
		}
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
		// attacking moves
		if (squares[target] != None) {
			validMoves.push_back(target + AttackingMove);
		}
		else {
			validMoves.push_back(target);
		}
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
				// attacking moves
				if (squares[target] != None) {
					validMoves.push_back(target + AttackingMove);
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
		// king-face-king condition
		int mapKingPos = *chessPos[getReversedPiece(chess)].begin();
		int redKingPos = max(target, mapKingPos);
		int blackKingPos = min(target, mapKingPos);
		if (redKingPos % 9 == blackKingPos % 9) {
			bool block = false;
			for (int i = blackKingPos + 9; i < redKingPos; i += 9) {
				if (squares[i] != None) {
					block = true;
					break;
				}
			}
			if (!block) continue;
		}
		// attacking moves
		if (squares[target] != None) {
			validMoves.push_back(target + AttackingMove);
		}
		else {
			validMoves.push_back(target);
		}
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
		// attacking moves
		if (squares[target] != None) {
			validMoves.push_back(target + AttackingMove);
		}
		else {
			validMoves.push_back(target);
		}
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
			// attacking moves
			if (squares[target] != None) {
				validMoves.push_back(target + AttackingMove);
			}
			else {
				validMoves.push_back(target);
			}
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
			// attacking moves
			if (squares[target] != None) {
				validMoves.push_back(target + AttackingMove);
			}
			else {
				validMoves.push_back(target);
			}
		}
		return validMoves;
	}
	else {
		// not passing river
		if (index > 44) {
			target = index - 9;
			// attacking moves
			if (squares[target] != None) {
				validMoves.push_back(target + AttackingMove);
			}
			else {
				validMoves.push_back(target);
			}
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
			// attacking moves
			if (squares[target] != None) {
				validMoves.push_back(target + AttackingMove);
			}
			else {
				validMoves.push_back(target);
			}
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
			// attacking moves
			if (squares[target] != None) {
				validMoves.push_back(target + AttackingMove);
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
	int thisMove = 0;
	int kingColour = chess & GetColour;
	int rivalColour = kingColour ^ GetColour;
	for (int move : pseudoLegalMoves) {
		if (move >= AttackingMove) {
			thisMove = move - AttackingMove;
		}
		else {
			thisMove = move;
		}
		int target = squares[thisMove];
		setChessPos(thisMove, chess);
		std::vector<int> atk = attackMoves(rivalColour);
		if (count(atk.begin(), atk.end(), *chessPos[King | kingColour].begin())) {
			// king captured
		}
		else if (isKingFaces()) {
			// king faces
		}
		else {
			legalMoves.push_back(move);
		}
		setChessPos(thisMove, target);
	}
	return legalMoves;
}

std::vector<int> Board::attackMoves(int colour) {
	std::vector<int> attackMoves;
	if (colour == Red) {
		for (int p : redAtkPieces) {
			for (auto it = chessPos[p].begin(); it != chessPos[p].end(); it++) {
				for (int m : moveGeneration(*it, p)) {
					if (m >= AttackingMove) {
						attackMoves.push_back(m - AttackingMove);
					}
				}
			}
		}
	}
	else {
		for (int p : blackAtkPieces) {
			for (auto it = chessPos[p].begin(); it != chessPos[p].end(); it++) {
				for (int m : moveGeneration(*it, p)) {
					if (m >= AttackingMove) {
						attackMoves.push_back(m - AttackingMove);
					}
				}
			}
		}
	}
	
	return attackMoves;
}

bool Board::isKingFaces() {
	int redKingPos = *chessPos[Red | King].begin();
	int blackKingPos = *chessPos[Black | King].begin();
	
	for (int i = blackKingPos + 9; i < redKingPos; i += 9) {
		if (squares[i] != None) {
			return false;
		}
	}
	
	return true;
}
