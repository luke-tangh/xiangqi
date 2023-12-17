#pragma once
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "utility.h"

// Starting position expressed in FEN notation
// Use lower case for black and UPPER case for red
const std::string START_FEN = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR";

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

class Board: public BaseBoard {
private:
	std::vector<int> redAtkPieces;
	std::vector<int> blackAtkPieces;
	std::vector<int> directMoves;
	std::vector<int> slidingMoves;
	std::vector<int> knightPosMoves;
	std::vector<int> reverseKnightBlocks;
	std::unordered_map<int, std::unordered_set<int>> chessPos;

	bool isKingFaces();
	bool isSameColour(int chessA, int chessB);
	std::vector<int> advisorMoves(int index, int chess);
	std::vector<int> bishopMoves(int index, int chess);
	std::vector<int> cannonMoves(int index, int chess);
	std::vector<int> kingMoves(int index, int chess);
	std::vector<int> knightMoves(int index, int chess);
	std::vector<int> pawnMoves(int index, int chess);
	std::vector<int> rookMoves(int index, int chess);
	std::vector<int> moveGeneration(int index, int chess);

public:
	Board();
	void initSquare();
	void initChessPosMap();
	std::vector<int> getSquares();
	char getChessOnPos(int index);
	void setChessPos(int index, int chess);
	std::vector<int> legalMoveGeneration(int index, int chess);
	bool isKingInCheck(int king);
};
