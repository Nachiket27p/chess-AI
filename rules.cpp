#include "rules.h"
#include "utils.h"

extern bool isWhiteTurn;
extern Theme *currentTheme;

Rules *Rules::instance = nullptr;

Rules *Rules::getInstance()
{
    if (!instance)
    {
        instance = new Rules();
    }
    return instance;
}

bool Rules::canMove(BoardTile *tile)
{
//    if (isCheck && (tile->getPieceSymbol() != kingID))
//    {
//        okToMove = false;
//        return okToMove;
//    }
    if (isCheck)
    {
        // TODO: Use checkRow amd checkCol to determine if the
        //          selected piece is allowed to move, if os then
        //          only allow it to move to places which block
        //          the check.
        okToMove = false;
        return okToMove;
    }


    switch (tile->getPieceSymbol())
    {
    case pawnID:
        okToMove = enforcePawn(tile);
        break;
    case rookID:
        okToMove = enforceRook(tile);
        break;
    case knightID:
        okToMove = enforceKnight(tile);
        break;
    case bishopID:
        okToMove = enforceBishop(tile);
        break;
    case queenID:
        okToMove = enforceQueen(tile);
        break;
    case kingID:
        okToMove = enforceKing(tile);
        break;
    default:
        break;
    }

    // highlight the tile to which this piece an move to
    highlightTiles();
    return okToMove;
}

void Rules::setKingPos(bool isWhite, BoardTile *newTile)
{
    // because the king moved set the check value back to false
    if(isWhite)
    {
        whiteKing = newTile;
//        whiteCheck = false;
    }
    else
    {
        blackKing = newTile;
//        blackCheck = false;
    }
    isCheck = false;
}

void Rules::scanForCheck()
{
    int row, col;
    BoardTile *k;

    if(isWhiteTurn)
    {
        row = whiteKing->getRow();
        col = whiteKing->getCol();
        k = whiteKing;
    }
    else
    {
        row = blackKing->getRow();
        col = blackKing->getCol();
        k = blackKing;
    }

    char checkPiece;

    // check north
    if(scanCheckHelper(row-1, col, -1, 0, &checkPiece) && checkPiece != '\0'
        && ((checkPiece == rookID) || (checkPiece == queenID)))
    {
            setCheck();
            return;
    }

    // check north-east
    if(scanCheckHelper(row-1, col+1, -1, 1, &checkPiece) && checkPiece != '\0'
       && ((checkPiece == bishopID) || (checkPiece == queenID)))
    {
            setCheck();
            return;
    }

    // check east
    if(scanCheckHelper(row, col+1, 0, 1, &checkPiece) && checkPiece != '\0'
       && ((checkPiece == rookID) || (checkPiece == queenID)))
    {
            setCheck();
            return;
    }

    // check south-east
    if(scanCheckHelper(row+1, col+1, 1, 1, &checkPiece) && checkPiece != '\0'
       && ((checkPiece == bishopID) || (checkPiece == queenID)))
    {
            setCheck();
            return;
    }

    // check south
    if(scanCheckHelper(row+1, col, 1, 0, &checkPiece) && checkPiece != '\0'
       && ((checkPiece == rookID) || (checkPiece == queenID)))
    {
            setCheck();
            return;
    }

    // check south-west
    if(scanCheckHelper(row+1, col-1, 1, -1, &checkPiece) && checkPiece != '\0'
       && ((checkPiece == bishopID) || (checkPiece == queenID)))
    {
            setCheck();
            return;
    }

    // check west
    if(scanCheckHelper(row, col-1, -1, 0, &checkPiece) && checkPiece != '\0'
       && ((checkPiece == rookID) || (checkPiece == queenID)))
    {
            setCheck();
            return;
    }

    // check north-west
    if(scanCheckHelper(row-1, col-1, -1, -1, &checkPiece) && checkPiece != '\0'
       && ((checkPiece == bishopID) || (checkPiece == queenID)))
    {
            setCheck();
            return;
    }

    // check knght
    if(checkKnight(row, col))
    {
        setCheck();
        return;
    }

    // check pawn
    if(isWhiteTurn)
    {
        if(checkTile(row+1, col-1, pawnID) || checkTile(row+1, col+1, pawnID))
        {
            setCheck();
            return;
        }
    }
    else if(checkTile(row-1, col-1, pawnID) || checkTile(row-1, col+1, pawnID))
    {
        setCheck();
        return;
    }

}



