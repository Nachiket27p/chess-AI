#include "minmaxabp.h"
#include <stdlib.h>
#include <time.h>

MinMaxABP::MinMaxABP(BoardTile *(*_grid)[8][8], Piece *(*_whitePieces)[16], Piece *(*_blackPieces)[16], bool _color, bool _maxingColor, EvaluationScheme _evalSchema)
{
    game = game->getInstance();
    grid = _grid;
    whitePieces = _whitePieces;
    blackPieces = _blackPieces;
    color = _color;
    evalSchema = _evalSchema;
    maxingColor = _maxingColor;
}

int MinMaxABP::minMax(int depth, int alpha, int beta, bool maximizing, Move *bestMove)
{
    if (depth == 0 || game->hasGameEnded(maxingColor))
    {
        return evaluate();
    }

    int currEval;
    std::vector<Move> *moves = new std::vector<Move>();
    game->getMoves(moves);

    // select a random move as the best move first
    srand(time(NULL));
    int randChoice = rand() % moves->size();
    Move bm = (*moves)[randChoice];

    if (maximizing)
    {
        int maxEval = INT_MIN;
        for (auto mve : *moves)
        {
            backUpEPValue();
            makeMove(mve);
            currEval = minMax(depth - 1, alpha, beta, false, bestMove);
            unmakeMove(mve);
            restoreEPValue();
            if (currEval > maxEval)
            {
                maxEval = currEval;
                bm = mve;
            }
            // alpha-beta pruning
            alpha = std::max(alpha, currEval);
            if (alpha >= beta)
                break;
        }
        // if loop has ended then depth is about to change
        // so reset the possible enpassant value
        game->resetEnPassant(game->isWhiteTurn());

        // save the best move
        bestMove->startTileNumb = bm.startTileNumb;
        bestMove->endTileNumb = bm.endTileNumb;

        return maxEval;
    }
    else
    {
        int minEval = INT_MAX;
        for (auto mve : *moves)
        {
            backUpEPValue();
            makeMove(mve);
            currEval = minMax(depth - 1, alpha, beta, true, bestMove);
            unmakeMove(mve);
            restoreEPValue();
            if (currEval < minEval)
            {
                minEval = currEval;
                bm = mve;
            }
            // alpha-beta pruning
            beta = std::min(beta, currEval);
            if (beta <= alpha)
                break;
        }
        // if loop has ended then depth is about to change
        // so reset the possible enpassant value
        game->resetEnPassant(game->isWhiteTurn());
        // save the best move
        bestMove->startTileNumb = bm.startTileNumb;
        bestMove->endTileNumb = bm.endTileNumb;

        return minEval;
    }
}

int MinMaxABP::evaluate()
{
    int eval;
    switch (evalSchema)
    {
    case EvaluationScheme::basic:
        eval = basicEvaluate();
        break;
    case EvaluationScheme::complex:
        eval = complexEbaluate();
        break;
    default:
        eval = staticEvaluate();
        break;
    }

    return eval;
}

int MinMaxABP::staticEvaluate()
{
    // reset the scores
    whiteScore = 0;
    blackScore = 0;

    for (int i = 0; i < 16; i++)
    {
        // accumulate the white score if the piece has not been captured
        if (!(*whitePieces)[i]->isCaptured())
            whiteScore += (*whitePieces)[i]->getBasePowerValue();

        // accumulate the black score if the piece has not been captured
        if (!(*blackPieces)[i]->isCaptured())
            blackScore += (*blackPieces)[i]->getBasePowerValue();
    }
    return (whiteScore - blackScore);
}

int MinMaxABP::basicEvaluate()
{
    // reset the scores
    whiteScore = 0;
    blackScore = 0;

    Piece* currPiece;
    int r, c;

    for (int i = 0; i < 16; i++)
    {
        // accumulate the white score if the piece has not been captured
        currPiece = (*whitePieces)[i];
        if (!currPiece->isCaptured())
        {
            whiteScore += currPiece->getBasePowerValue();
            r = currPiece->getRow();
            c = currPiece->getCol();

            if(game->blackAttacks[r][c] == SINGLE_DEFENDER)
                whiteScore -= (currPiece->getBasePowerValue()/2);
            else if(game->blackAttacks[r][c] > SINGLE_DEFENDER)
                whiteScore += game->whiteAttacks[r][c];
            else
                whiteScore += (game->whiteAttacks[r][c] - game->blackAttacks[r][c]);
        }

        // accumulate the black score if the piece has not been captured
        currPiece = (*blackPieces)[i];
        if (!currPiece->isCaptured())
        {
            blackScore += currPiece->getBasePowerValue();
            r = currPiece->getRow();
            c = currPiece->getCol();

            if(game->whiteAttacks[r][c] == SINGLE_DEFENDER)
                blackScore -= (currPiece->getBasePowerValue()/2);
            else if(game->whiteAttacks[r][c] > SINGLE_DEFENDER)
                blackScore += game->blackAttacks[r][c];
            else
                blackScore += (game->blackAttacks[r][c] - game->whiteAttacks[r][c]);

        }
    }
    return (whiteScore - blackScore);
}

