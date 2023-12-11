#pragma once
#include <graphics.h>
#include <vector>
#include <string>

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
public:
	Gui();
	void loadAssets();
	void drawOnPosition(char sym, int x, int y);
	void drawFromFEN(std::string fen, int x, int y);
	int getChessClick(int x, int y);
	void exitGui();
};
