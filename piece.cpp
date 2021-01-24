#include "piece.h"
#include "utils.h"

extern std::unordered_map<int, QPixmap *> pieceIconPointers;

Piece::Piece(bool _white, char _pieceSymbol, uint _tileNumber, uint _row, uint _col, uint _index, int iconMapKey)
{
    moved = 0;
    captured = false;
    white = _white;
    pieceSymbol = _pieceSymbol;
    tileNumber = _tileNumber;
    row = _row;
    col = _col;
    basePowerValue = basePowerValues.at(pieceSymbol);
    index = _index;
    icon = pieceIconPointers.at(iconMapKey);
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
    this->icon = p.icon;
}

uint Piece::hasMoved()
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

uint Piece::getRow()
{
    return row;
}

uint Piece::getCol()
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

uint Piece::getBasePowerValue()
{
    return basePowerValue;
}

uint Piece::getIndex()
{
    return index;
}

void Piece::updatePosition(uint _row, uint _col, uint _tileNumber)
{
    row = _row;
    col = _col;
    tileNumber = _tileNumber;
}

uint Piece::getTileNumber()
{
    return tileNumber;
}

void Piece::resetPiece(uint _row, uint _col)
{
    row = _row;
    col = _col;

    captured = false;
    moved = 0;
}
