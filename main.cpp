#include <QApplication>
#include "utils.h"
#include "board.h"

#ifdef DEBUGGING_WINDOW
#include "debugwindow.h"
DebugWindow *dbw;
#endif

// map which keeps track of the pointers to all the QPixmap objects
std::unordered_map<int, QPixmap *> pieceIconPointers;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef DEBUGGING_WINDOW
    dbw = new DebugWindow();
    // DebugWindow db;
    dbw->show();
#endif

    // initialize the QPixmap pointers
    pieceIconPointers.emplace(pawnIDW, new QPixmap(whitePath + pawnIconName));
    pieceIconPointers.emplace(rookIDW, new QPixmap(whitePath + rookIconName));
    pieceIconPointers.emplace(knightIDW, new QPixmap(whitePath + knightIconName));
    pieceIconPointers.emplace(bishopIDW, new QPixmap(whitePath + bishopIconName));
    pieceIconPointers.emplace(queenIDW, new QPixmap(whitePath + queenIconName));
    pieceIconPointers.emplace(kingIDW, new QPixmap(whitePath + kingIconName));

    pieceIconPointers.emplace(pawnIDB, new QPixmap(blackPath + pawnIconName));
    pieceIconPointers.emplace(rookIDB, new QPixmap(blackPath + rookIconName));
    pieceIconPointers.emplace(knightIDB, new QPixmap(blackPath + knightIconName));
    pieceIconPointers.emplace(bishopIDB, new QPixmap(blackPath + bishopIconName));
    pieceIconPointers.emplace(queenIDB, new QPixmap(blackPath + queenIconName));
    pieceIconPointers.emplace(kingIDB, new QPixmap(blackPath + kingIconName));

    Board b;

    b.show();
    b.checkResources();

    return a.exec();
}