bool Rules::enforcePawn(BoardTile *tile)
{
    bool ok = false;
    int row = tile->getRow();
    int col = tile->getCol();

    if (tile->getPieceColor())
    {
        if (row - 1 >= 0 && !board[row - 1][col]->isOccupied())
        {
            validMoves[vmIdx++] = board[row - 1][col]->getTileNumber();
            ok = true;
        }
        if (row == 6 && !board[5][col]->isOccupied() && !board[4][col]->isOccupied())
        {
            validMoves[vmIdx++] = board[row - 2][col]->getTileNumber();
            ok = true;
        }
        if (row - 1 >= 0 && col - 1 >= 0)
        {
            if (board[row - 1][col - 1]->getPieceColor() != tile->getPieceColor() &&
                board[row - 1][col - 1]->isOccupied())
            {
                validMoves[vmIdx++] = board[row - 1][col - 1]->getTileNumber();
                ok = true;
            }
        }
        if (row - 1 >= 0 && col + 1 <= 7)
        {
            if (board[row - 1][col + 1]->getPieceColor() != tile->getPieceColor() &&
                board[row - 1][col + 1]->isOccupied())
            {
                validMoves[vmIdx++] = board[row - 1][col + 1]->getTileNumber();
                ok = true;
            }
        }
    }
    else
    {
        if (row + 1 <= 7 && !board[row + 1][col]->isOccupied())
        {
            validMoves[vmIdx++] = board[row + 1][col]->getTileNumber();
            ok = true;
        }
        if (row == 1 && !board[2][col]->isOccupied() && !board[3][col]->isOccupied())
        {
            validMoves[vmIdx++] = board[row + 2][col]->getTileNumber();
            ok = true;
        }
        if (row + 1 <= 7 && col - 1 >= 0)
        {
            if (board[row + 1][col - 1]->getPieceColor() != tile->getPieceColor() &&
                board[row + 1][col - 1]->isOccupied())
            {
                validMoves[vmIdx++] = board[row + 1][col - 1]->getTileNumber();
                ok = true;
            }
        }
        if (row + 1 <= 7 && col + 1 <= 7)
        {
            if (board[row + 1][col + 1]->getPieceColor() != tile->getPieceColor() &&
                board[row + 1][col + 1]->isOccupied())
            {
                validMoves[vmIdx++] = board[row + 1][col + 1]->getTileNumber();
                ok = true;
            }
        }
    }
    return ok;
}

bool Rules::enforceRook(BoardTile *tile)
{
    bool ok = false;
    int origRow = tile->getRow();
    int origCol = tile->getCol();
    int row = origRow;
    int col = origCol;

    // check north
    enforceRBQHelper(row - 1, col, -1, 0, &ok, tile);

    // chek east
    enforceRBQHelper(row, col + 1, 0, +1, &ok, tile);

    // check south
    enforceRBQHelper(row + 1, col, +1, 0, &ok, tile);

    // check west
    enforceRBQHelper(row, col - 1, 0, -1, &ok, tile);

    return ok;
}

