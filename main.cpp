#include <QApplication>
#include "board.h"
#include "debugwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DebugWindow db;
    db.show();

    Board b;
    b.setDebugWindowAccess(&db);
    b.show();
    b.checkResources();



    return a.exec();
}
