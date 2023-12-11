#include <iostream>
#include "xiangqi.h"
#include "utility.h"
#include "gui.h"

int main() {
    std::unique_ptr<Board> pB(new Board);
    std::unique_ptr<Gui> pG(new Gui);

    pG->drawFromFEN(START_FEN, 0, 0);
    pB->readFromFEN(START_FEN);
    pB->initChessPosMap();
    
    std::cout << pB->convertToFEN() << std::endl;

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
        case WM_LBUTTONDOWN:
            chessClicked = pG->getChessClick(m.x, m.y);
            if (chessClicked == NotClicked) break;
            if (chessAttached) {
                hold = pB->getHoldingChess();
                pB->clearHoldingChess();
                pB->setChessPos(chessClicked, hold.value);
                pB->reverseGameTurn();
                pG->drawFromFEN(pB->convertToFEN(), 0, 0);
                chessAttached = false;
            }
            else {
                pB->setHoldingChess(chessClicked);
                pB->setChessPos(chessClicked, None);
                pG->drawFromFEN(pB->convertToFEN(), m.x - MARGIN, m.y - MARGIN);
                chessAttached = true;
            }
            std::cout << pB->convertToFEN() << std::endl;
            break;
        case WM_MOUSEMOVE:
            if (not chessAttached) break;
            pG->drawFromFEN(pB->convertToFEN(), m.x - MARGIN, m.y - MARGIN);
            break;
        }
    }

    pG->exitGui();

    return 0;
} 
