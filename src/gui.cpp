#include "gui.h"

Gui::Gui(Board* pBoard) {
	pB = pBoard;
	
	curX = 0;
	curY = 0;

	sidebar = { 900, 0, 999, 999 };
	gameTurnBar = { 900, 0, 999, 99 };
	
	initgraph(1000, 1000);
	setbkcolor(RGB(255, 255, 255));
	
	settextcolor(BLACK);
	settextstyle(20, 0, _T("Consolas"));

	cleardevice();
	loadAssets();
	putimage(0, 0, &chessBoard);
}

void Gui::loadAssets() {
	loadimage(&chessBoard, _T("assets/xiangqi_gmchess_wood.png"), BOARD_WIDTH, BOARD_HEIGHT);
	loadimage(&redAdvisor, _T("assets/red_advisor.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&redBishop, _T("assets/red_bishop.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&redCannon, _T("assets/red_cannon.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&redKing, _T("assets/red_king.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&redKnight, _T("assets/red_knight.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&redPawn, _T("assets/red_pawn.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&redRook, _T("assets/red_rook.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackAdvisor, _T("assets/black_advisor.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackBishop, _T("assets/black_bishop.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackCannon, _T("assets/black_cannon.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackKing, _T("assets/black_king.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackKnight, _T("assets/black_knight.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackPawn, _T("assets/black_pawn.png"), CHESS_SIZE, CHESS_SIZE);
	loadimage(&blackRook, _T("assets/black_rook.png"), CHESS_SIZE, CHESS_SIZE);
}

void Gui::setCursorPosition(int x, int y) {
	curX = x;
	curY = y;
}

void Gui::drawOnPosition(int chess, int x, int y) {
	x = max(0, min(x, BOARD_WIDTH - CHESS_SIZE));
	y = max(0, min(y, BOARD_HEIGHT - CHESS_SIZE));

	switch (chess) {
	case Black | Advisor : putAlphaImage(x, y, &blackAdvisor); break;
	case Black | Bishop  : putAlphaImage(x, y, &blackBishop);  break;
	case Black | Cannon  : putAlphaImage(x, y, &blackCannon);  break;
	case Black | King    : putAlphaImage(x, y, &blackKing);    break;
	case Black | Knight  : putAlphaImage(x, y, &blackKnight);  break;
	case Black | Pawn    : putAlphaImage(x, y, &blackPawn);    break;
	case Black | Rook    : putAlphaImage(x, y, &blackRook);    break;
	case Red   | Advisor : putAlphaImage(x, y, &redAdvisor);   break;
	case Red   | Bishop  : putAlphaImage(x, y, &redBishop);    break;
	case Red   | Cannon  : putAlphaImage(x, y, &redCannon);    break;
	case Red   | King    : putAlphaImage(x, y, &redKing);      break;
	case Red   | Knight  : putAlphaImage(x, y, &redKnight);    break;
	case Red   | Pawn    : putAlphaImage(x, y, &redPawn);      break;
	case Red   | Rook    : putAlphaImage(x, y, &redRook);      break;
	}
}

void Gui::drawValidMoves() {
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	for (Move m : pB->validMoves) {
		int to = m & GetMove;
		int x = (to % 16 - 3) * CHESS_SIZE + MARGIN;
		int y = (to / 16 - 3) * CHESS_SIZE + MARGIN;
		if (pB->squares[to] != None) {
			setfillcolor(LIGHTBLUE);
		}
		else {
			setfillcolor(YELLOW);
		}
		fillcircle(x, y, 10);
	}
}

void Gui::drawNextTurnInfo(char turn) {
	if (turn == Red) {
		TCHAR gameTurnInfo[] = _T("Next:\nRed");
		drawtext(gameTurnInfo, &gameTurnBar, DT_CENTER);
	}
	else {
		TCHAR gameTurnInfo[] = _T("Next:\nBlack");
		drawtext(gameTurnInfo, &gameTurnBar, DT_CENTER);
	}
}

void Gui::drawPreviousPosition() {
	setlinecolor(RED);
	setlinestyle(PS_SOLID, 3);
	int x = (pB->lastMove % 16 - 3) * CHESS_SIZE;
	int y = (pB->lastMove / 16 - 3) * CHESS_SIZE;
	
	// left up
	x += GAP;
	y += GAP;
	line(x, y, x + 10, y);
	line(x, y, x, y + 10);

	// right up
	x -= 2 * GAP;
	line(x + 100, y, x + 90, y);
	line(x + 100, y, x + 100, y + 10);
	
	// right down
	y -= 2 * GAP;
	line(x + 100, y + 100, x + 90, y + 100);
	line(x + 100, y + 100, x + 100, y + 90);
	
	// left down
	x += 2 * GAP;
	line(x, y + 100, x, y + 90);
	line(x, y + 100, x + 10, y + 100);
}

void Gui::drawFromBoard() {
	BeginBatchDraw();
	putimage(0, 0, &chessBoard);

	for (int i = 51; i < 204; ++i) {
		int file = i % 16;
		int rank = i / 16;
		drawOnPosition(pB->squares[i], (file - 3) * 100, (rank - 3) * 100);
	}
	
	// draw valid moves
	if (!pB->validMoves.empty()) {
		drawValidMoves();
	}

	// holding chess
	if (pB->holdChessVal != -1) {
		drawOnPosition(pB->holdChessVal, curX, curY);
	}

	// chess previous position
	if (pB->lastMove != -1) {
		drawPreviousPosition();
	}

	// draw information sidebar
	drawNextTurnInfo(pB->gameTurn);

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

	int chessClicked = -1;
	for (std::vector<int> xys : nearest) {
		double dist = sqrt(pow(x - xys[0], 2) + pow(y - xys[1], 2));
		if (dist < CHESS_RAD) {
			chessClicked = (xys[0] / CHESS_SIZE + 3) + 16 * (xys[1] / CHESS_SIZE + 3);
			return chessClicked;
		}
	}
	return chessClicked;
}

void Gui::exitGui() {
	EndBatchDraw();
	closegraph();
}
