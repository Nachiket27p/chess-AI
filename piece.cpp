#include "piece.h"
#include "utils.h"

Piece::Piece(bool _white, char _pieceSymbol, int _tileNumber, int _row, int _col, int _index, QString iconPath)
{
    moved = 0;
    captured = false;
    white = _white;
    pieceSymbol = _pieceSymbol;
    tileNumber = _tileNumber;
    row = _row;
    col = _col;
    basePowerValue = basePowerValues[_pieceSymbol];
    index = _index;
    icon = new QPixmap(iconPath);
}

Piece::Piece(const Piece &p)
{
    this->moved = p.moved;
    this->white = p.white;
    this->pieceSymbol = p.pieceSymbol;
    this->tileNumber = p.tileNumber;
    this->row = p.row;
    this->col = p.col;
    this->captured = p.captured;
    this->index = p.index;
    this->basePowerValue = p.basePowerValue;

    // determine correct path
    QString subPath = blackPath;
    if (this->white)
        subPath = whitePath;

    QString iconImgName = pawnIconName;
    switch (this->pieceSymbol)
    {
    case rookID:
        iconImgName = rookIconName;
        break;
    case knightID:
        iconImgName = knightIconName;
        break;
    case bishopID:
        iconImgName = bishopIconName;
        break;
    case queenID:
        iconImgName = queenIconName;
        break;
    case kingID:
        iconImgName = kingIconName;
        break;
    };

    this->icon = new QPixmap(subPath + iconImgName);
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

int Piece::getBasePowerValue()
{
    return basePowerValue;
}

int Piece::getIndex()
{
    return index;
}

void Piece::updatePosition(int _row, int _col, int _tileNumber)
{
    row = _row;
    col = _col;
    tileNumber = _tileNumber;
}

int Piece::getTileNumber()
{
    return tileNumber;
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
