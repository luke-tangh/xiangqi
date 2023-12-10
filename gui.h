#pragma once
#include <graphics.h>
#include <string>

const int BOARD_HEIGHT = 1000;
const int BOARD_WIDTH = 900;
const int CHESS_SIZE = 100;

void putAlphaImage(int picX, int picY, IMAGE* picture);

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
	void initBoardDisplay();
	void drawOnPosition(char sym, int x, int y);
	void drawFromFEN(std::string fen);
	void exitGui();
};
