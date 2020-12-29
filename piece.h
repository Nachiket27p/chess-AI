#ifndef PIECE_H
#define PIECE_H

#include <QPixmap>

class Piece
{
private:
    bool moved;
    bool white;
    char pieceSymbol;
    int index;
    int row;
    int col;
    QPixmap *icon;

public:
    Piece(bool _white, char _pieceSymbol, int _index, int _row, int _col, QString iconPath);

    bool hasMoved();
    void setMoved();
    int getRow();
    int getCol();
    bool isWhite();
    char getPieceSymbol();
    QPixmap *getIcon();
    void updatePosition(int _row, int _col);
};

#endif // PIECE_H
