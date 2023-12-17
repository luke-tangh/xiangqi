#pragma once
#include <graphics.h>
#include <conio.h>

enum PieceIndex {
	// the value of piece is the result of bitwise OR
	// for example the second black cannon is
	// Black | Cannon
	// = 32 + 16 + 3 = 51 = 0011 0011
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

enum PieceValue {
	AdvisorValue = 2,
	BishopValue = 2,
	CannonValue = 5,
	KnightValue = 4,
	PawnValue = 1,
	RookValue = 9,
};

enum Control {
	NotClicked = -1,
	RedTurn = 'r',
	BlackTurn = 'b',
	NotAttached = '.',
	Space = ' ',

	// bitwise and - magic number
	GetPiece = 7,
	GetColour = 24,
	PieceTypeMask = 15,
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
const int GAP = 10;

std::vector<std::string> split(const std::string& s, char delim);
void putAlphaImage(int picX, int picY, IMAGE* picture);
void printFEN(std::string fen);
