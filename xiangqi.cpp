#include <iostream>
#include <vector>
#include <unordered_map>
#include "xiangqi.h"
#include "utility.h"

Board::Board() {
	gameTurn = RedTurn;
	holdingChess = { NotClicked, None };
	directMoves = { -9, -1, +9, +1 };
	slidingMoves = { -10, -8, +8, +10 };
	knightPosMoves = { -19, -17, -11, +7, +17, +19, +11, -7 };
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

// initialise board to the start of game
void Board::initSquare() {
	squares.clear();
	squares.resize(90, None);
}

// read content in `squares` to map `chessPos`
void Board::initChessPosMap() {
	for (int i = 0; i < squares.size(); ++i) {
		if (squares[i] != 0) {
			chessPos[squares[i]] = i;
		}
	}
}

// set `squares` and `chessPos`
void Board::setChessPos(int index, int chess) {
	// remove chess from index position
	if (squares[index] != 0) {
		chessPos[squares[index]] = -1;
	}

	squares[index] = chess;
	if (chess != 0) {
		chessPos[chess] = index;
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
		// attacking moves
		if (squares[target] != None) {
			validMoves.push_back(target + AttackingMove);
		}
		else {
			validMoves.push_back(target);
		}
	}
	// return if king not on diagonal
	if ((index % 2 == 0 && index < 45) || (index % 2 == 1 && index > 44)) {
		return validMoves;
	}
	for (int move : slidingMoves) {
		int target = index + move;
		// out of bound conditions
		if (target < 0 || target > 89) continue;
		if (target % 9 > 5 || target % 9 < 3) continue;
		if (target / 9 > 2 && target / 9 < 7) continue;
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
