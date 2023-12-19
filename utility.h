#pragma once
#include <conio.h>
#include <graphics.h>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

// Move consist of 16 bit
// XXXXXXXXYYYYYYYY
// X: from position
// Y: to position
using Move = unsigned __int32;

// the value of piece is the result of bitwise OR
// for example the second black cannon is
// Black | Cannon
// = 16 + 3 = 19 = 0001 0011
enum PieceIndex {
	None = 0,
	Advisor = 1,
	Bishop = 2,
	Cannon = 3,
	King = 4,
	Knight = 5,
	Pawn = 6,
	Rook = 7,

	Red = 8,
	Black = 16,
};

enum Control {
	Space = ' ',

	// bitwise masks
	GetPiece = 7,    // 0000 0111
	GetColour = 24,  // 0001 1000
	GetMove = 255    // 1111 1111
};

const int BOARD_HEIGHT = 1000;
const int BOARD_WIDTH = 900;
const int CHESS_SIZE = 100;
const int MARGIN = 50;
// magic number - measured
const int CHESS_RAD = 38;
const int GAP = 10;

std::vector<std::string> split(const std::string& s, char delim);
void putAlphaImage(int picX, int picY, IMAGE* picture);
