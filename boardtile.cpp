#include <QMenu>
#include "utils.h"
#include "boardtile.h"
#include "rules.h"

// Get an instance of the rules class.
extern Rules *game;

// Instantiate the private static variables
BoardTile *BoardTile::selectedTile = nullptr;
int BoardTile::selected = 0;

// Access the extern variables which determine the run and the current theme
extern Theme *currentTheme;
extern bool isWhiteTurn;

void BoardTile::setPiece(Piece *piece)
{
    this->piece = piece;
    piece->updatePosition(row, col);
}

void BoardTile::displayTile()
{
    if (piece != nullptr)
    {
        setPixmap(*piece->getIcon());
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

        setStyleSheet(QString("QLabel {background-color: ") + currentTheme->getDarkBackground() +
                      QString("} :hover {background-color: ") + currentTheme->getHoverBackground() +
                      QString(" color: ") + currentTheme->getHoverColorWhite() + QString("}"));
    }
    else
    {
        setStyleSheet(QString("QLabel {background-color: ") + currentTheme->getLightBackground() +
                      QString("} :hover {background-color: ") + currentTheme->getHoverBackground() +
                      QString(" color: ") + currentTheme->getHoverColorWhite() + QString("}"));
    }
}

void BoardTile::removePiece()
{
    piece = nullptr;
}

char BoardTile::getPieceSymbol()
{
    if (piece != nullptr)
    {
        return piece->getPieceSymbol();
    }
    return '\0';
}

bool BoardTile::getPieceColor()
{
    if (piece != nullptr)
    {
        return piece->isWhite();
    }
    return false;
}

bool BoardTile::isOccupied()
{
    if (piece != nullptr)
    {
        return true;
    }
    return false;
}

void BoardTile::mousePressEvent(QMouseEvent *)
{
    ++selected;
    enforceRules();
}

bool BoardTile::hasMoved()
{
    if (piece != nullptr)
    {
        return piece->hasMoved();
    }
    return false;
}

void BoardTile::setMoved()
{
    piece->setMoved();
}

void BoardTile::unhighlightTiles()
{
    for (int i = 0; i < vmIdx; i++)
        grid[validMoves[i] / 8][validMoves[i] & 7]->displayTileColor();
}

void BoardTile::enforceRules()
{
    if (selected == 1)
    {
        // TODO: Need to find a way to reduce the number of positions
        //          the king can move based on miving to a position will yeild
        //          another check.
        //        **Perhaps consider using a grid of 8x8 which holds positive
        //              values to indicate that moving to a specific tile will
        //              move it into a check position.
        if ((piece != nullptr) && (piece->isWhite() == isWhiteTurn))
        {
            if (game->canMove(this))
            {
                selectedTile = this;
                if (piece->isWhite())
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

        int kingTNBefore;
        // if the piece being moved is the the king the update the king pointer
        if (selectedTile->getPieceSymbol() == kingID)
        {
            game->setKingPos(selectedTile->getPieceColor(), this);
            kingTNBefore = selectedTile->getTileNumber();
        }

        // perform the operations to move the piece
        bool moveSuccess = false;
        for (int i = 0; i < vmIdx; i++)
        {
            if (tileNumber == validMoves[i])
            {
                setPiece(selectedTile->getPiece());
                selectedTile->removePiece();
                selectedTile->displayTile();
                // mark the piece as being moved
                setMoved();
                displayTile();
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
        if (!moveSuccess)
        {
            selected = 1;
            return;
        }

        if (piece->getPieceSymbol() == kingID)
        {
            // Queen side castle
            if ((kingTNBefore - tileNumber) == 2)
            {
                grid[row][col - 2]->setMoved();
                grid[row][col + 1]->setPiece(grid[row][col - 2]->getPiece());
                grid[row][col - 2]->removePiece();
                grid[row][col - 2]->displayTile();
                grid[row][col + 1]->displayTile();
            }
            // king side castle
            else if ((kingTNBefore - tileNumber) == -2)
            {
                grid[row][col + 1]->setMoved();
                grid[row][col - 1]->setPiece(grid[row][col + 1]->getPiece());
                grid[row][col + 1]->removePiece();
                grid[row][col + 1]->displayTile();
                grid[row][col - 1]->displayTile();
            }
        }

        // update the attack board
        game->updateAttackBoard();
        // scan for check after a piece has moved
        game->scanForCheck();
    }
}
