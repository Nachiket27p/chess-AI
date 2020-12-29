#include "piece.h"

Piece::Piece(bool _white, char _pieceSymbol, int _index, int _row, int _col, QString iconPath)
{
    moved = false;
    white = _white;
    pieceSymbol = _pieceSymbol;
    index = _index;
    row = _row;
    col = _col;
    icon = new QPixmap(iconPath);
}

bool Piece::hasMoved()
{
    return moved;
}

void Piece::setMoved()
{
    moved = true;
}

int Piece::getRow()
{
    return row;
}

int Piece::getCol()
{
    return col;
}

bool Piece::isWhite()
{
    return white;
}

char Piece::getPieceSymbol()
{
    return pieceSymbol;
}

QPixmap *Piece::getIcon()
{
    return icon;
}

void Piece::updatePosition(int _row, int _col)
{
    row = _row;
    col = _col;
}
