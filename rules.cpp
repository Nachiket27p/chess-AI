#include "rules.h"
#include "utils.h"
#include "piece.h"

extern bool isWhiteTurn;
extern Theme *currentTheme;
extern Piece *whitePieces[16];
extern Piece *blackPieces[16];

Rules *Rules::instance = nullptr;

//sssssssssssssssssssssss
void Rules::setDebugWindowAccess(DebugWindow *_dbw)
{
    dbw = _dbw;
}

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
    }
    else
    {
        blackKing = newTile;
    }
}

void Rules::scanForCheck()
{
    // use the attack grid to determine a potential for a check
    isCheck = false;
    int row, col;

    if (isWhiteTurn)
    {
        row = whiteKing->getRow();
        col = whiteKing->getCol();
        if (blackAttacks[row][col] == 1)
             setCheck();
    }
    else
    {
        row = blackKing->getRow();
        col = blackKing->getCol();
        if (whiteAttacks[row][col] == 1)
             setCheck();
    }

    // if there is a possibility of a check then properly computer the check location
    // if one really exists
    if (isCheck)
    {
        locateCheckSource();
    }
}

bool Rules::enforcePawn(BoardTile *tile)
{
    bool ok = false;

    // If this piece is a king defender
    bool defender = isKingDefender(tile->getPiece());
    int kcol = whiteKing->getCol();

    if(isWhiteTurn)
    {
        kcol = blackKing->getCol();
    }
    bool multiDefenders = false;

    int row = tile->getRow();
    int col = tile->getCol();
    if(whiteAttacks[row][col] < -2)
        multiDefenders = true;

    if (tile->getPieceColor())
    {
        if((defender && (kcol == col)) || !defender || multiDefenders)
        {
            if (row - 1 >= 0 && !grid[row - 1][col]->isOccupied())
            {
                addValidMove(grid[row - 1][col]->getTileNumber(), &ok);
            }
            if (row == 6 && !grid[5][col]->isOccupied() && !grid[4][col]->isOccupied())
            {
                addValidMove(grid[row - 2][col]->getTileNumber(), &ok);
            }
        }
        if ((row - 1 >= 0) && (col - 1 >= 0) && (!defender || (defender && (kcol != col)) || multiDefenders))
        {
            if (grid[row - 1][col - 1]->isOccupied() &&
                grid[row - 1][col - 1]->getPieceColor() != tile->getPieceColor())
            {
                addValidMove(grid[row - 1][col - 1]->getTileNumber(), &ok);
            }
        }
        if ((row - 1 >= 0) && (col + 1 <= 7) && (!defender || (defender && (kcol != col)) || multiDefenders))
        {
            if (grid[row - 1][col + 1]->isOccupied() &&
                grid[row - 1][col + 1]->getPieceColor() != tile->getPieceColor())
            {
                addValidMove(grid[row - 1][col + 1]->getTileNumber(), &ok);
            }
        }
    }
    else
    {
        if((defender && (kcol == col)) || !defender || multiDefenders)
        {
            if (row + 1 <= 7 && !grid[row + 1][col]->isOccupied())
            {
                addValidMove(grid[row + 1][col]->getTileNumber(), &ok);
            }
            if (row == 1 && !grid[2][col]->isOccupied() && !grid[3][col]->isOccupied())
            {
                addValidMove(grid[row + 2][col]->getTileNumber(), &ok);
            }
        }
        if ((row + 1 <= 7) && (col - 1 >= 0) && (!defender || (defender && (kcol != col)) || multiDefenders))
        {
            if (grid[row + 1][col - 1]->isOccupied() &&
                grid[row + 1][col - 1]->getPieceColor() != tile->getPieceColor())
            {
                addValidMove(grid[row + 1][col - 1]->getTileNumber(), &ok);
            }
        }
        if ((row + 1 <= 7) && (col + 1 <= 7) && (!defender || (defender && (kcol != col)) || multiDefenders))
        {
            if (grid[row + 1][col + 1]->isOccupied() &&
                grid[row + 1][col + 1]->getPieceColor() != tile->getPieceColor())
            {
                addValidMove(grid[row + 1][col + 1]->getTileNumber(), &ok);
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
    // if the knight is a protecting the king currently it cannot move
    if(isKingDefender(tile->getPiece()))
    {
        return ok;
    }
    int rowOrig = tile->getRow();
    int colOrig = tile->getCol();
    int pos[8][2] = {{-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}};
    int row, col, dr, dc;

    for (int i = 0; i < 8; i++)
    {
        dr = pos[i][0];
        dc = pos[i][1];
        row = rowOrig + dr;
        col = colOrig + dc;
        if (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
        {
            if (grid[row][col]->getPieceColor() != tile->getPieceColor() ||
                !grid[row][col]->isOccupied())
            {
                addValidMove(grid[row][col]->getTileNumber(), &ok);
            }
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

    BoardTile *king = blackKing;
    int attackBoardVal = whiteAttacks[row][col];
    if(isWhiteTurn)
    {
        king = whiteKing;
        attackBoardVal = blackAttacks[row][col];
    }

    for(int dr = -1; dr <= 1; dr++)
    {
        for(int dc = -1; dc <= 1; dc++)
        {
            if(!((dr == 0) && (dc ==0)))
            {
                if(canKingDefenderMove(dr, dc, tile, king, attackBoardVal))
                    enforceRBQHelper(row + dr, col + dc, dr, dc, &ok, tile);
            }
        }
    }
    return ok;
}

inline void Rules::enforceRBQHelper(int row, int col, int dr, int dc, bool *ok, BoardTile *tile)
{
    while (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
    {
        if (!grid[row][col]->isOccupied())
        {
            addValidMove(grid[row][col]->getTileNumber(), ok);
        }
        else if (grid[row][col]->getPieceColor() == tile->getPieceColor())
        {
            break;
        }
        else if (grid[row][col]->getPieceColor() != tile->getPieceColor())
        {
            addValidMove(grid[row][col]->getTileNumber(), ok);
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
    int row, col;
    for (int dr = -1; dr <= 1; dr++)
    {
        if (WITHIN_BOUNDS(rowOrig + dr))
        {
            for (int dc = -1; dc <= 1; dc++)
            {
                if (!(dr == 0 && dc == 0) && WITHIN_BOUNDS(colOrig + dc))
                {
                    row = rowOrig + dr;
                    col = colOrig + dc;

                    if (!grid[row][col]->isOccupied() ||
                        grid[row][col]->getPieceColor() != tile->getPieceColor())
                    {
                        addValidMoveKing(row, col, grid[row][col]->getTileNumber(), &ok);
                    }
                }
            }
        }
    }
    return ok;
}

void Rules::highlightTiles()
{
    BoardTile *tile;
    for (int i = 0; i < vmIdx; i++)
    {
        tile = grid[validMoves[i] / 8][validMoves[i] & 7];

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

inline void Rules::addValidMove(int tileNumber, bool *ok)
{
    int vmIdxbefore = vmIdx;
    if (isCheck)
    {
        if ((grid[checkRow][checkCol]->getPieceSymbol() == knightID) &&
            (tileNumber == grid[checkRow][checkCol]->getTileNumber()))
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
            if (diffRow)
            {
                dr = diffRow / abs(diffRow);
            }
            if (diffCol)
            {
                dc = diffCol / abs(diffCol);
            }

            int r = checkRow;
            int c = checkCol;
            BoardTile *curr = grid[r][c];
            while ((!curr->isOccupied()) || (curr->getPieceSymbol() != kingID))
            {
                if (curr->getTileNumber() == tileNumber)
                {
                    validMoves[vmIdx++] = tileNumber;
                }
                r += dr;
                c += dc;
                curr = grid[r][c];
            }
        }

        // if a valid move was added then return true otherwise return false.
        if (vmIdxbefore != vmIdx)
            *ok = true;
    }
    else
    {
        validMoves[vmIdx++] = tileNumber;
        *ok = true;
    }
}

inline void Rules::addValidMoveKing(int row, int col, int tileNumber, bool *ok)
{
    if (isCheck)
    {
        int vmIdxbefore = vmIdx;
        if (isWhiteTurn)
        {
            if (!blackAttacks[row][col])
                validMoves[vmIdx++] = tileNumber;
        }
        else
        {
            if (!whiteAttacks[row][col])
                validMoves[vmIdx++] = tileNumber;
        }

        // if a valid move was added return true otherwise return false
        if (vmIdxbefore != vmIdx)
        {
            *ok = true;
        }
    }
    else
    {
        validMoves[vmIdx++] = tileNumber;
        *ok = true;
    }
}

//--------------------------------------------------------------------------//
// The following function are used to compute the possible attack positions
// for black/white side.
//--------------------------------------------------------------------------//

void Rules::updateAttackBoard()
{
    if (isWhiteTurn)
    {
        // reset white defenders
        whiteKingDefenders.clear();
        // reset all values to zero
        memset(blackAttacks, 0, (8 * 8) * sizeof(int));
        // update black attack board
        updateAttackBoardHelper(blackPieces, blackAttacks, false);
        dbw->updateBlackValues();
    }
    else
    {
        // reset king defenders
        blackKingDefenders.clear();
        // reset all values to zero
        memset(whiteAttacks, 0, (8 * 8) * sizeof(int));
        // update white attack board
        updateAttackBoardHelper(whitePieces, whiteAttacks, true);
        dbw->updateWhiteValues();
    }
}

void Rules::resetKingDefenders()
{
    if(isWhiteTurn)
    {
        whiteKingDefenders.clear();
    }
    else
    {
        blackKingDefenders.clear();
    }
}

inline void Rules::updateAttackBoardHelper(Piece *pieces[16], int attackBoard[8][8], int pieceColor)
{
    for (int x = 0; x < 16; x++)
    {
        Piece *p = pieces[x];
        int pr = p->getRow();
        int pc = p->getCol();

        // used to indexing pawns
        int pawndr = 1;
        if(pieceColor)
            pawndr = -1;

        switch (p->getPieceSymbol())
        {
        case pawnID:
            if (WITHIN_BOUNDS(pr + pawndr))
            {
                if (WITHIN_BOUNDS(pc + 1))
                {
                    if(!grid[pr + pawndr][pc + 1]->isOccupied())
                    {
                        attackBoard[pr + pawndr][pc + 1] = 1;
                    }
                    else if(grid[pr + pawndr][pc + 1]->getPieceColor() != pieceColor)
                    {
                        attackBoard[pr + pawndr][pc + 1] = -1;
                    }
                }

                if (WITHIN_BOUNDS(pc - 1))
                {
                    if(!grid[pr + pawndr][pc - 1]->isOccupied())
                    {
                        attackBoard[pr + pawndr][pc - 1] = 1;
                    }
                    else if(grid[pr + pawndr][pc - 1]->getPieceColor() != pieceColor)
                    {
                        attackBoard[pr + pawndr][pc - 1] = -1;
                    }
                }
            }
            break;
        case rookID:
            attackeRBQHelper(pr, pc, +1, 0, pieceColor, attackBoard);
            attackeRBQHelper(pr, pc, -1, 0, pieceColor, attackBoard);
            attackeRBQHelper(pr, pc, 0, +1, pieceColor, attackBoard);
            attackeRBQHelper(pr, pc, 0, -1, pieceColor, attackBoard);
            break;

        case knightID:
        {
            int pos[8][2] = {{-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}};
            int dr, dc;
            for (int i = 0; i < 8; i++)
            {
                dr = pos[i][0];
                dc = pos[i][1];
                if (WITHIN_BOUNDS(pr + dr) && WITHIN_BOUNDS(pc + dc))
                {
                    if(!grid[pr + dr][pc + dc]->isOccupied())
                    {
                        attackBoard[pr + dr][pc + dc] = 1;
                    }
                    else if(grid[pr + dr][pc + dc]->getPieceColor() != pieceColor)
                    {
                        attackBoard[pr + dr][pc + dc] = -1;
                    }
                }
            }
            break;
        }
        case queenID:
            attackeRBQHelper(pr, pc, +1, 0, pieceColor, attackBoard);
            attackeRBQHelper(pr, pc, -1, 0, pieceColor, attackBoard);
            attackeRBQHelper(pr, pc, 0, +1, pieceColor, attackBoard);
            attackeRBQHelper(pr, pc, 0, -1, pieceColor, attackBoard);

        case bishopID:
            attackeRBQHelper(pr, pc, +1, +1, pieceColor, attackBoard);
            attackeRBQHelper(pr, pc, -1, -1, pieceColor, attackBoard);
            attackeRBQHelper(pr, pc, +1, -1, pieceColor, attackBoard);
            attackeRBQHelper(pr, pc, -1, +1, pieceColor, attackBoard);
            break;
        case kingID:
        {
            for (int i = -1; i <= 1; i++)
            {
                if (WITHIN_BOUNDS(pr + i))
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        if (!(i == 0 && j == 0) && WITHIN_BOUNDS(pc + i) &&
                            grid[pr + i][pc + j]->isOccupied() &&
                            grid[pr + i][pc + j]->getPieceColor() != pieceColor)
                        {
                            attackBoard[pr + i][pc + j] = true;
                        }
                    }
                }
            }
        }
        break;
        default:
            break;
        }
    }
}

inline void Rules::attackeRBQHelper(int row, int col, int dr, int dc, bool pieceColor, int attackGrid[8][8])
{
    row += dr;
    col += dc;
    int val = 1;
    while (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
    {
        if (grid[row][col]->isOccupied())
        {
            if((grid[row][col]->getPieceSymbol() == kingID) && (attackGrid[row-dr][col-dc] == -1))
            {
                int dval = -1;
                attackGrid[row][col] = dval--;
                int nrow = row - dr;
                int ncol = col - dc;
                while(attackGrid[nrow][ncol] == -1)
                {
                    if(grid[nrow][ncol]->isOccupied())
                    {
                        attackGrid[nrow][ncol] = dval--;
                    }
                    nrow -= dr;
                    ncol -= dc;
                }
                // save the piece in the list of kings defenders
                if(pieceColor)
                {
                    blackKingDefenders.push_back(grid[nrow + dr][ncol + dc]->getPiece());
                }
                else
                {
                    whiteKingDefenders.push_back(grid[nrow + dr][ncol + dc]->getPiece());
                }

                // update the row and column and continue
                row += dr;
                col += dc;
                continue;
            }

            if(grid[row][col]->getPieceColor() == pieceColor)
            {
                break;
            }
            else
            {
                val = -1;
            }

        }
        // if this grid slot is already marked with a negative value do not change it
        // only set the vaue if it is 0
        if(attackGrid[row][col] >= 0)
        {
            attackGrid[row][col] = val;
        }
        row += dr;
        col += dc;
    }
}

//--------------------------------------------------------------------------//
// The following function are used to determine the source of a check.      //
//--------------------------------------------------------------------------//

inline void Rules::locateCheckSource()
{
    isCheck = false;
    int row, col;

    if (isWhiteTurn)
    {
        row = whiteKing->getRow();
        col = whiteKing->getCol();
    }
    else
    {
        row = blackKing->getRow();
        col = blackKing->getCol();
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

inline bool Rules::scanCheckHelper(int row, int col, int dr, int dc, char *checkPiece)
{
    BoardTile *curr;
    while (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
    {
        curr = grid[row][col];
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
        curr = grid[row][col];
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
        BoardTile *curr = grid[row][col];
        if ((curr->getPieceSymbol() == pieceType) && (curr->getPieceColor() != isWhiteTurn))
        {
            return true;
        }
    }
    return false;
}

inline void Rules::setCheck()
{
    isCheck = true;
}

bool Rules::isKingDefender(Piece* p)
{
    std::vector<Piece *> kd;
    if(isWhiteTurn)
    {
        kd = whiteKingDefenders;
    }
    else
    {
        kd = blackKingDefenders;
    }

    int kSize = kd.size();
    for(int i = 0; i < kSize; i++)
    {
        if(p == kd[i])
        {
            return true;
        }
    }
    return false;
}

bool Rules::canKingDefenderMove(int dr, int dc, BoardTile *tile, BoardTile *king, int attackBoardVal)
{
    int kdr;
    int kdc;

    int tr = tile->getRow();
    int tc = tile->getCol();

    // if this piece is a defender
    if(isKingDefender(tile->getPiece()) && (attackBoardVal == -2))
    {
        kdr = tr - king->getRow();
        kdc = tc - king->getCol();

        // normalize them if non-zero
        if(kdr)
            kdr /= abs(kdr);
        if(kdc)
            kdc /= abs(kdc);

        if(!((kdr == dr) && (kdc == dc)) && !((-kdr == dr) && (-kdc == dc)))
        {
            return false;
        }
    }
    return true;
}