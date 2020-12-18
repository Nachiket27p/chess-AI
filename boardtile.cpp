#include <QMenu>
#include "utils.h"
#include "boardtile.h"
#include "rules.h"

// Get an instance of the rules class.
Rules *game = game->getInstance();

// Instantiate the private static variables
BoardTile *BoardTile::selectedTile = nullptr;
int BoardTile::selected = 0;

// Access the extern variables which determine the run and the current theme
extern Theme *currentTheme;
extern bool isWhiteTurn;

void BoardTile::setPiece(const char symbol, bool isWhite)
{
    occupied = true;
    this->isWhite = isWhite;
    pieceSymbol = symbol;
    populateTile(symbol, isWhite);
}

void BoardTile::populateTile(const char symbol, bool isWhite)
{
    pieceSymbol = symbol;

    if (occupied)
    {
        switch (symbol)
        {
        case pawnID:
            if (isWhite)
                setPixmap(QPixmap(whitePath + "pawn_white.svg"));
            else
                setPixmap(QPixmap(blackPath + "pawn_black.svg"));
            break;

        case rookID:
            if (isWhite)
                setPixmap(QPixmap(whitePath + "rook_white.svg"));
            else
                setPixmap(QPixmap(blackPath + "rook_black.svg"));
            break;
        case knightID:
            if (isWhite)
                setPixmap(QPixmap(whitePath + "knight_white.svg"));
            else
                setPixmap(QPixmap(blackPath + "knight_black.svg"));
            break;
        case bishopID:
            if (isWhite)
                setPixmap(QPixmap(whitePath + "bishop_white.svg"));
            else
                setPixmap(QPixmap(blackPath + "bishop_black.svg"));
            break;
        case kingID:
            if (isWhite)
                setPixmap(QPixmap(whitePath + "king_white.svg"));
            else
                setPixmap(QPixmap(blackPath + "king_black.svg"));
            break;
        case queenID:
            if (isWhite)
                setPixmap(QPixmap(whitePath + "queen_white.svg"));
            else
                setPixmap(QPixmap(blackPath + "queen_black.svg"));
            break;
        default:
            break;
        }
    }
    else
    {
        clear();
    }
    setAlignment(Qt::AlignCenter);
    displayTileColor();
}

void BoardTile::displayTileColor()
{
    if (isDarkTile)
    {
        if (isWhite)
        {
            setStyleSheet(QString("QLabel {background-color: ") + currentTheme->getDarkBackground() +
                          QString("} :hover {background-color: ") + currentTheme->getHoverBackground() +
                          QString(" color: ") + currentTheme->getHoverColorWhite() + QString("}"));
        }
        else
        {
            setStyleSheet(QString("QLabel {background-color: ") + currentTheme->getDarkBackground() +
                          QString("} :hover {background-color: ") + currentTheme->getHoverBackground() +
                          QString(" color: ") + currentTheme->getHoverColorBlack() + QString("}"));
        }
    }
    else
    {
        if (isWhite)
        {
            setStyleSheet(QString("QLabel {background-color: ") + currentTheme->getLightBackground() +
                          QString("} :hover {background-color: ") + currentTheme->getHoverBackground() +
                          QString(" color: ") + currentTheme->getHoverColorWhite() + QString("}"));
        }
        else
        {
            setStyleSheet(QString("QLabel {background-color: ") + currentTheme->getLightBackground() +
                          QString("} :hover {background-color: ") + currentTheme->getHoverBackground() +
                          QString(" color: ") + currentTheme->getHoverColorBlack() + QString("}"));
        }
    }
}

void BoardTile::removePiece()
{
    occupied = false;
    setText("");
}

void BoardTile::mousePressEvent(QMouseEvent *)
{
    ++selected;
    enforceRules();
}

void BoardTile::unhighlightTiles()
{
    for (int i = 0; i < listIndex; i++)
        board[validMoves[i] / 8][validMoves[i] & 7]->displayTileColor();
}

void BoardTile::enforceRules()
{
    if (selected == 1)
    {
        if (occupied && isWhite == isWhiteTurn)
        {
            if (game->canMove(this))
            {
                selectedTile = this;
                if (isWhite)
                {
                    selectedTile->setStyleSheet(QString("QLabel {background-color:") +
                                                currentTheme->getHoverBackground() + "color: " + currentTheme->getHoverColorWhite() + QString("}"));
                }
                else
                {
                    selectedTile->setStyleSheet(QString("QLabel {background-color:") +
                                                currentTheme->getHoverBackground() + "color: " + currentTheme->getHoverColorBlack() + QString("}"));
                }
            }
            else
            {
                selected = 0;
            }
        }
        else
        {
            selected = 0;
        }
    }
    else
    {
        if (tileNumber == selectedTile->getTileNumber())
        {
            selectedTile->displayTileColor();
            unhighlightTiles();
            listIndex = 0;
            selected = 0;
        }

        for (int i = 0; i < listIndex; i++)
        {
            if (tileNumber == validMoves[i])
            {
                setPiece(selectedTile->getPieceSymbol(), selectedTile->getPieceColor());
                selectedTile->removePiece();
                selectedTile->populateTile(selectedTile->getPieceSymbol(), selectedTile->getPieceColor());
                populateTile(selectedTile->getPieceSymbol(), selectedTile->getPieceColor());

                unhighlightTiles();
                listIndex = 0;
                isWhiteTurn = (isWhiteTurn + 1) & 1;
                selected = 0;
            }
            else
            {
                selected = 1;
            }
        }
    }
}
