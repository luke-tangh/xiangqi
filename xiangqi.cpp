#include <iostream>
#include <vector>
#include <unordered_map>
#include "xiangqi.h"
#include "utility.h"

Board::Board() {
	gameTurn = RedTurn;
	holdingChess = { NotClicked, None };
	initSquare();
}

void Board::reverseGameTurn() {
	if (gameTurn == BlackTurn) {
		gameTurn = RedTurn;
	}
	else {
		gameTurn = BlackTurn;
	}
}

// initialise board to the start of game
void Board::initSquare() {
	squares.clear();
	squares.resize(90, None);
}

// read content in `squares` to map `chessPos`
void Board::initChessPosMap() {
	for (int i = 0; i < squares.size(); ++i) {
		if (squares[i] != 0) {
			chessPos[squares[i]] = i;
		}
	}
}

// set `squares` and `chessPos`
void Board::setChessPos(int index, int chess) {
	squares[index] = chess;
	if (chess != 0) {
		chessPos[chess] = index;
	}
}

void Board::setHoldingChess(int index) {
	holdingChess = { index, squares[index] };
}

void Board::clearHoldingChess() {
	holdingChess = { NotClicked, None };
}

Chess Board::getHoldingChess() {
	return holdingChess;
}

void Board::readFromFEN(std::string fen) {
	std::unordered_map<char, int> fenCharToInt {
		{'a', Red | Advisor}, {'A', Black | Advisor},
		{'b', Red | Bishop}, {'B', Black | Bishop},
		{'c', Red | Cannon}, {'C', Black | Cannon},
		{'q', Red | King}, {'Q', Black | King},
		{'k', Red | Knight}, {'K', Black | Knight},
		{'p', Red | Pawn}, {'P', Black | Pawn},
		{'r', Red | Rook}, {'R', Black | Rook}
	};

	std::vector<std::string> fs = split(fen, Space);

	// chess board
	int file = 0, rank = 0;
	for (char sym : fs[0]) {
		if (sym == '/') {
			file = 0;
			rank++;
		}
		else {
			if (isdigit(sym)) {
				file += sym - '0';
			}
			else {
				squares[rank * 9 + file] = fenCharToInt[sym];
				file++;
			}
		}
	}

	// modify game turn
	gameTurn = fs[1][0];
}

std::string Board::convertToFEN() {
	std::unordered_map<int, char> fenIntToChar {
	{Red | Advisor, 'a'}, {Black | Advisor, 'A'},
	{Red | Bishop, 'b'}, {Black | Bishop, 'B'},
	{Red | Cannon, 'c'}, {Black | Cannon, 'C'},
	{Red | King, 'q'}, {Black | King, 'Q'},
	{Red | Knight, 'k'}, {Black | Knight, 'K'},
	{Red | Pawn, 'p'}, {Black | Pawn, 'P'},
	{Red | Rook, 'r'}, {Black | Rook, 'R'}
	};

	// chess board
	std::string fen = "";
	for (int i = 0; i < 10; ++i) {
		int acc = 0;
		for (int j = 0; j < 9; ++j) {
			int index = i * 9 + j;
			if (squares[index] == 0) {
				acc++;
			}
			else {
				if (acc != 0) {
					fen += acc + '0';
					acc = 0;
				}
				fen += fenIntToChar[squares[index]];
			}
		}
		if (acc != 0) {
			fen += acc + '0';
		}
		fen += '/';
	}

	// remove redundant '/' at the end
	fen.pop_back();

	// game turn
	fen += Space;
	fen += gameTurn;

	// holding chess
	if (holdingChess.value != None) {
		fen += Space;
		fen += fenIntToChar[holdingChess.value];
	}
	else {
		fen += Space;
		fen += NotAttached;
	}

	return fen;
}
