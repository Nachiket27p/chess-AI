#include <QApplication>
#include "board.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Board b;
    b.show();
    b.checkResources();

    return a.exec();
}
