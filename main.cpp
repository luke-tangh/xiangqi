#include <iostream>
#include "xiangqi.h"
#include "utility.h"
#include "gui.h"

int main() {
    std::unique_ptr<Board> pB(new Board);
    std::unique_ptr<Gui> pG(new Gui);

    pG->drawFromFEN(START_FEN);
    pB->readFromFEN(START_FEN);
    pB->initChessPosMap();

	ExMessage m;
    bool exit = false;
    bool chessAttached = false;
    int chessClicked = NotClicked;
    Chess hold;

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
                hold = pB->getHoldingChess();
                pB->clearHoldingChess();
                pB->setChessPos(chessClicked, hold.value);
                pB->reverseGameTurn();
                pG->setChessPreviousPos(hold.index);
                pG->clearValidMoves();

                // std::cout << "is red in check? " << pB->isKingInCheck(Red | King) << std::endl;
                // std::cout << "is black in check? " << pB->isKingInCheck(Black | King) << std::endl;
                
                pG->drawFromFEN(pB->convertToFEN());
                chessAttached = false;
            }
            // select a chess
            else {
                // turn check
                if (!pB->turnCheck(chessClicked)) break;
                pB->setHoldingChess(chessClicked);
                pB->setChessPos(chessClicked, None);
                pG->setCursorPosition(m.x - MARGIN, m.y - MARGIN);
                // get valid moves
                hold = pB->getHoldingChess();
                pG->setValidMoves(pB->legalMoveGeneration(hold.index, hold.value));
                pG->drawFromFEN(pB->convertToFEN());
                chessAttached = true;
            }
            break;
        // trace mouse move
        case WM_MOUSEMOVE:
            if (not chessAttached) break;
            pG->setCursorPosition(m.x - MARGIN, m.y - MARGIN);
            pG->drawFromFEN(pB->convertToFEN());
            break;
        // cancel move
        case WM_RBUTTONDOWN:
            if (!chessAttached) break;
            hold = pB->getHoldingChess();
            pB->clearHoldingChess();
            pB->setChessPos(hold.index, hold.value);
            pG->clearValidMoves();
            pG->drawFromFEN(pB->convertToFEN());
            chessAttached = false;
            break;
        }
    }

    pG->exitGui();

    return 0;
} 