bool Rules::enforceKnight(BoardTile *tile)
{
    bool ok = false;
    int row = tile->getRow();
    int col = tile->getCol();

    if (row - 2 >= 0 && col - 1 >= 0)
    {
        if (board[row - 2][col - 1]->getPieceColor() != tile->getPieceColor() ||
            !board[row - 2][col - 1]->isOccupied())
        {
            validMoves[vmIdx++] = board[row - 2][col - 1]->getTileNumber();
            ok = true;
        }
    }
    if (row - 2 >= 0 && col + 1 <= 7)
    {
        if (board[row - 2][col + 1]->getPieceColor() != tile->getPieceColor() ||
            !board[row - 2][col + 1]->isOccupied())
        {
            validMoves[vmIdx++] = board[row - 2][col + 1]->getTileNumber();
            ok = true;
        }
    }
    if (row - 1 >= 0 && col - 2 >= 0)
    {
        if (board[row - 1][col - 2]->getPieceColor() != tile->getPieceColor() ||
            !board[row - 1][col - 2]->isOccupied())
        {
            validMoves[vmIdx++] = board[row - 1][col - 2]->getTileNumber();
            ok = true;
        }
    }
    if (row - 1 >= 0 && col + 2 <= 7)
    {
        if (board[row - 1][col + 2]->getPieceColor() != tile->getPieceColor() ||
            !board[row - 1][col + 2]->isOccupied())
        {
            validMoves[vmIdx++] = board[row - 1][col + 2]->getTileNumber();
            ok = true;
        }
    }
    if (row + 2 <= 7 && col + 1 <= 7)
    {
        if (board[row + 2][col + 1]->getPieceColor() != tile->getPieceColor() ||
            !board[row + 2][col + 1]->isOccupied())
        {
            validMoves[vmIdx++] = board[row + 2][col + 1]->getTileNumber();
            ok = true;
        }
    }
    if (row + 2 <= 7 && col - 1 >= 0)
    {
        if (board[row + 2][col - 1]->getPieceColor() != tile->getPieceColor() ||
            !board[row + 2][col - 1]->isOccupied())
        {
            validMoves[vmIdx++] = board[row + 2][col - 1]->getTileNumber();
            ok = true;
        }
    }
    if (row + 1 <= 7 && col - 2 >= 0)
    {
        if (board[row + 1][col - 2]->getPieceColor() != tile->getPieceColor() ||
            !board[row + 1][col - 2]->isOccupied())
        {
            validMoves[vmIdx++] = board[row + 1][col - 2]->getTileNumber();
            ok = true;
        }
    }
    if (row + 1 <= 7 && col + 2 <= 7)
    {
        if (board[row + 1][col + 2]->getPieceColor() != tile->getPieceColor() || !board[row + 1][col + 2]->isOccupied())
        {
            validMoves[vmIdx++] = board[row + 1][col + 2]->getTileNumber();
            ok = true;
        }
    }
    return ok;
}

bool Rules::enforceBishop(BoardTile *tile)
{
    bool ok = false;
    int row = tile->getRow();
    int col = tile->getCol();


    // check north-east
    enforceRBQHelper(row - 1, col + 1, -1, +1, &ok, tile);

    // check south-east
    enforceRBQHelper(row + 1, col + 1, +1, +1, &ok, tile);

    // check south-west
    enforceRBQHelper(row + 1, col - 1, +1, -1, &ok, tile);

    // check norh-west
    enforceRBQHelper(row - 1, col - 1, -1, -1, &ok, tile);

    return ok;
}

bool Rules::enforceQueen(BoardTile *tile)
{
    bool ok = false;
    int row = tile->getRow();
    int col = tile->getCol();

    // check north
    enforceRBQHelper(row - 1, col, -1, 0, &ok, tile);

    // check north-east
    enforceRBQHelper(row - 1, col + 1, -1, +1, &ok, tile);

    // chek east
    enforceRBQHelper(row, col + 1, 0, +1, &ok, tile);

    // check south-east
    enforceRBQHelper(row + 1, col + 1, +1, +1, &ok, tile);

    // check south
    enforceRBQHelper(row + 1, col, +1, 0, &ok, tile);

    // check south-west
    enforceRBQHelper(row + 1, col - 1, +1, -1, &ok, tile);

    // check west
    enforceRBQHelper(row, col - 1, 0, -1, &ok, tile);

    // check norh-west
    enforceRBQHelper(row - 1, col - 1, -1, -1, &ok, tile);

    return ok;
}

