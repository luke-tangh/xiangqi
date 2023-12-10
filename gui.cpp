#include <graphics.h>
#include <conio.h>
#include "gui.h"

// put transparent image in EasyX
// credit: csdn@weixin_45779485
void putAlphaImage(int picX, int picY, IMAGE* picture) {
	DWORD* dst = GetImageBuffer();
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture);
	int picture_width = picture->getwidth();
	int picture_height = picture->getheight();
	int graphWidth = getwidth();
	int graphHeight = getheight();
	int dstX = 0;

	for (int iy = 0; iy < picture_height; iy++) {
		for (int ix = 0; ix < picture_width; ix++) {
			int srcX = ix + iy * picture_width;
			int sa = ((src[srcX] & 0xff000000) >> 24);
			int sr = ((src[srcX] & 0xff0000) >> 16);
			int sg = ((src[srcX] & 0xff00) >> 8);
			int sb = src[srcX] & 0xff;
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight) {
				dstX = (ix + picX) + (iy + picY) * graphWidth;
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
					| (sb * sa / 255 + db * (255 - sa) / 255);
			}
		}
	}
}

Gui::Gui() {
	initgraph(900, 1000);
	setbkcolor(RGB(255, 255, 255));
	cleardevice();
	loadAssets();
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

void Gui::initBoardDisplay() {
	putimage(0, 0, &chessBoard);
}

void Gui::drawOnPosition(char sym, int x, int y) {
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

void Gui::drawFromFEN(std::string fen) {
	int file = 0, rank = 0;
	for (char sym : fen) {
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
}

void Gui::exitGui() {
	closegraph();
}
