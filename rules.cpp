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
    if (isWhite)
    {
        whiteKing = newTile;
        //        whiteCheck = false;
    }
    else
    {
        blackKing = newTile;
        //        blackCheck = false;
    }
}

void Rules::scanForCheck()
{
    isCheck = false;
    int row, col;
    BoardTile *k;

    if (isWhiteTurn)
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
    if (scanCheckHelper(row - 1, col, -1, 0, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == rookID) || (checkPiece == queenID)))
    {
        setCheck();
        return;
    }

    // check north-east
    if (scanCheckHelper(row - 1, col + 1, -1, 1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == bishopID) || (checkPiece == queenID)))
    {
        setCheck();
        return;
    }

    // check east
    if (scanCheckHelper(row, col + 1, 0, 1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == rookID) || (checkPiece == queenID)))
    {
        setCheck();
        return;
    }

    // check south-east
    if (scanCheckHelper(row + 1, col + 1, 1, 1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == bishopID) || (checkPiece == queenID)))
    {
        setCheck();
        return;
    }

    // check south
    if (scanCheckHelper(row + 1, col, 1, 0, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == rookID) || (checkPiece == queenID)))
    {
        setCheck();
        return;
    }

    // check south-west
    if (scanCheckHelper(row + 1, col - 1, 1, -1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == bishopID) || (checkPiece == queenID)))
    {
        setCheck();
        return;
    }

    // check west
    if (scanCheckHelper(row, col - 1, -1, 0, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == rookID) || (checkPiece == queenID)))
    {
        setCheck();
        return;
    }

    // check north-west
    if (scanCheckHelper(row - 1, col - 1, -1, -1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == bishopID) || (checkPiece == queenID)))
    {
        setCheck();
        return;
    }

    // check knght
    if (checkKnight(row, col))
    {
        setCheck();
        return;
    }

    // check pawn
    if (isWhiteTurn)
    {
        if (checkTile(row + 1, col - 1, pawnID) || checkTile(row + 1, col + 1, pawnID))
        {
            setCheck();
            return;
        }
    }
    else if (checkTile(row - 1, col - 1, pawnID) || checkTile(row - 1, col + 1, pawnID))
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
            ok = addValidMove(board[row - 1][col]->getTileNumber());
        }
        if (row == 6 && !board[5][col]->isOccupied() && !board[4][col]->isOccupied())
        {
            ok = addValidMove(board[row - 2][col]->getTileNumber());
        }
        if (row - 1 >= 0 && col - 1 >= 0)
        {
            if (board[row - 1][col - 1]->getPieceColor() != tile->getPieceColor() &&
                board[row - 1][col - 1]->isOccupied())
            {
                ok = addValidMove(board[row - 1][col - 1]->getTileNumber());
            }
        }
        if (row - 1 >= 0 && col + 1 <= 7)
        {
            if (board[row - 1][col + 1]->getPieceColor() != tile->getPieceColor() &&
                board[row - 1][col + 1]->isOccupied())
            {
                ok = addValidMove(board[row - 1][col + 1]->getTileNumber());
            }
        }
    }
    else
    {
        if (row + 1 <= 7 && !board[row + 1][col]->isOccupied())
        {
            ok = addValidMove(board[row + 1][col]->getTileNumber());
        }
        if (row == 1 && !board[2][col]->isOccupied() && !board[3][col]->isOccupied())
        {
            ok = addValidMove(board[row + 2][col]->getTileNumber());
        }
        if (row + 1 <= 7 && col - 1 >= 0)
        {
            if (board[row + 1][col - 1]->getPieceColor() != tile->getPieceColor() &&
                board[row + 1][col - 1]->isOccupied())
            {
                ok = addValidMove(board[row + 1][col - 1]->getTileNumber());
            }
        }
        if (row + 1 <= 7 && col + 1 <= 7)
        {
            if (board[row + 1][col + 1]->getPieceColor() != tile->getPieceColor() &&
                board[row + 1][col + 1]->isOccupied())
            {
                ok = addValidMove(board[row + 1][col + 1]->getTileNumber());
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
            ok = addValidMove(board[row - 2][col - 1]->getTileNumber());
        }
    }
    if (row - 2 >= 0 && col + 1 <= 7)
    {
        if (board[row - 2][col + 1]->getPieceColor() != tile->getPieceColor() ||
            !board[row - 2][col + 1]->isOccupied())
        {
            ok = addValidMove(board[row - 2][col + 1]->getTileNumber());
        }
    }
    if (row - 1 >= 0 && col - 2 >= 0)
    {
        if (board[row - 1][col - 2]->getPieceColor() != tile->getPieceColor() ||
            !board[row - 1][col - 2]->isOccupied())
        {
            ok = addValidMove(board[row - 1][col - 2]->getTileNumber());
        }
    }
    if (row - 1 >= 0 && col + 2 <= 7)
    {
        if (board[row - 1][col + 2]->getPieceColor() != tile->getPieceColor() ||
            !board[row - 1][col + 2]->isOccupied())
        {
            ok = addValidMove(board[row - 1][col + 2]->getTileNumber());
        }
    }
    if (row + 2 <= 7 && col + 1 <= 7)
    {
        if (board[row + 2][col + 1]->getPieceColor() != tile->getPieceColor() ||
            !board[row + 2][col + 1]->isOccupied())
        {
            ok = addValidMove(board[row + 2][col + 1]->getTileNumber());
        }
    }
    if (row + 2 <= 7 && col - 1 >= 0)
    {
        if (board[row + 2][col - 1]->getPieceColor() != tile->getPieceColor() ||
            !board[row + 2][col - 1]->isOccupied())
        {
            ok = addValidMove(board[row + 2][col - 1]->getTileNumber());
        }
    }
    if (row + 1 <= 7 && col - 2 >= 0)
    {
        if (board[row + 1][col - 2]->getPieceColor() != tile->getPieceColor() ||
            !board[row + 1][col - 2]->isOccupied())
        {
            ok = addValidMove(board[row + 1][col - 2]->getTileNumber());
        }
    }
    if (row + 1 <= 7 && col + 2 <= 7)
    {
        if ((board[row + 1][col + 2]->getPieceColor() != tile->getPieceColor()) ||
            (!board[row + 1][col + 2]->isOccupied()))
        {
            ok = addValidMove(board[row + 1][col + 2]->getTileNumber());
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
            *ok = addValidMove(board[row][col]->getTileNumber());
        }
        else if (board[row][col]->getPieceColor() == tile->getPieceColor())
        {
            break;
        }
        else if (board[row][col]->getPieceColor() != tile->getPieceColor())
        {
            *ok = addValidMove(board[row][col]->getTileNumber());
            break;
        }
        row += dr;
        col += dc;
    }
}