int MinMaxABP::complexEbaluate()
{
    // TODO
    return 0;
}

void MinMaxABP::makeMove(Move m)
{
    // TODO: potential additional steps to ensre move can be undone correctly
    simulateMove(m);
}

void MinMaxABP::unmakeMove(Move m)
{
    // TODO: use backup pieces to restore the gird/board to the state before
    //          the move performed by the makeMove function.
    BackUpMove *bUpM = backUpMoves.top();
    backUpMoves.pop();

    int row = bUpM->backUpStartPiece->getRow();
    int col = bUpM->backUpStartPiece->getCol();
    int index = bUpM->backUpStartPiece->getIndex();
    bool whiteTurn = bUpM->backUpStartPiece->isWhite();
    delete (*grid)[row][col]->getPiece();
    (*grid)[row][col]->setPiece(bUpM->backUpStartPiece);

    if (whiteTurn)
    {
        (*whitePieces)[index] = bUpM->backUpStartPiece;
    }
    else
    {
        (*blackPieces)[index] = bUpM->backUpStartPiece;
    }

    if (bUpM->backUpEndPiece == nullptr)
    {
        row = m.endTileNumb / 8;
        col = m.endTileNumb % 8;
        (*grid)[row][col]->setPiece();
    }
    else
    {
        index = bUpM->backUpEndPiece->getIndex();
        row = bUpM->backUpEndPiece->getRow();
        col = bUpM->backUpEndPiece->getCol();
        whiteTurn = bUpM->backUpEndPiece->isWhite();
        delete (*grid)[row][col]->getPiece();
        (*grid)[row][col]->setPiece(bUpM->backUpEndPiece);

        // restore the piece in piece arrays
        if (whiteTurn)
            (*whitePieces)[index] = bUpM->backUpEndPiece;
        else
            (*blackPieces)[index] = bUpM->backUpEndPiece;
    }

    if (bUpM->backUpAdditionalPiece != nullptr)
    {
        index = bUpM->backUpAdditionalPiece->getIndex();
        row = bUpM->backUpAdditionalPiece->getRow();
        col = bUpM->backUpAdditionalPiece->getCol();
        whiteTurn = bUpM->backUpAdditionalPiece->isWhite();

        // if the additional piece is a rook then a castle was performed
        // based on the rook index determine if it was a queen side
        // or king side castle. Reset d1/d8 or f1/f8.
        if(bUpM->backUpAdditionalPiece->getPieceSymbol() == rookID)
        {
            if(bUpM->backUpAdditionalPiece->getIndex() == 8)
            {
                (*grid)[row][col+3]->setPiece();
            }
            else if(bUpM->backUpAdditionalPiece->getIndex() == 15)
            {
                (*grid)[row][col-2]->setPiece();
            }
        }

        delete (*grid)[row][col]->getPiece();
        (*grid)[row][col]->setPiece(bUpM->backUpAdditionalPiece);
        if (whiteTurn)
            (*whitePieces)[index] = bUpM->backUpAdditionalPiece;
        else
            (*blackPieces)[index] = bUpM->backUpAdditionalPiece;
    }

    // rotate turn
    game->rotateTurn();
}

