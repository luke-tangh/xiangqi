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
	int curX, curY;
	std::vector<int> validMoves;
 public:
	Gui();
	void loadAssets();
	void setCursorPosition(int x, int y);
	void drawOnPosition(char sym, int x, int y);
	void drawValidMoves();
	void drawFromFEN(std::string fen);
	int getChessClick(int x, int y);
	void clearValidMoves();
	void setValidMoves(std::vector<int> moves);
	bool isValidMove(int index);
	void exitGui();
};
