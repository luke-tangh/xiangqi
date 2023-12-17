#include <iostream>
#include "xiangqi.h"
#include "utility.h"
#include "gui.h"

int main() {
    std::unique_ptr<Board> pB(new Board);
    std::unique_ptr<Gui> pG(new Gui);

    pB->readFromFEN(START_FEN);
    pB->initChessPosMap();
    pG->drawFromBoard(pB->getSquares());

	ExMessage m;
    bool exit = false;
    bool chessAttached = false;
    int chessClicked = NotClicked;
    int holdChessVal = -1;
    int holdChessPos = -1;
    char gameTurn = Red;

    while (not exit) {
        m = getmessage(EX_MOUSE | EX_KEY);
        switch (m.message) {
        case WM_KEYDOWN: 
            if (m.vkcode == VK_ESCAPE) { exit = true; }
            break;
        // drop or select a chess
        case WM_LBUTTONDOWN:
            chessClicked = pG->getChessClick(m.x, m.y);
            if (chessClicked == NotClicked) break;
            // drop a chess
            if (chessAttached) {
                if (!pG->isValidMove(chessClicked)) break;
                pB->setChessPos(chessClicked, holdChessVal);
                // reverse game turn
                gameTurn ^= GetColour;
                pG->setLastMove(holdChessPos);
                pG->nextRound(gameTurn);
                pG->drawFromBoard(pB->getSquares());
                chessAttached = false;
            }
            // select a chess
            else {
                if (gameTurn != (pB->getChessOnPos(chessClicked) & GetColour)) break;
                // update holding chess info
                holdChessVal = pB->getChessOnPos(chessClicked);
                holdChessPos = chessClicked;
                pB->setChessPos(chessClicked, None);
                pG->setHoldChess(holdChessVal);
                pG->setCursorPosition(m.x - MARGIN, m.y - MARGIN);
                // get valid moves
                pG->setValidMoves(pB->legalMoveGeneration(holdChessPos, holdChessVal));
                pG->drawFromBoard(pB->getSquares());
                chessAttached = true;
            }
            break;
        // trace mouse move
        case WM_MOUSEMOVE:
            if (not chessAttached) break;
            pG->setCursorPosition(m.x - MARGIN, m.y - MARGIN);
            pG->drawFromBoard(pB->getSquares());
            break;
        // cancel move
        case WM_RBUTTONDOWN:
            if (!chessAttached) break;
            pB->setChessPos(holdChessPos, holdChessVal);
            pG->nextRound(gameTurn);
            pG->drawFromBoard(pB->getSquares());
            chessAttached = false;
            break;
        }
    }

    pG->exitGui();

    return 0;
} 
