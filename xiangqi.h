#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "utility.h"

// Starting position expressed in FEN notation
// But use { Q | q } for Kings
// Use UPPER case for red and lower case for black
const std::string START_FEN = "rkbaqabkr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RKBAQABKR r .";

/* squares (index to position)
* --------------------------
* 0  1  2  3  4  5  6  7  8
* 9  10 11 12 13 14 15 16 17
* 18 19 20 21 22 23 24 25 26
* ...
* 81 82 83 84 85 86 87 88 89
* --------------------------
*     | -10 | - 9 | - 8 |
* opt | - 1 |  x  | + 1 |
*     | + 8 | + 9 | +10 |
*/

class Board {
private:
	char gameTurn;
	Chess holdingChess;
	std::vector<int> squares;
	std::unordered_map<int, int> chessPos;
public:
	Board();
	void reverseGameTurn();
	void initSquare();
	void initChessPosMap();
	void setChessPos(int index, int chess);
	void clearHoldingChess();
	void setHoldingChess(int index);
	Chess getHoldingChess();
	void readFromFEN(std::string fen);
	std::string convertToFEN();
};
