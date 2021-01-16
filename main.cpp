#include <QApplication>
#include "board.h"
#include "debugwindow.h"

DebugWindow *dbw;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    dbw = new DebugWindow();
    // DebugWindow db;
    dbw->show();

    Board b;

    b.show();
    b.checkResources();

    return a.exec();
}
