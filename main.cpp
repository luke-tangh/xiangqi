#include <iostream>
#include "xiangqi.h"
#include "gui.h"

int main() {
    std::unique_ptr<Board> pB(new Board);
    std::unique_ptr<Gui> pG(new Gui);

    pG->initBoardDisplay();
    pG->drawFromFEN(START_FEN);
    pB->readFromFEN(START_FEN);
    
	ExMessage m;
    while (true) {
        m = getmessage(EX_MOUSE | EX_KEY);
        if (m.message == WM_KEYDOWN) break;
    }

    pG->exitGui();

    return 0;
} 