bool Rules::enforceKing(BoardTile *tile)
{
    // TODO: If check then narrow down valid moves

    bool ok = false;
    int rowOrig = tile->getRow();
    int colOrig = tile->getCol();

    int row = rowOrig - 1;
    int col = colOrig;
    if (row >= 0)
    {
        if (!board[row][col]->isOccupied() ||
            board[row][col]->getPieceColor() != tile->getPieceColor())
        {
//            ok = addValidMove(board[row][col]->getTileNumber(), true);
            ok = addValidMoveKing(row, col, board[row][col]->getTileNumber());
        }
    }

    row = rowOrig + 1;
    col = colOrig;
    if (row <= 7)
    {
        if (!board[row][col]->isOccupied() ||
            board[row][col]->getPieceColor() != tile->getPieceColor())
        {
//            ok = addValidMove(board[row][col]->getTileNumber(), true);
            ok = addValidMoveKing(row, col, board[row][col]->getTileNumber());
        }
    }

    row = rowOrig;
    col = colOrig - 1;
    if (col >= 0)
    {
        if (!board[row][col]->isOccupied() ||
            board[row][col]->getPieceColor() != tile->getPieceColor())
        {
//            ok = addValidMove(board[row][col]->getTileNumber(), true);
            ok = addValidMoveKing(row, col, board[row][col]->getTileNumber());
        }
    }

    row = rowOrig;
    col = colOrig + 1;
    if (col <= 7)
    {
        if (!board[row][col]->isOccupied() ||
            board[row][col]->getPieceColor() != tile->getPieceColor())
        {
//            ok = addValidMove(board[row][col]->getTileNumber(), true);
            ok = addValidMoveKing(row, col, board[row][col]->getTileNumber());
        }
    }

    row = rowOrig - 1;
    col = colOrig - 1;
    if (row >= 0 && col >= 0)
    {
        if (!board[row][col]->isOccupied() ||
            board[row][col]->getPieceColor() != tile->getPieceColor())
        {
//            ok = addValidMove(board[row][col]->getTileNumber(), true);
            ok = addValidMoveKing(row, col, board[row][col]->getTileNumber());
        }
    }

    row = rowOrig - 1;
    col = colOrig + 1;
    if (row >= 0 && col <= 7)
    {
        if (!board[row][col]->isOccupied() ||
            board[row][col]->getPieceColor() != tile->getPieceColor())
        {
//            ok = addValidMove(board[row][col]->getTileNumber(), true);
            ok = addValidMoveKing(row, col, board[row][col]->getTileNumber());
        }
    }

    row = rowOrig + 1;
    col = colOrig - 1;
    if (row <= 7 && col >= 0)
    {
        if (!board[row][col]->isOccupied() ||
            board[row][col]->getPieceColor() != tile->getPieceColor())
        {
//            ok = addValidMove(board[row][col]->getTileNumber(), true);
            ok = addValidMoveKing(row, col, board[row][col]->getTileNumber());
        }
    }

    row = rowOrig + 1;
    col = colOrig + 1;
    if (row <= 7 && col <= 7)
    {
        if (!board[row][col]->isOccupied() ||
            board[row][col]->getPieceColor() != tile->getPieceColor())
        {
//            ok = addValidMove(board[row][col]->getTileNumber(), true);
            ok = addValidMoveKing(row, col, board[row][col]->getTileNumber());
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
                tile->setStyleSheet(QString("QLabel {background-color: ") +
                                    currentTheme->getAttackBackground() + QString(" color: ") +
                                    currentTheme->getHoverColorWhite() + QString("}"));
            }
            else
            {
                tile->setStyleSheet(QString("QLabel {background-color: ") +
                                    currentTheme->getAttackBackground() + QString(" color: ") +
                                    currentTheme->getHoverColorBlack() + QString("}"));
            }
        }
        else
            tile->setStyleSheet(QString("QLabel {background-color: ") +
                                currentTheme->getHoverBackground() + QString("}"));
    }
}

