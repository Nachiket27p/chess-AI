#include <QMenu>
#include "utils.h"
#include "boardtile.h"
#include "rules.h"

// Get an instance of the rules class.
Rules *game;

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
    // Get instance of the rules
    game = game->getInstance();
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
    for (int i = 0; i < vmIdx; i++)
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
        // if the same tile is clicked twice do unhighlight the possible moves
        if (tileNumber == selectedTile->getTileNumber())
        {
            selectedTile->displayTileColor();
            unhighlightTiles();
            vmIdx = 0;
            selected = 0;
            return;
        }

        // if the piece being moved is the the king the update the king pointer
        if(selectedTile->getPieceSymbol() == kingID)
        {
            game->setKingPos(selectedTile->getPieceColor(), this);
        }


        // perform the operations to move the piece
        bool moveSuccess = false;
        for (int i = 0; i < vmIdx; i++)
        {
            if (tileNumber == validMoves[i])
            {
                setPiece(selectedTile->getPieceSymbol(), selectedTile->getPieceColor());
                selectedTile->removePiece();
                selectedTile->populateTile(selectedTile->getPieceSymbol(), selectedTile->getPieceColor());
                populateTile(selectedTile->getPieceSymbol(), selectedTile->getPieceColor());

                unhighlightTiles();
                vmIdx = 0;
                selected = 0;
                isWhiteTurn = (isWhiteTurn + 1) & 1;
                moveSuccess = true;
                break;
            }
        }



        // If the move was not valid/successful then set the selected value back to 1
        // to indicate that game state has returned to a position where the player has
        // selected a piece.
        if(!moveSuccess)
        {
            selected = 1;
        }

        // scan for check after a piece has moved
        game->scanForCheck();
    }
}
