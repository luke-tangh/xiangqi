#include <string>
#include <vector>
#include <unordered_map>
#include "xiangqi.h"

Board::Board() {
	initSquare();
}

// initialise board to the start of game
void Board::initSquare() {
	squares.clear();
	squares.resize(90, None);
}

void Board::readFromFEN(std::string fen) {
	std::unordered_map<char, int> fenMap{
		{'a', Red | Advisor}, {'A', Black | Advisor},
		{'b', Red | Bishop}, {'B', Black | Bishop},
		{'c', Red | Cannon}, {'C', Black | Cannon},
		{'q', Red | King}, {'Q', Black | King},
		{'k', Red | Knight}, {'K', Black | Knight},
		{'p', Red | Pawn}, {'P', Black | Pawn},
		{'r', Red | Rook}, {'R', Black | Rook}
	};

	int file = 0, rank = 9;
	for (char sym : fen) {
		if (sym == '/') {
			file = 0;
			rank--;
		}
		else {
			if (isdigit(sym)) {
				file += sym - '0';
			}
			else {
				squares[rank * 9 + file] = fenMap[sym];
				file++;
			}
		}
	}
}