void MinMaxABP::simulateMove(Move m)
{
    int rowStart, colStart, rowEnd, colEnd;
    rowStart = m.startTileNumb / 8;
    colStart = m.startTileNumb % 8;
    rowEnd = m.endTileNumb / 8;
    colEnd = m.endTileNumb % 8;
    BoardTile *startTile = (*grid)[rowStart][colStart];
    BoardTile *endTile = (*grid)[rowEnd][colEnd];

    // back up the start and end tiles for when the move is undone
    BackUpMove *bUpM = new BackUpMove();
    bUpM->backUpStartPiece = new Piece(*(startTile->getPiece()));
    if (endTile->getPiece() != nullptr)
        bUpM->backUpEndPiece = new Piece(*(endTile->getPiece()));

    // the king tile number before moving
    // used to determine which side to castle and
    // which rook to move.
    char movingPieceSymbol = startTile->getPieceSymbol();
    int kingTNBefore;
    // if the piece being moved is the the king the update the king pointer
    if (movingPieceSymbol == kingID)
    {
        kingTNBefore = startTile->getTileNumber();
    }

    // check if en passant move is being performed
    bool ep = false;
    if ((movingPieceSymbol == pawnID) &&
        (endTile->getCol() != startTile->getCol()) && !endTile->isOccupied())
    {
        ep = true;
    }

    // if the tile to which this piece is being moved is
    // occupied by the opponents piece then set the piece as
    // captured
    if (endTile->isOccupied())
        endTile->getPiece()->setCaptured();

    // replace the piece on this tile
    endTile->setPiece(startTile->getPiece());
    // remove the piece from the selected tile & display the empty tile
    startTile->removePiece();
    // mark the piece as being moved
    endTile->setMoved();
    // display the piece on the new position and unhighlight other tiles
    // unhilight the unselected tiles
    game->unhighlightTiles();
    // reset the valid moves index to 0 so the validMoves array does not
    // need to be cleared
    game->resetVmIdx();
    // change the turn variable to indicate it is the opponents turn now
    game->rotateTurn();

    // Casteling
    if (movingPieceSymbol == kingID)
    {
        // Queen side castle
        if ((kingTNBefore - endTile->getTileNumber()) == 2)
        {
            // save the rook when the move is undone
            bUpM->backUpAdditionalPiece = new Piece(*((*grid)[rowEnd][colEnd - 2]->getPiece()));
            // perform rook movement
            (*grid)[rowEnd][colEnd - 2]->setMoved();
            (*grid)[rowEnd][colEnd + 1]->setPiece((*grid)[rowEnd][colEnd - 2]->getPiece());
            (*grid)[rowEnd][colEnd - 2]->removePiece();
        }
        // king side castle
        else if ((kingTNBefore - endTile->getTileNumber()) == -2)
        {
            // save the rook for when the move is undone
            bUpM->backUpAdditionalPiece = new Piece(*((*grid)[rowEnd][colEnd + 1]->getPiece()));
            // perform rook movement
            (*grid)[rowEnd][colEnd + 1]->setMoved();
            (*grid)[rowEnd][colEnd - 1]->setPiece((*grid)[rowEnd][colEnd + 1]->getPiece());
            (*grid)[rowEnd][colEnd + 1]->removePiece();
        }
    }

    // en passant
    if (movingPieceSymbol == pawnID)
    {
        game->canEnPassant(endTile);

        // if ep was set to true above then perform enpassant by
        // removing the opposings pawn.
        if (ep)
        {
            int tn = game->getEPTileNumber(!game->isWhiteTurn());
            int r = tn / 8;
            int c = tn % 8;
            // save the pawn
            bUpM->backUpAdditionalPiece = new Piece(*(*grid)[r][c]->getPiece());
            // remove the captured pawn
            (*grid)[r][c]->getPiece()->setCaptured();
            (*grid)[r][c]->removePiece();
        }
    }

    // convert pawn to queen
    if (movingPieceSymbol == pawnID)
    {
        // because turn has switched over true means its black's turn
        if (game->isWhiteTurn())
        {
            // if a black pawn has reached the opposite end conver to queen
            if (rowEnd == 7)
            {
                // At the moment just select the queen
                int pIndex = endTile->getPiece()->getIndex();
                delete endTile->getPiece();
                // TODO: Some heuristic of selectin most powerful pawn conversion
                endTile->setPiece(new Piece(false, queenID, endTile->getTileNumber(), rowEnd, colEnd, pIndex, blackPath + queenIconName));
            }
        }
        else
        {
            // if a white pawn has reached the opposite end conver to queen
            if (rowEnd == 0)
            {
                // At the moment just select the queen
                int pIndex = endTile->getPiece()->getIndex();
                delete endTile->getPiece();
                // TODO: Some heuristic of selectin most powerful pawn conversion
                endTile->setPiece(new Piece(true, queenID, endTile->getTileNumber(), rowEnd, colEnd, pIndex, whitePath + queenIconName));
            }
        }
    }
    // reset en passant
    game->resetEnPassant(!game->isWhiteTurn());
    // update the attack board
    game->updateAttackBoard();
    // scan for check after a piece has moved
    game->scanForCheck();
    // check if the game is over
    //    endTile->checkGameEnd();
    // save the move to the stack
    backUpMoves.push(bUpM);
}

void MinMaxABP::backUpEPValue()
{
    backUpEP.push(game->getEPTileNumber(game->isWhiteTurn()));
}

void MinMaxABP::restoreEPValue()
{
    game->setEPTileNumber(backUpEP.top(), game->isWhiteTurn());
    backUpEP.pop();
}