inline void Rules::enforceRBQHelper(int row, int col, int dr, int dc, bool *ok, BoardTile *tile)
{
    while (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
    {
        if (!board[row][col]->isOccupied())
        {
            validMoves[vmIdx++] = board[row][col]->getTileNumber();
            *ok = true;
        }
        else if (board[row][col]->getPieceColor() == tile->getPieceColor())
        {
            break;
        }
        else if (board[row][col]->getPieceColor() != tile->getPieceColor())
        {
            validMoves[vmIdx++] = board[row][col]->getTileNumber();
            *ok = true;
        }
        row += dr;
        col += dc;
    }

}

bool Rules::enforceKing(BoardTile *tile)
{
    // TODO: If check then narrow down valid moves

    bool ok = false;
    int row = tile->getRow();
    int col = tile->getCol();

    if (row - 1 >= 0)
    {
        if (!board[row - 1][col]->isOccupied() ||
            board[row - 1][col]->getPieceColor() != tile->getPieceColor())
        {
            validMoves[vmIdx++] = board[row - 1][col]->getTileNumber();
            ok = true;
        }
    }
    if (row + 1 <= 7)
    {
        if (!board[row + 1][col]->isOccupied() ||
            board[row + 1][col]->getPieceColor() != tile->getPieceColor())
        {
            validMoves[vmIdx++] = board[row + 1][col]->getTileNumber();
            ok = true;
        }
    }
    if (col - 1 >= 0)
    {
        if (!board[row][col - 1]->isOccupied() ||
            board[row][col - 1]->getPieceColor() != tile->getPieceColor())
        {
            validMoves[vmIdx++] = board[row][col - 1]->getTileNumber();
            ok = true;
        }
    }
    if (col + 1 <= 7)
    {
        if (!board[row][col + 1]->isOccupied() ||
            board[row][col + 1]->getPieceColor() != tile->getPieceColor())
        {
            validMoves[vmIdx++] = board[row][col + 1]->getTileNumber();
            ok = true;
        }
    }
    if (row - 1 >= 0 && col - 1 >= 0)
    {
        if (!board[row - 1][col - 1]->isOccupied() ||
            board[row - 1][col - 1]->getPieceColor() != tile->getPieceColor())
        {
            validMoves[vmIdx++] = board[row - 1][col - 1]->getTileNumber();
            ok = true;
        }
    }
    if (row - 1 >= 0 && col + 1 <= 7)
    {
        if (!board[row - 1][col + 1]->isOccupied() ||
            board[row - 1][col + 1]->getPieceColor() != tile->getPieceColor())
        {
            validMoves[vmIdx++] = board[row - 1][col + 1]->getTileNumber();
            ok = true;
        }
    }
    if (row + 1 <= 7 && col - 1 >= 0)
    {
        if (!board[row + 1][col - 1]->isOccupied() ||
            board[row + 1][col - 1]->getPieceColor() != tile->getPieceColor())
        {
            validMoves[vmIdx++] = board[row + 1][col - 1]->getTileNumber();
            ok = true;
        }
    }
    if (row + 1 <= 7 && col + 1 <= 7)
    {
        if (!board[row + 1][col + 1]->isOccupied() ||
            board[row + 1][col + 1]->getPieceColor() != tile->getPieceColor())
        {
            validMoves[vmIdx++] = board[row + 1][col + 1]->getTileNumber();
            ok = true;
        }
    }
    return ok;
}

void Rules::highlightTiles()
{
    BoardTile *tile;
    for (int i = 0; i < vmIdx; i++)
    {
        tile = board[validMoves[i] / 8][validMoves[i] & 7];

        if (tile->isOccupied())
        {
            if (tile->getPieceColor())
            {
                tile->setStyleSheet(QString("QLabel {background-color: ") + currentTheme->getAttackBackground() + QString(" color: ") +
                                    currentTheme->getHoverColorWhite() + QString("}"));
            }
            else
            {
                tile->setStyleSheet(QString("QLabel {background-color: ") + currentTheme->getAttackBackground() + QString(" color: ") +
                                    currentTheme->getHoverColorBlack() + QString("}"));
            }
        }
        else
            tile->setStyleSheet(QString("QLabel {background-color: ") + currentTheme->getHoverBackground() + QString("}"));
    }
}

inline void Rules::setCheck()
{
    isCheck = true;
}

inline bool Rules::scanCheckHelper(int row, int col, int dr, int dc, char *checkPiece)
{
    BoardTile *curr;
    while (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col)) {
        curr = board[row][col];
        if (curr->isOccupied())
        {
            if (curr->getPieceColor() != isWhiteTurn)
            {
                *checkPiece = curr->getPieceSymbol();
                checkRow = row;
                checkCol = col;
                return true;
            }
            else
            {
                *checkPiece = '\0';
                return false;
            }
        }
        row += dr;
        col += dc;
        curr = board[row][col];
    }
    *checkPiece = '\0';
    return false;
}

inline bool Rules::checkKnight(int row, int col)
{
    if (checkTile(row-2, col+1, knightID) || checkTile(row-1, col+2, knightID)
        || checkTile(row+1, col+2, knightID) || checkTile(row+2, col+1, knightID)
        || checkTile(row+2, col-1, knightID) ||checkTile(row+1, col-2, knightID)
        || checkTile(row-1, col-2, knightID) || checkTile(row-2, col-1, knightID))
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline bool Rules::checkTile(int row, int col, char pieceType)
{
    if(WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
    {
        BoardTile *curr = board[row][col];
        if ((curr->getPieceSymbol() == pieceType) && (curr->getPieceColor() != isWhiteTurn))
        {
            return true;
        }
    }
    return false;
}
