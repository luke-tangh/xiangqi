#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "utility.h"

// Starting position expressed in FEN notation
// But use { Q | q } for Kings
// Use UPPER case for red and lower case for black
// { board | turn | holding chess } separated by SPACE 
const std::string START_FEN = "rkbaqabkr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RKBAQABKR r .";

/* squares (index to position)
* --------------------------
* 00 01 02 03 04 05 06 07 08
* 09 10 11 12 13 14 15 16 17
* 18 19 20 21 22 23 24 25 26
* 27 28 29 30 31 32 33 34 35
* 36 37 38 39 40 41 42 43 44
* 45 46 47 48 49 50 51 52 53
* 54 55 56 57 58 59 60 61 62
* 63 64 65 66 67 68 69 70 71
* 72 73 74 75 76 77 78 79 80
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
	std::vector<int> directMoves;
	std::vector<int> slidingMoves;
	std::vector<int> knightPosMoves;
	std::unordered_map<int, int> chessPos;
public:
	Board();
	bool isSameColour(int chessA, int chessB);
	void reverseGameTurn();
	void initSquare();
	void initChessPosMap();
	void setChessPos(int index, int chess);
	void clearHoldingChess();
	void setHoldingChess(int index);
	Chess getHoldingChess();
	void readFromFEN(std::string fen);
	std::string convertToFEN();
	std::vector<int> moveGeneration(int index, int chess);
	std::vector<int> advisorMoves(int index, int chess);
	std::vector<int> bishopMoves(int index, int chess);
	std::vector<int> cannonMoves(int index, int chess);
	std::vector<int> kingMoves(int index, int chess);
	std::vector<int> knightMoves(int index, int chess);
	std::vector<int> pawnMoves(int index, int chess);
	std::vector<int> rookMoves(int index, int chess);
};
