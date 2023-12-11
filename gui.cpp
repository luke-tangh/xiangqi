#include <iostream>
#include <sstream>
#include <vector>
#include <graphics.h>
#include <conio.h>
#include "utility.h"
#include "gui.h"

Gui::Gui() {
	curX = 0;
	curY = 0;
	initgraph(900, 1000);
	setbkcolor(RGB(255, 255, 255));
	cleardevice();
	loadAssets();
	putimage(0, 0, &chessBoard);
}

void Gui::loadAssets() {
	loadimage(&chessBoard, _T("./assets/xiangqi_gmchess_wood.png"), BOARD_WIDTH, BOARD_HEIGHT);
	loadimage(&redAdvisor, _T("./assets/red_advisor.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&redBishop, _T("./assets/red_bishop.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&redCannon, _T("./assets/red_cannon.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&redKing, _T("./assets/red_king.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&redKnight, _T("./assets/red_knight.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&redPawn, _T("./assets/red_pawn.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&redRook, _T("./assets/red_rook.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackAdvisor, _T("./assets/black_advisor.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackBishop, _T("./assets/black_bishop.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackCannon, _T("./assets/black_cannon.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackKing, _T("./assets/black_king.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackKnight, _T("./assets/black_knight.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackPawn, _T("./assets/black_pawn.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackRook, _T("./assets/black_rook.png"), CHESS_SIZE, CHESS_SIZE);
}

void Gui::setCursorPosition(int x, int y) {
	curX = x;
	curY = y;
}

void Gui::drawOnPosition(char sym, int x, int y) {
	x = max(0, min(x, BOARD_WIDTH - CHESS_SIZE));
	y = max(0, min(y, BOARD_HEIGHT - CHESS_SIZE));

	switch (sym) {
	case 'a': putAlphaImage(x, y, &blackAdvisor); break;
	case 'b': putAlphaImage(x, y, &blackBishop); break;
	case 'c': putAlphaImage(x, y, &blackCannon); break;
	case 'q': putAlphaImage(x, y, &blackKing); break;
	case 'k': putAlphaImage(x, y, &blackKnight); break;
	case 'p': putAlphaImage(x, y, &blackPawn); break;
	case 'r': putAlphaImage(x, y, &blackRook); break;
	case 'A': putAlphaImage(x, y, &redAdvisor); break;
	case 'B': putAlphaImage(x, y, &redBishop); break;
	case 'C': putAlphaImage(x, y, &redCannon); break;
	case 'Q': putAlphaImage(x, y, &redKing); break;
	case 'K': putAlphaImage(x, y, &redKnight); break;
	case 'P': putAlphaImage(x, y, &redPawn); break;
	case 'R': putAlphaImage(x, y, &redRook); break;
	}
}

void Gui::drawValidMoves() {
	for (int index : validMoves) {
		// Attacking moves are added 128
		if (index >= AttackingMove) {
			setfillcolor(LIGHTBLUE);
			index = index & (AttackingMove - 1);
		}
		else {
			setfillcolor(YELLOW);
		}
		int x = (index % 9) * CHESS_SIZE + MARGIN;
		int y = (index / 9) * CHESS_SIZE + MARGIN;
		fillcircle(x, y, 10);
	}
}

void Gui::drawFromFEN(std::string fen) {
	BeginBatchDraw();
	putimage(0, 0, &chessBoard);

	std::vector<std::string> fs = split(fen, Space);

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
				drawOnPosition(sym, file * 100, rank * 100);
				file++;
			}
		}
	}

	// draw valid moves
	if (!validMoves.empty()) {
		drawValidMoves();
	}

	// holding chess
	char sym = fs[2][0];
	if (sym != NotAttached) {
		drawOnPosition(sym, curX, curY);
	}

	FlushBatchDraw();
	cleardevice();
}

/*
*  get chess position clicked
*  |  x  | + 1 |
*  | + 9 | +10 |
*/
int Gui::getChessClick(int x, int y) {
	int xPos = x / CHESS_SIZE;
	int yPos = y / CHESS_SIZE;

	std::vector<std::vector<int>> nearest = {
		{xPos * CHESS_SIZE + MARGIN, yPos * CHESS_SIZE + MARGIN},
		{(xPos + 1) * CHESS_SIZE + MARGIN, yPos * CHESS_SIZE + MARGIN},
		{xPos * CHESS_SIZE + MARGIN, (yPos + 1) * CHESS_SIZE + MARGIN},
		{(xPos + 1) * CHESS_SIZE + MARGIN, (yPos + 1) * CHESS_SIZE + MARGIN}
	};

	int chessClicked = NotClicked;
	for (std::vector<int> xys : nearest) {
		double dist = sqrt(pow(x - xys[0], 2) + pow(y - xys[1], 2));
		if (dist < CHESS_RAD) {
			chessClicked = (xys[0] / CHESS_SIZE) + 9 * (xys[1] / CHESS_SIZE);
			return chessClicked;
		}
	}
	return chessClicked;
}

void Gui::clearValidMoves() {
	validMoves.clear();
}

void Gui::setValidMoves(std::vector<int> moves) {
	validMoves = moves;
}

bool Gui::isValidMove(int index) {
	// check normal moves
	if (std::count(validMoves.begin(), validMoves.end(), index) > 0) {
		return true;
	}
	// check attacking moves
	if (std::count(validMoves.begin(), validMoves.end(), index + AttackingMove) > 0) {
		return true;
	}
	return false;
}

void Gui::exitGui() {
	EndBatchDraw();
	closegraph();
}
