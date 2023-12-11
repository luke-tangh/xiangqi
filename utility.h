#pragma once
#include <graphics.h>
#include <conio.h>

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

enum Control {
	NotClicked = -1,
	RedTurn = 'r',
	BlackTurn = 'b',
	NotAttached = '.',
	Space = ' ',

	// bitwise and magic number
	GetPiece = 7,
	GetColour = 24,
	AttackingMove = 128
};

struct Chess {
	int index;
	int value;
};

const int BOARD_HEIGHT = 1000;
const int BOARD_WIDTH = 900;
const int CHESS_SIZE = 100;
const int MARGIN = 50;
// magic number - measured
const int CHESS_RAD = 38;

std::vector<std::string> split(const std::string& s, char delim);
void putAlphaImage(int picX, int picY, IMAGE* picture);
