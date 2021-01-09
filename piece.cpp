#include "piece.h"

Piece::Piece(bool _white, char _pieceSymbol, int _index, int _row, int _col, QString iconPath)
{
    moved = 0;
    captured = false;
    white = _white;
    pieceSymbol = _pieceSymbol;
    index = _index;
    row = _row;
    col = _col;
    icon = new QPixmap(iconPath);
}

int Piece::hasMoved()
{
    return moved;
}

void Piece::setMoved()
{
    moved++;
}

bool Piece::isCaptured()
{
    return captured;
}

void Piece::setCaptured()
{
    captured = true;
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

int Piece::getIndex()
{
    return index;
}

void Piece::resetPiece(int _row, int _col)
{
    if (_row != -1)
        row = _row;

    if (_col != -1)
        col = _col;

    captured = false;
    moved = 0;
}
