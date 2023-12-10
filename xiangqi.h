#pragma once
#include <string>
#include <vector>

// Starting position expressed in FEN notation
// But use { Q | q } for Kings
// Use UPPER case for red and lower case for black
const std::string START_FEN = "rkbaqabkr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RKBAQABKR";

enum Piece {
	None = 0,
	Advisor = 1,
	Bishop = 2,
	Cannon = 3,
	King = 4,
	Knight = 5,
	Pawn = 6,
	Rook = 7,

	Red = 8,
	Black = 16
};

class Board {
private:
	std::vector<int> squares;
public:
	Board();
	void initSquare();
	void readFromFEN(std::string fen);
	std::string convertToFEN();
};