inline void Rules::setCheck()
{
    isCheck = true;
}

inline bool Rules::scanCheckHelper(int row, int col, int dr, int dc, char *checkPiece)
{
    BoardTile *curr;
    while (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
    {
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
    if (checkTile(row - 2, col + 1, knightID) || checkTile(row - 1, col + 2, knightID) ||
        checkTile(row + 1, col + 2, knightID) || checkTile(row + 2, col + 1, knightID) ||
        checkTile(row + 2, col - 1, knightID) || checkTile(row + 1, col - 2, knightID) ||
        checkTile(row - 1, col - 2, knightID) || checkTile(row - 2, col - 1, knightID))
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
    if (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
    {
        BoardTile *curr = board[row][col];
        if ((curr->getPieceSymbol() == pieceType) && (curr->getPieceColor() != isWhiteTurn))
        {
            return true;
        }
    }
    return false;
}

inline bool Rules::addValidMove(int tileNumber, bool isKing)
{
    if (isCheck)
    {
        if ((board[checkRow][checkCol]->getPieceSymbol() == knightID) &&
            (tileNumber == board[checkRow][checkCol]->getTileNumber()))
        {
            validMoves[vmIdx++] = tileNumber;
        }
        else
        {
            // determine the direction towards the king from the
            // piece causing the check.
            int dr = 0;
            int dc = 0;
            int diffRow, diffCol;
            if (isWhiteTurn)
            {
                diffRow = whiteKing->getRow() - checkRow;
                diffCol = whiteKing->getCol() - checkCol;
            }
            else
            {
                diffRow = blackKing->getRow() - checkRow;
                diffCol = blackKing->getCol() - checkCol;
            }
            // if dr and dc are non-zero than normalize them such
            // that any positive integer becomes +1 and any negative
            // integer becomes -1.
            if(diffRow)
            {
                dr = diffRow / abs(diffRow);
            }
            if(diffCol)
            {
                dc = diffCol / abs(diffCol);
            }

            bool kingMove = true;
            int r = checkRow;
            int c = checkCol;
            BoardTile *curr = board[r][c];
            while (curr->getPieceSymbol() != kingID)
            {
                if(isKing)
                {
                    if(curr->getTileNumber() == tileNumber && !curr->isOccupied())
                    {
                        kingMove = false;
                    }
                }
                else if (curr->getTileNumber() == tileNumber)
                {
                    validMoves[vmIdx++] = tileNumber;
                }
                r += dr;
                c += dc;
                curr = board[r][c];
            }

            // if the king move is valid add to list
            if(isKing && kingMove)
            {
                validMoves[vmIdx++] = tileNumber;
            }
        }
    }
    else
    {
        validMoves[vmIdx++] = tileNumber;
    }
    return true;
}

inline bool Rules::addValidMoveKing(int row, int col, int tileNumber)
{
    if(isCheck)
    {
        if ((board[checkRow][checkCol]->getPieceSymbol() == knightID) &&
            (tileNumber == board[checkRow][checkCol]->getTileNumber()))
        {
            validMoves[vmIdx++] = tileNumber;
        }
        else
        {
            // determine the direction from the king towards the king from the
            // piece causing the check.
            int dr = 0;
            int dc = 0;
            int diffRow, diffCol, r, c;
            if (isWhiteTurn)
            {
                diffRow = whiteKing->getRow() - checkRow;
                diffCol = whiteKing->getCol() - checkCol;
                r = whiteKing->getRow();
                c = whiteKing->getCol();
            }
            else
            {
                diffRow = blackKing->getRow() - checkRow;
                diffCol = blackKing->getCol() - checkCol;
                r = blackKing->getRow();
                c = whiteKing->getCol();
            }
            // if dr and dc are non-zero than normalize them such
            // that any positive integer becomes +1 and any negative
            // integer becomes -1.
            if(diffRow)
            {
                dr = diffRow / abs(diffRow);
            }
            if(diffCol)
            {
                dc = diffCol / abs(diffCol);
            }

            // if the piece causing check is not a pawn
            if(board[checkRow][checkCol]->getPieceSymbol() != pawnID)
            {
                if((((r+dr)== row) && ((c+dc) == col)) ||
                        (((r-dr) == row) && ((c-dc) == col) && !board[row][col]->isOccupied()))
                {
                    return true;
                }
                else
                {
                    validMoves[vmIdx++] = tileNumber;
                }
            }
            else
            {

            }


        }

    }
    else
    {
        validMoves[vmIdx++] = tileNumber;
    }
    return true;
}
