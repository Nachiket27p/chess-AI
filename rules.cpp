#include <stack>
#include "rules.h"
#include "utils.h"
#include "piece.h"

#define SINGLE_DEFENDER 17
#define ATTACK_OCCUPIED 2
#define ATTACK_BEHIND -1
#define ATTACK_KING_BEHIND -16
#define ATTACK_SPECIAL -77

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
//        if (blackAttacks[row][col] == ATTACK_OCCUPIED)
        if (blackAttacks[row][col] > 0)
            setCheck();
    }
    else
    {
        row = blackKing->getRow();
        col = blackKing->getCol();
//        if (whiteAttacks[row][col] == ATTACK_OCCUPIED)
        if (whiteAttacks[row][col] > 0)
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
    // check en passant
    enPassant(tile->getTileNumber());

    bool ok = false;
    int row = tile->getRow();
    int col = tile->getCol();

    // If this piece is a king defender
    bool defender = isKingDefender(tile->getPiece());
    int kcol = blackKing->getCol();
    // get value from white attack board for black move
    int attackBoardVal = whiteAttacks[row][col];

    if (isWhiteTurn)
    {
        kcol = whiteKing->getCol();
        attackBoardVal = blackAttacks[row][col];
    }
    bool multiDefenders = false;


    if (attackBoardVal > SINGLE_DEFENDER)
        multiDefenders = true;

    if (tile->getPieceColor())
    {
        if ((defender && (kcol == col)) || !defender || multiDefenders)
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
        if ((defender && (kcol == col)) || !defender || multiDefenders)
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

    BoardTile *king = blackKing;
    int attackBoardVal = whiteAttacks[row][col];
    if (isWhiteTurn)
    {
        king = whiteKing;
        attackBoardVal = blackAttacks[row][col];
    }

    // check north
    int dr = -1;
    int dc = 0;
    if (canKingDefenderMove(dr, dc, tile, king, attackBoardVal))
        enforceRBQHelper(row + dr, col + dc, dr, dc, &ok, tile);

    // chek east
    dr = 0;
    dc = 1;
    if (canKingDefenderMove(dr, dc, tile, king, attackBoardVal))
        enforceRBQHelper(row + dr, col + dc, dr, dc, &ok, tile);

    // check south
    dr = 1;
    dc = 0;
    if (canKingDefenderMove(dr, dc, tile, king, attackBoardVal))
        enforceRBQHelper(row + dr, col + dc, dr, dc, &ok, tile);

    // check west
    dr = 0;
    dc = -1;
    if (canKingDefenderMove(dr, dc, tile, king, attackBoardVal))
        enforceRBQHelper(row + dr, col + dc, dr, dc, &ok, tile);

    return ok;
}

bool Rules::enforceKnight(BoardTile *tile)
{
    bool ok = false;
    // if the knight is a protecting the king currently it cannot move
    if (isKingDefender(tile->getPiece()))
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

    BoardTile *king = blackKing;
    int attackBoardVal = whiteAttacks[row][col];
    if (isWhiteTurn)
    {
        king = whiteKing;
        attackBoardVal = blackAttacks[row][col];
    }

    // check north-east
    int dr = -1;
    int dc = 1;
    if (canKingDefenderMove(dr, dc, tile, king, attackBoardVal))
        enforceRBQHelper(row + dr, col + dc, dr, dc, &ok, tile);

    // check south-east
    dr = 1;
    dc = 1;
    if (canKingDefenderMove(dr, dc, tile, king, attackBoardVal))
        enforceRBQHelper(row + dr, col + dc, dr, dc, &ok, tile);

    // check south-west
    dr = 1;
    dc = -1;
    if (canKingDefenderMove(dr, dc, tile, king, attackBoardVal))
        enforceRBQHelper(row + dr, col + dc, dr, dc, &ok, tile);

    // check norh-west
    dr = -1;
    dc = -1;
    if (canKingDefenderMove(dr, dc, tile, king, attackBoardVal))
        enforceRBQHelper(row + dr, col + dc, dr, dc, &ok, tile);

    return ok;
}

bool Rules::enforceQueen(BoardTile *tile)
{
    bool ok = false;
    int row = tile->getRow();
    int col = tile->getCol();

    BoardTile *king = blackKing;
    int attackBoardVal = whiteAttacks[row][col];
    if (isWhiteTurn)
    {
        king = whiteKing;
        attackBoardVal = blackAttacks[row][col];
    }

    for (int dr = -1; dr <= 1; dr++)
    {
        for (int dc = -1; dc <= 1; dc++)
        {
            if (!((dr == 0) && (dc == 0)))
            {
                if (canKingDefenderMove(dr, dc, tile, king, attackBoardVal))
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
    // check casteling
    canCastle(tile);

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
        // if there are more than pieces causing check
        if (checkPieces.size() > 1)
        {
            *ok = false;
            return;
        }

        // for (int i = 0; i < checkPieces.size(); i++)
        // {
        //     int checkRow = checkPieces[i].row;
        //     int checkCol = checkPieces[i].col;

        int checkRow = checkPieces[0].row;
        int checkCol = checkPieces[0].col;

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
        // } // commented out for loop

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

bool Rules::canCastle(BoardTile *tile)
{
    bool ok = false;
    Piece *rookQ = blackPieces[8];
    Piece *rookK = blackPieces[15];
    int(*attackBoard)[8] = whiteAttacks;

    if (isWhiteTurn)
    {
        rookQ = whitePieces[8];
        rookK = whitePieces[15];
        attackBoard = blackAttacks;
    }

    int kRow = tile->getRow();
    int kCol = tile->getCol();

    if (!tile->hasMoved() && !isCheck)
    {
        bool cc = true;
        // king side castle
        if (!rookK->hasMoved())
        {
            for (int dc = -1; dc >= -3; dc--)
            {
                if (grid[kRow][kCol + dc]->isOccupied() ||
                    (attackBoard[kRow][kCol + dc] == 1))
                {
                    cc = false;
                }
            }
            if (cc)
            {
                validMoves[vmIdx++] = tile->getTileNumber() - 2;
                ok = true;
            }
        }

        // queen side castle
        if (!rookQ->hasMoved())
        {
            cc = true;
            for (int dc = 1; dc <= 2; dc++)
            {
                if (grid[kRow][kCol + dc]->isOccupied() ||
                    (attackBoard[kRow][kCol + dc] == 1))
                {
                    cc = false;
                }
            }
            if (cc)
            {
                validMoves[vmIdx++] = tile->getTileNumber() + 2;
                ok = true;
            }
        }
    }
    return ok;
}

bool Rules::enPassant(int tileNumb)
{
    bool ok = false;

    if (isWhiteTurn)
    {
        if (whiteEPL == (tileNumb - 1))
        {
            validMoves[vmIdx++] = tileNumb - 8 - 1;
            ok = true;
        }
        if (whiteEPR == (tileNumb + 1))
        {
            validMoves[vmIdx++] = tileNumb - 8 + 1;
            ok = true;
        }
    }
    else
    {
        if (blackEPL == (tileNumb + 1))
        {
            validMoves[vmIdx++] = tileNumb + 8 + 1;
            ok = true;
        }
        if (blackEPR == (tileNumb - 1))
        {
            validMoves[vmIdx++] = tileNumb + 8 - 1;
            ok = true;
        }
    }

    return ok;
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
    if (isWhiteTurn)
    {
        whiteKingDefenders.clear();
    }
    else
    {
        blackKingDefenders.clear();
    }
}

void Rules::canEnPassant(BoardTile *tile)
{
    int row = tile->getRow();
    int col = tile->getCol();

    if ((isWhiteTurn && (row == 3)) || (!isWhiteTurn && (row == 4)))
    {
        BoardTile *otherPawn = grid[row][col + 1];
        // Left en passant
        // should check to see if the pawn is of the opponent but
        // it is impossible to have the same side pawn move there in 1 move.
        if (((col + 1) < 8) && otherPawn->isOccupied() && (isWhiteTurn == otherPawn->getPieceColor()) &&
            (otherPawn->getPieceSymbol() == pawnID) && tile->hasMoved() == 1)
        {
            if (isWhiteTurn)
            {
                whiteEPL = tile->getTileNumber();
            }
            else
            {
                blackEPR = tile->getTileNumber();
            }
        }

        // Right en passant
        // should check to see if the pawn is of the opponent but
        // it is impossible to have the same side pawn move there in 1 move.
        otherPawn = grid[row][col - 1];
        if (((col - 1) >= 0) && otherPawn->isOccupied() && (isWhiteTurn == otherPawn->getPieceColor()) &&
            (otherPawn->getPieceSymbol() == pawnID) && tile->hasMoved() == 1)
        {
            if (isWhiteTurn)
            {
                whiteEPR = tile->getTileNumber();
            }
            else
            {
                blackEPL = tile->getTileNumber();
            }
        }
    }
}

void Rules::resetEnPassant(bool white)
{
    if (white)
    {
        whiteEPL = 0;
        whiteEPR = 0;
    }
    else
    {
        blackEPL = 0;
        blackEPR = 0;
    }
}

int Rules::getEPTileNumber(bool white)
{
    if (white)
    {
        if (whiteEPL)
            return whiteEPL;
        if (whiteEPR)
            return whiteEPR;
    }
    else
    {
        if (blackEPL)
            return blackEPL;
        if (blackEPR)
            return blackEPR;
    }
    return -1;
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
        if (pieceColor)
            pawndr = -1;

        switch (p->getPieceSymbol())
        {
        case pawnID:
            if (WITHIN_BOUNDS(pr + pawndr))
            {
                if (WITHIN_BOUNDS(pc + 1))
                {
                    attackBoard[pr + pawndr][pc + 1]++;
                }

                if (WITHIN_BOUNDS(pc - 1))
                {
                    attackBoard[pr + pawndr][pc - 1]++;
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
                    attackBoard[pr + dr][pc + dc]++;
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
                        if (!(i == 0 && j == 0) && WITHIN_BOUNDS(pc + j) &&
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
    std::stack<int> savedAttackValues;
    bool behind = false;
    row += dr;
    col += dc;

    while (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
    {
        // save the current attack value in the case of backtracking
        savedAttackValues.push(attackGrid[row][col]);

        if (grid[row][col]->isOccupied())
        {
            if ((grid[row][col]->getPieceSymbol() == kingID) && (attackGrid[row - dr][col - dc] == ATTACK_BEHIND))
            {
                int dval = ATTACK_KING_BEHIND;
                int oldAV = savedAttackValues.top();
                savedAttackValues.pop();
                if (oldAV > 0)
                {
                    attackGrid[row][col] = oldAV;
                }
                else
                {
                    attackGrid[row][col] = dval--;
                }
                // move in the opposite direction and populate attack grid
                int nrow = row - dr;
                int ncol = col - dc;
                while (attackGrid[nrow][ncol] == ATTACK_BEHIND)
                {
                    oldAV = savedAttackValues.top();
                    savedAttackValues.pop();

                    if (grid[nrow][ncol]->isOccupied())
                    {
                        dval--;
                    }
                    // if the old value is positive then restore it
                    if (oldAV > 0)
                    {
                        attackGrid[nrow][ncol] = oldAV;
                    }
                    else
                    {
                        attackGrid[nrow][ncol] = 0;
                    }

                    nrow -= dr;
                    ncol -= dc;
                }

                // update the attack value to be positive
                attackGrid[nrow][ncol] = abs(dval);
                // save the piece in the list of kings defenders
                if (pieceColor)
                {
                    blackKingDefenders.push_back(grid[nrow][ncol]->getPiece());
                }
                else
                {
                    whiteKingDefenders.push_back(grid[nrow][ncol]->getPiece());
                }

                // update the row and column and continue
                row += dr;
                col += dc;
                continue;
            }

            // if the piece is sam color break
            if (grid[row][col]->getPieceColor() == pieceColor)
            {
                break;
            }
            else if (!behind)
            {
                if(savedAttackValues.top() >= 0)
                    attackGrid[row][col]++;
                row += dr;
                col += dc;
                behind = true;
                continue;
            }
        }

        // if the attack grid value is negative then overwrite it
        if (behind)
        {
            attackGrid[row][col] = ATTACK_BEHIND;
        }
        else if (attackGrid[row][col] >= 0)
        {
            attackGrid[row][col]++;
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
    checkPieces.clear();

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
        // return;
    }

    // check north-east
    if (scanCheckHelper(row - 1, col + 1, -1, 1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == bishopID) || (checkPiece == queenID)))
    {
        setCheck();
        // return;
    }

    // check east
    if (scanCheckHelper(row, col + 1, 0, 1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == rookID) || (checkPiece == queenID)))
    {
        setCheck();
        // return;
    }

    // check south-east
    if (scanCheckHelper(row + 1, col + 1, 1, 1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == bishopID) || (checkPiece == queenID)))
    {
        setCheck();
        // return;
    }

    // check south
    if (scanCheckHelper(row + 1, col, 1, 0, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == rookID) || (checkPiece == queenID)))
    {
        setCheck();
        // return;
    }

    // check south-west
    if (scanCheckHelper(row + 1, col - 1, 1, -1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == bishopID) || (checkPiece == queenID)))
    {
        setCheck();
        // return;
    }

    // check west
    if (scanCheckHelper(row, col - 1, -1, 0, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == rookID) || (checkPiece == queenID)))
    {
        setCheck();
        // return;
    }

    // check north-west
    if (scanCheckHelper(row - 1, col - 1, -1, -1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == bishopID) || (checkPiece == queenID)))
    {
        setCheck();
        // return;
    }

    // check knght
    if (checkKnight(row, col))
    {
        setCheck();
        // return;
    }

    // check pawn
    if (isWhiteTurn)
    {
        if (checkTile(row + 1, col - 1, pawnID) || checkTile(row + 1, col + 1, pawnID))
        {
            setCheck();
            // return;
        }
    }
    else if (checkTile(row - 1, col - 1, pawnID) || checkTile(row - 1, col + 1, pawnID))
    {
        setCheck();
        // return;
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
                // checkRow = row;
                // checkCol = col;
                checkPieces.push_back(checkPos{row, col});
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

bool Rules::checkKnight(int row, int col)
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

bool Rules::isKingDefender(Piece *p)
{
    std::vector<Piece *> kd;
    if (isWhiteTurn)
    {
        kd = whiteKingDefenders;
    }
    else
    {
        kd = blackKingDefenders;
    }

    int kSize = kd.size();
    for (int i = 0; i < kSize; i++)
    {
        if (p == kd[i])
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
    if (isKingDefender(tile->getPiece()) && (attackBoardVal == SINGLE_DEFENDER))
    {
        kdr = tr - king->getRow();
        kdc = tc - king->getCol();

        // normalize them if non-zero
        if (kdr)
            kdr /= abs(kdr);
        if (kdc)
            kdc /= abs(kdc);

        if (!((kdr == dr) && (kdc == dc)) && !((-kdr == dr) && (-kdc == dc)))
        {
            return false;
        }
    }
    return true;
}
