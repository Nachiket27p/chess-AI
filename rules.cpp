#include <stack>
#include "rules.h"
#include "utils.h"
#include "piece.h"

#define SINGLE_DEFENDER 17
#define ATTACK_OCCUPIED 2
#define ATTACK_BEHIND -1
#define ATTACK_KING_BEHIND -16
#define ATTACK_SPECIAL -77

extern Theme *currentTheme;
extern Piece *whitePieces[16];
extern Piece *blackPieces[16];

Rules *Rules::instance = nullptr;

//sssssssssssssssssssssss
void Rules::setDebugWindowAccess(DebugWindow *_dbw)
{
    dbw = _dbw;
}

bool Rules::isWhiteTurn()
{
    return turn;
}

void Rules::setTurn(bool _turn)
{
    turn = _turn;
}

void Rules::rotateTurn()
{
    turn = (turn + 1) & 1;
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
    // based on what piece is being checked enforce its movement.
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

    return okToMove;
}

void Rules::scanForCheck()
{
    // use the attack grid to determine a potential for a check
    isCheck = false;
    int row, col;

    if (turn)
    {
        row = whitePieces[12]->getRow();
        col = whitePieces[12]->getCol();
        if (blackAttacks[row][col] > 0)
            setCheck();
    }
    else
    {
        row = blackPieces[12]->getRow();
        col = blackPieces[12]->getCol();
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
    int kcol = blackPieces[12]->getCol();
    // get value from white attack board for black move
    int attackBoardVal = whiteAttacks[row][col];
    // if it is white turn then the black attack board needs to be checked
    // and the white king column needs to be used.
    if (turn)
    {
        kcol = whitePieces[12]->getCol();
        attackBoardVal = blackAttacks[row][col];
    }
    // determine if this piece is blocing a check and if there
    // are multiple pieces blocking line of sight to the king
    // then this piece can move.
    bool multiDefenders = false;
    if (attackBoardVal > SINGLE_DEFENDER)
        multiDefenders = true;

    // based on the tile color move the pawn in the correct direction.
    if (tile->getPieceColor())
    {
        if ((defender && (kcol == col)) || !defender || multiDefenders)
        {
            if (row - 1 >= 0 && !grid[row - 1][col]->isOccupied())
            {
                // call helper function to see if this move is valid.
                addValidMove(grid[row - 1][col]->getTileNumber(), &ok);
            }
            if (row == 6 && !grid[5][col]->isOccupied() && !grid[4][col]->isOccupied())
            {
                // call helper function to see if this move is valid.
                addValidMove(grid[row - 2][col]->getTileNumber(), &ok);
            }
        }
        if ((row - 1 >= 0) && (col - 1 >= 0) && (!defender || (defender && (kcol != col)) || multiDefenders))
        {
            if (grid[row - 1][col - 1]->isOccupied() &&
                grid[row - 1][col - 1]->getPieceColor() != tile->getPieceColor())
            {
                // call helper function to see if this move is valid.
                addValidMove(grid[row - 1][col - 1]->getTileNumber(), &ok);
            }
        }
        if ((row - 1 >= 0) && (col + 1 <= 7) && (!defender || (defender && (kcol != col)) || multiDefenders))
        {
            if (grid[row - 1][col + 1]->isOccupied() &&
                grid[row - 1][col + 1]->getPieceColor() != tile->getPieceColor())
            {
                // call helper function to see if this move is valid.
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
                // call helper function to see if this move is valid.
                addValidMove(grid[row + 1][col]->getTileNumber(), &ok);
            }
            if (row == 1 && !grid[2][col]->isOccupied() && !grid[3][col]->isOccupied())
            {
                // call helper function to see if this move is valid.
                addValidMove(grid[row + 2][col]->getTileNumber(), &ok);
            }
        }
        if ((row + 1 <= 7) && (col - 1 >= 0) && (!defender || (defender && (kcol != col)) || multiDefenders))
        {
            if (grid[row + 1][col - 1]->isOccupied() &&
                grid[row + 1][col - 1]->getPieceColor() != tile->getPieceColor())
            {
                // call helper function to see if this move is valid.
                addValidMove(grid[row + 1][col - 1]->getTileNumber(), &ok);
            }
        }
        if ((row + 1 <= 7) && (col + 1 <= 7) && (!defender || (defender && (kcol != col)) || multiDefenders))
        {
            if (grid[row + 1][col + 1]->isOccupied() &&
                grid[row + 1][col + 1]->getPieceColor() != tile->getPieceColor())
            {
                // call helper function to see if this move is valid.
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

    // based on which turn it is select the appropriate king
    // and attack board.
    BoardTile *king = grid[blackPieces[12]->getRow()][blackPieces[12]->getCol()];
    int attackBoardVal = whiteAttacks[row][col];
    if (turn)
    {
        king = grid[whitePieces[12]->getRow()][whitePieces[12]->getCol()];
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

    int row = tile->getRow();
    int col = tile->getCol();

    // If this piece is a king defender
    if (isKingDefender(tile->getPiece()))
    {
        // get value from white attack board for black move
        int attackBoardVal = whiteAttacks[row][col];
        // if its white white turn get black attacks
        if (turn)
            attackBoardVal = blackAttacks[row][col];
        // if the knight is a single defender then return false
        // because it cannot move.
        if (attackBoardVal == SINGLE_DEFENDER)
            return ok;
    }

    int rowOrig = tile->getRow();
    int colOrig = tile->getCol();
    // 2d array representing the offsets to check.
    int pos[8][2] = {{-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}};
    int dr, dc;

    // loop through 'pos' and check for balud moves.
    for (int i = 0; i < 8; i++)
    {
        dr = pos[i][0];
        dc = pos[i][1];
        row = rowOrig + dr;
        col = colOrig + dc;
        if (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
        {
            // if the piece on (row,col) is different color to this one or
            // the tile is unoccupied then the knight can move to this position.
            if (grid[row][col]->getPieceColor() != tile->getPieceColor() ||
                !grid[row][col]->isOccupied())
            {
                // call helper function to add the move if it is valid under the
                // current game state.
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
    // based on whose turn it is select the appropriate king
    // and attack board.
    BoardTile *king = grid[blackPieces[12]->getRow()][blackPieces[12]->getCol()];
    int attackBoardVal = whiteAttacks[row][col];
    if (turn)
    {
        king = grid[whitePieces[12]->getRow()][whitePieces[12]->getCol()];
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
    // based whose turn it is select the appropriate king
    // and attack board.
    BoardTile *king = grid[blackPieces[12]->getRow()][blackPieces[12]->getCol()];
    int attackBoardVal = whiteAttacks[row][col];
    if (turn)
    {
        king = grid[whitePieces[12]->getRow()][whitePieces[12]->getCol()];
        attackBoardVal = blackAttacks[row][col];
    }

    // iterator through all the direction except 0,0
    for (int dr = -1; dr <= 1; dr++)
    {
        for (int dc = -1; dc <= 1; dc++)
        {
            // if dr and dc both are not 0 then call helper function to
            // add move to list of valid moves.
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
    // check the row and column are within the bounds
    while (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
    {
        // if the position on the grid is not occupied then call the helper
        // function to check and add valid move to list of valid moves.
        if (!grid[row][col]->isOccupied())
        {
            addValidMove(grid[row][col]->getTileNumber(), ok);
        }
        // else if the color of the piece at (row,col) is same as this pice
        // then it cannot be a valid move.
        else if (grid[row][col]->getPieceColor() == tile->getPieceColor())
        {
            break;
        }
        // if the color of the piece at (row,col) is different then
        // call the helper function to check if this move is valid and
        // add to the list of valid moves.
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
        // if the row with the offset dr is within bounds
        if (WITHIN_BOUNDS(rowOrig + dr))
        {
            for (int dc = -1; dc <= 1; dc++)
            {
                // if dr and dc are not 0 and
                // the column with the offset dc is within bounds
                if (!(dr == 0 && dc == 0) && WITHIN_BOUNDS(colOrig + dc))
                {
                    row = rowOrig + dr;
                    col = colOrig + dc;

                    // if the grid position (row,col) is not occupied or
                    // the color of this piece does not match the one at (row,col).
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

inline void Rules::addValidMove(int tileNumber, bool *ok)
{
    int vmIdxbefore = vmIdx;
    if (isCheck)
    {
        // if there are more than pieces causing check then
        // nothing except the king can move
        if (checkPieces.size() > 1)
        {
            *ok = false;
            return;
        }

        // if ther is only one piece causing check
        // get the row and col position.
        int checkRow = checkPieces[0].row;
        int checkCol = checkPieces[0].col;

        // the knight can only move if it is able to attack the
        if (grid[checkRow][checkCol]->getPieceSymbol() == knightID)
        {
            if(tileNumber == grid[checkRow][checkCol]->getTileNumber())
                validMoves[vmIdx++] = tileNumber;
        }
        else
        {
            // determine the direction towards the king from the
            // piece causing the check.
            int dr = 0;
            int dc = 0;
            int diffRow, diffCol;
            if (turn)
            {
                diffRow = whitePieces[12]->getRow() - checkRow;
                diffCol = whitePieces[12]->getCol() - checkCol;
            }
            else
            {
                diffRow = blackPieces[12]->getRow() - checkRow;
                diffCol = blackPieces[12]->getCol() - checkCol;
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
    int vmIdxbefore = vmIdx;
    if (turn)
    {
        if (blackAttacks[row][col] <= 0)
            validMoves[vmIdx++] = tileNumber;
    }
    else
    {
        if (whiteAttacks[row][col] <= 0)
            validMoves[vmIdx++] = tileNumber;
    }

    // if a valid move was added return true otherwise return false
    if (vmIdxbefore != vmIdx)
    {
        *ok = true;
    }
}

bool Rules::canCastle(BoardTile *tile)
{
    bool ok = false;
    Piece *rookQ = blackPieces[8];
    Piece *rookK = blackPieces[15];
    int(*attackBoard)[8] = whiteAttacks;

    // get the correct rooks based on whose turn it is.
    if (turn)
    {
        rookQ = whitePieces[8];
        rookK = whitePieces[15];
        attackBoard = blackAttacks;
    }

    int kRow = tile->getRow();
    int kCol = tile->getCol();

    // the king must have not moved.
    if (!tile->hasMoved() && !isCheck)
    {
        bool cc = true;
        // king side castle, the rook must not have moved
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

        // queen side castle, the rook must not have move
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
    // if it's white turn then check the white enpassant flags
    if (turn)
    {
        // whiteEPL and whiteEPR contrin the tile number of the
        // pawn next to the one being checked.
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
    if (turn)
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
    if (turn)
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

    if ((turn && (row == 3)) || (!turn && (row == 4)))
    {
        BoardTile *otherPawn = grid[row][col + 1];
        // Left en passant
        // should check to see if the pawn is of the opponent but
        // it is impossible to have the same side pawn move there in 1 move.
        if (((col + 1) < 8) && otherPawn->isOccupied() && (turn == otherPawn->getPieceColor()) &&
            (otherPawn->getPieceSymbol() == pawnID) && tile->hasMoved() == 1)
        {
            if (turn)
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
        if (((col - 1) >= 0) && otherPawn->isOccupied() && (turn == otherPawn->getPieceColor()) &&
            (otherPawn->getPieceSymbol() == pawnID) && tile->hasMoved() == 1)
        {
            if (turn)
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

void Rules::resetEnPassant(bool whiteTurn)
{
    // reset enpassant flags
    if (whiteTurn)
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

int Rules::getEPTileNumber(bool whiteTurn)
{
    // returns the appropriate enpassant flag
    // if it ends with 'L' - Left side
    // if it ends with 'R' - Right side
    if (whiteTurn)
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

int Rules::hasGameEnded(bool whiteTurn)
{
    // based on if whose turn it is get the appropriate
    // array of pieces.
    Piece *(*pcs) = blackPieces;
    if (whiteTurn)
    {
        pcs = whitePieces;
    }

    // save the current value of vmidx as it will be modified
    // when determining if any piece can move in the for loop below.
    // it will be restored beforning returning from this function.
    int oldVmidx = vmIdx;
    vmIdx = 0;
    int row, col;
    // initially set the game state to stale mate.
    // if there is a check then it must be a check mate.
    int gameEnded = STALE_MATE;
    if (isCheck)
    {
        gameEnded = CHECK_MATE;
    }

    // now iterate through all pieces on the board and
    // if any of them can move then it cannot be a check mate
    // or stale mate. Set the gameEnded value to 0 to indicate
    // the game will continue.
    for (int i = 0; i < 16; i++)
    {
        // if this piece is not on the board then skip it
        if (pcs[i]->isCaptured())
            continue;

        row = pcs[i]->getRow();
        col = pcs[i]->getCol();
        canMove(grid[row][col]);
        if (vmIdx)
        {
            gameEnded = 0;
            break;
        }
    }

    // restore the original value.
    vmIdx = oldVmidx;

    return gameEnded;
}

void Rules::highlightTiles()
{
    BoardTile *tile;
    // iterate through the validMoves array upt to the index 'vmIdx'
    for (int i = 0; i < vmIdx; i++)
    {
        tile = grid[validMoves[i] / 8][validMoves[i] & 7];

        if (tile->isOccupied())
        {
            if (tile->getPieceColor())
            {
                tile->setStyleSheet(QString("QLabel {background-color: ") +
                                    currentTheme->getAttackBackground() +
                                    QString(" color: ") +
                                    currentTheme->getHoverColorWhite() +
                                    QString("}"));
            }
            else
            {
                tile->setStyleSheet(QString("QLabel {background-color: ") +
                                    currentTheme->getAttackBackground() +
                                    QString(" color: ") +
                                    currentTheme->getHoverColorBlack() +
                                    QString("}"));
            }
        }
        else
            tile->setStyleSheet(QString("QLabel {background-color: ") +
                                currentTheme->getHoverBackground() +
                                QString("}"));
    }
}

void Rules::unhighlightTiles()
{
    // unhilight all the tiles on the board using the validMoves array
    for (int i = 0; i < vmIdx; i++)
        grid[validMoves[i] / 8][validMoves[i] & 7]->displayTileColor();
}

bool Rules::isValidMove(int tileNumb)
{
    for(int i = 0; i < vmIdx; i++)
    {
        if(tileNumb == validMoves[i])
            return true;
    }
    return false;
}

void Rules::resetVmIdx()
{
    vmIdx = 0;
}

void Rules::getMoves(std::vector<Move> &moves, int startTileNumb)
{
    for(int i = 0; i < vmIdx; i++)
    {
        moves.push_back(Move{startTileNumb, validMoves[i]});
    }
}

inline void Rules::updateAttackBoardHelper(Piece *pieces[16], int attackBoard[8][8], int pieceColor)
{
    for (int x = 0; x < 16; x++)
    {
        Piece *p = pieces[x];

        // if this piece is not on the board then skip it
        if (p->isCaptured())
            continue;

        int pr = p->getRow();
        int pc = p->getCol();

        // used to indexing pawns
        int pawndr = 1;
        if (pieceColor)
            pawndr = -1;

        // based on the symbol on switch to the appropriate case
        // to update the attack board values which are used to determine
        // valid moves and check.
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
            // 2d array representing the position relative to the position of the knight
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
            // there is no break because the queen can also move in the
            // directions the bishop can. The bishops moves are a subset
            // of the queen.

        case bishopID:
            attackeRBQHelper(pr, pc, +1, +1, pieceColor, attackBoard);
            attackeRBQHelper(pr, pc, -1, -1, pieceColor, attackBoard);
            attackeRBQHelper(pr, pc, +1, -1, pieceColor, attackBoard);
            attackeRBQHelper(pr, pc, -1, +1, pieceColor, attackBoard);
            break;
        case kingID:
        {
            // double loop to iterate through the 8 tiles around the king
            // because these are the directions the king can move in.
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
                            attackBoard[pr + i][pc + j]++;
                        }
                    }
                }
            }
            break;
        }
        default:
            break;
        }
    }
}

inline void Rules::attackeRBQHelper(int row, int col, int dr, int dc, bool pieceColor, int attackGrid[8][8])
{
    // this stack is used to keep track of the previous values on
    // the attack board in the case that there was a previous
    // value whih holds a higher president over the new one being set.
    std::stack<int> savedAttackValues;

    // this flag is used to determine if the tiles being examined
    // are behind another piece. This is uesful when trying to determine
    // if backtracking is needed when the opponents king is found.
    int behind = 0;
    row += dr;
    col += dc;

    while (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
    {
        // save the current attack value in the case of backtracking
        savedAttackValues.push(attackGrid[row][col]);

        if (grid[row][col]->isOccupied())
        {
             // only back track if a piece was detected in front of the king
            if ((grid[row][col]->getPieceSymbol() == kingID) && behind)
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
                while (behind)
                {
                    // pop the previous values off the stack.
                    oldAV = savedAttackValues.top();
                    savedAttackValues.pop();

                    if (grid[nrow][ncol]->isOccupied())
                    {
                        dval--;
                    }
                    // if the old value is positive then restore it
                    // because it means there is a direct attack on this tile.
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
                    behind--;
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

            // if the piece is same color then no need to
            // keep checking in this direction
            if (grid[row][col]->getPieceColor() == pieceColor)
            {
                attackGrid[row][col]++;
                break;
            }
            else
            {
                if (savedAttackValues.top() >= 0)
                    attackGrid[row][col]++;
                row += dr;
                col += dc;

                // if behind is more than 0 then keep incrementing
                // else set behind to 1 to indicate this is the first
                // piece encountered
                if(behind)
                    behind++;
                else
                    behind = 1;

                continue;
            }
        }

        // if this tile is not occupied but behing is more then 0
        // it means a piece was encountered before and the counter
        // needs to be incremented incase the king is located and
        // backtracking is required.
        if(behind)
            behind++;

        attackGrid[row][col]++;

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

    // based on the turn get the appropriate king position.
    if (turn)
    {
        row = whitePieces[12]->getRow();
        col = whitePieces[12]->getCol();
    }
    else
    {
        row = blackPieces[12]->getRow();
        col = blackPieces[12]->getCol();
    }

    char checkPiece;

    // check north
    if (scanCheckHelper(row - 1, col, -1, 0, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == rookID) || (checkPiece == queenID) || (checkPiece == pawnID)))
    {
        setCheck();
        // return;
    }

    // check north-east
    if (scanCheckHelper(row - 1, col + 1, -1, 1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == bishopID) || (checkPiece == queenID) || (checkPiece == pawnID)))
    {
        setCheck();
        // return;
    }

    // check east
    if (scanCheckHelper(row, col + 1, 0, 1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == rookID) || (checkPiece == queenID) || (checkPiece == pawnID)))
    {
        setCheck();
        // return;
    }

    // check south-east
    if (scanCheckHelper(row + 1, col + 1, 1, 1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == bishopID) || (checkPiece == queenID) || (checkPiece == pawnID)))
    {
        setCheck();
        // return;
    }

    // check south
    if (scanCheckHelper(row + 1, col, 1, 0, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == rookID) || (checkPiece == queenID) || (checkPiece == pawnID)))
    {
        setCheck();
        // return;
    }

    // check south-west
    if (scanCheckHelper(row + 1, col - 1, 1, -1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == bishopID) || (checkPiece == queenID) || (checkPiece == pawnID)))
    {
        setCheck();
        // return;
    }

    // check west
    if (scanCheckHelper(row, col - 1, -1, 0, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == rookID) || (checkPiece == queenID) || (checkPiece == pawnID)))
    {
        setCheck();
        // return;
    }

    // check north-west
    if (scanCheckHelper(row - 1, col - 1, -1, -1, &checkPiece) && (checkPiece != '\0') &&
        ((checkPiece == bishopID) || (checkPiece == queenID) || (checkPiece == pawnID)))
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
}

inline bool Rules::scanCheckHelper(int row, int col, int dr, int dc, char *checkPiece)
{
    BoardTile *curr;
    int distanceFromKing = 1;
    bool correctPawnDir = false;
    if(dc != 0)
    {
        if(turn)
        {
            if(dr == -1)
                correctPawnDir = true;
        }
        else if (dr == 1)
        {
            correctPawnDir = true;
        }
    }

    // while the position (row,col) being examined is within bounds
    // keep searcning in the direction (dr,dc).
    while (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
    {
        curr = grid[row][col];
        if (curr->isOccupied())
        {
            // if the color of the piece located on the
            // tile being examined is different to the turn
            // then save this piece as one causing check.
            if (curr->getPieceColor() != turn)
            {
                // if the piece detected is a pawn then it must
                // be only one tile away and has to be facing the
                // correct direction
                if(curr->getPieceSymbol() == pawnID)
                {
                    if(correctPawnDir && (distanceFromKing == 1))
                    {
                        *checkPiece = curr->getPieceSymbol();
                        checkPieces.push_back(checkPos{row, col});
                        return true;
                    }

                    *checkPiece = '\0';
                    return false;
                }
                else
                {
                    *checkPiece = curr->getPieceSymbol();
                    checkPieces.push_back(checkPos{row, col});
                    return true;
                }
            }
            else
            {
                // if the same color is detected then set the
                // check piece as null and return false.
                *checkPiece = '\0';
                return false;
            }
        }
        row += dr;
        col += dc;
//        curr = grid[row][col];
        distanceFromKing++;
    }
    // also if no piece is found then set the
    *checkPiece = '\0';
    return false;
}

bool Rules::checkKnight(int row, int col)
{
    // the row and column is the position of the king on the board
    // so checking all the knight attack positions around the king
    // allows to determine if a knight is causing a check.
    if (checkKnightHelper(row - 2, col + 1, knightID) || checkKnightHelper(row - 1, col + 2, knightID) ||
        checkKnightHelper(row + 1, col + 2, knightID) || checkKnightHelper(row + 2, col + 1, knightID) ||
        checkKnightHelper(row + 2, col - 1, knightID) || checkKnightHelper(row + 1, col - 2, knightID) ||
        checkKnightHelper(row - 1, col - 2, knightID) || checkKnightHelper(row - 2, col - 1, knightID))
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline bool Rules::checkKnightHelper(int row, int col, char pieceType)
{
    // if the position (row,col) is within bounds then
    // check if the position contains a piece type
    // provided in the parameter.
    if (WITHIN_BOUNDS(row) && WITHIN_BOUNDS(col))
    {
        BoardTile *curr = grid[row][col];
        if ((curr->getPieceSymbol() == pieceType) && (curr->getPieceColor() != turn))
        {
            checkPieces.push_back(checkPos{row, col});
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
    // get the appropriate king defender vector
    // based on the turn
    std::vector<Piece *> kd;
    if (turn)
    {
        kd = whiteKingDefenders;
    }
    else
    {
        kd = blackKingDefenders;
    }

    // if the piece passed in is also in the vector then
    // it is potentially preventing a check.
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
