#include "agent.h"
#include "gui.h"
#include "bitmap.h"
#include "utility.h"
#include "xiangqi.h"
#include <iostream>

int main() {
    BitMap* pM(new BitMap());
    Board* pB(new Board(pM));
    Gui* pG(new Gui(pB));
    Agent* pA(new Agent(pM, pB));

    pB->readFromFEN(START_FEN);
    pG->drawFromBoard();

	ExMessage m;
    bool exit = false;
    bool chessAttached = false;
    int chessClicked = -1;

    while (not exit) {
        m = getmessage(EX_MOUSE | EX_KEY);
        switch (m.message) {
        case WM_KEYDOWN: 
            if (m.vkcode == VK_ESCAPE) { exit = true; }
            break;
        // drop or select a chess
        case WM_LBUTTONDOWN:
            chessClicked = pG->getChessClick(m.x, m.y);
            if (chessClicked == -1) break;
            // drop a chess
            if (chessAttached) {
                if (!pB->isValidMove(chessClicked)) break;
                pB->makeMove((pB->holdChessPos << 8) + chessClicked);
                pG->drawFromBoard();
                chessAttached = false;

                // agent move
                if (AGENT_ON) {
                    pA->resetAgentMove();
                    pA->search(SEARCH_DEPTH, -INT_MAX, INT_MAX);
                    if (pA->agentMove != 0) {
                        pB->makeMove(pA->agentMove);
                    }
                    else {
                        std::cout << "no move available" << std::endl;
                    }
                    pG->drawFromBoard();
                }
            }
            // select a chess
            else {
                if (!pB->isTurnToMove(chessClicked)) break;
                pG->setCursorPosition(m.x - MARGIN, m.y - MARGIN);
                // get valid moves
                pB->legalMoveGeneration(chessClicked, pB->squares[chessClicked]);
                pB->pickUpChess(chessClicked);
                pG->drawFromBoard();
                chessAttached = true;
            }
            break;
        // trace mouse move
        case WM_MOUSEMOVE:
            if (not chessAttached) break;
            pG->setCursorPosition(m.x - MARGIN, m.y - MARGIN);
            pG->drawFromBoard();
            break;
        // cancel move
        case WM_RBUTTONDOWN:
            if (!chessAttached) break;
            pB->dropChess();
            pG->drawFromBoard();
            chessAttached = false;
            break;
        }
    }

    pG->exitGui();

    delete pG;
    delete pB;

    return 0;
} 
