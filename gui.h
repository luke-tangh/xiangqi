#pragma once
#include <iostream>
#include <vector>
#include <graphics.h>

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
	int holdChess;
	int lastMove;
	int gameTurn;
	std::vector<int> validMoves;

	void clearHoldChess();
	void clearValidMoves();
	void drawValidMoves();
	void drawPreviousPosition();
	void drawNextTurnInfo(char turn);
	void drawOnPosition(int chess, int x, int y);

 public:
	Gui();
	void loadAssets();
	void setCursorPosition(int x, int y);
	void setHoldChess(int chess);
	void setLastMove(int index);
	void setValidMoves(std::vector<int> moves);
	bool isValidMove(int index);
	void nextRound(int turn);
	void drawFromBoard(std::vector<int> squares);
	int getChessClick(int x, int y);
	void exitGui();
};
