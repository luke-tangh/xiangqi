#pragma once
#include "utility.h"
#include "xiangqi.h"
#include <conio.h>
#include <graphics.h>
#include <iostream>
#include <vector>

class Gui {
private:
	IMAGE chessBoard;
	IMAGE redAdvisor, blackAdvisor;
	IMAGE redBishop, blackBishop;
	IMAGE redCannon, blackCannon;
	IMAGE redKing, blackKing;
	IMAGE redKnight, blackKnight;
	IMAGE redPawn, blackPawn;
	IMAGE redRook, blackRook;
	RECT sidebar;
	RECT gameTurnBar;

	int curX, curY;
	Board* pB;

	void drawValidMoves();
	void drawPreviousPosition();
	void drawNextTurnInfo(char turn);
	void drawOnPosition(int chess, int x, int y);

 public:
	Gui(Board* pBoard);
	void loadAssets();
	void setCursorPosition(int x, int y);
	void drawFromBoard();
	int getChessClick(int x, int y);
	void exitGui();
};
