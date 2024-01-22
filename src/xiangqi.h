#pragma once
#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "bitmap.h"
#include "utility.h"

// Starting position expressed in FEN notation
// Use lower case for black and UPPER case for red
const std::string START_FEN = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR";

/*
* squares is a 16x16 (i.e. 256) 1D vector 
* _ _ _ ___ ___ ___ ___ ___ ___ ___ ___ ___ _ _ _ _
* _ _ _ ___ ___ ___ ___ ___ ___ ___ ___ ___ _ _ _ _
* _ _ _ ___ ___ ___ ___ ___ ___ ___ ___ ___ _ _ _ _
* _ _ _ 051 052 053 054 055 056 057 058 059 _ _ _ _
* _ _ _ 067 068 069 070 071 072 073 074 075 _ _ _ _
* _ _ _ 083 084 085 086 087 088 089 090 091 _ _ _ _
* _ _ _ 099 100 101 102 103 104 105 106 107 _ _ _ _
* _ _ _ 115 116 117 118 119 120 121 122 123 _ _ _ _
* _ _ _ 131 132 133 134 135 136 137 138 139 _ _ _ _
* _ _ _ 147 148 149 150 151 152 153 154 155 _ _ _ _
* _ _ _ 163 164 165 166 167 168 169 170 171 _ _ _ _
* _ _ _ 179 180 181 182 183 184 185 186 187 _ _ _ _
* _ _ _ 195 196 197 198 199 200 201 202 203 _ _ _ _
* _ _ _ ___ ___ ___ ___ ___ ___ ___ ___ ___ _ _ _ _
* _ _ _ ___ ___ ___ ___ ___ ___ ___ ___ ___ _ _ _ _
* _ _ _ ___ ___ ___ ___ ___ ___ ___ ___ ___ _ _ _ _
* 
*     | -17 | -16 | -15 |
* opt | - 1 |  x  | + 1 |
*     | +15 | +16 | +17 |
*/

class Board {
private:
	BitMap* pM;

	// deadPieces stores captured pieces per move
	// None (0) if no piece is captured add
	std::stack<int> deadPieces;

	std::unordered_map<int, char> fenIntToChar;
	std::unordered_map<char, int> fenCharToInt;

	std::vector<int> directMoves;
	std::vector<int> slidingMoves;
	std::vector<int> knightPosMoves;
	std::vector<int> reverseKnightBlocks;

	void swapGameTurn();
	bool isKingFaces();

	std::vector<Move> advisorMoves(int index);
	std::vector<Move> bishopMoves(int index);
	std::vector<Move> cannonMoves(int index);
	std::vector<Move> kingMoves(int index);
	std::vector<Move> knightMoves(int index);
	std::vector<Move> pawnMoves(int index);
	std::vector<Move> rookMoves(int index);
	std::vector<Move> moveGeneration(int index, int chess);

public:
	int gameTurn;
	int lastMove;
	int holdChessVal;
	int holdChessPos;
	std::vector<int> squares;  // squares[index] = piece
	std::unordered_map<int, std::unordered_set<int>> chessPos;
	std::vector<Move> validMoves;

	Board(BitMap* pBitMap);

	void initSquare();
	void initChessPosMap();

	void pickUpChess(int index);
	void dropChess();
	void makeMove(Move m);
	void undoMove(Move m);
	void readFromFEN(std::string fen);
	std::string convertToFEN();

	bool isTurnToMove(int index);
	bool isValidMove(int index);
	bool isKingDead();
	bool isKingInCheck();
	void legalMoveGeneration(int index, int chess, bool attackingMove = false);
	void generateAllMoves(bool attackingMove=false);
};
