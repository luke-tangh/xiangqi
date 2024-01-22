#pragma once
#include <vector>
#include "utility.h"

class BitMap {
public:
	std::vector<int> pieces;

	// value of a piece
	// ordered by PieceIndex
	std::vector<int> pieceValues;

	// value of a piece on a position
	// 7 x 256 2D vector
	// ordered by PieceIndex
	// recorded in Red's perspective
	// use `254 - index` to flip map
	std::vector<std::vector<int>> piecePosValues;

	std::vector<int> inBoard;
	std::vector<int> blackField;
	std::vector<int> whiteField;
	std::vector<int> kingSquare;

	BitMap();
};
