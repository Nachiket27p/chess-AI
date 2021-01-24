#include "minmaxabp.h"
#include <stdlib.h>
#include <time.h>

MinMaxABP::MinMaxABP(BoardTile *(*_grid)[8 * 8], Piece *(*_whitePieces)[16], Piece *(*_blackPieces)[16], bool _maxingColor, int depth, EvaluationScheme _evalSchema)
{
    game = game->getInstance();
    grid = _grid;
    whitePieces = _whitePieces;
    blackPieces = _blackPieces;
    //    color = _color;
    evalSchema = _evalSchema;
    searchDepth = depth;
    maxingColor = _maxingColor;
}

void MinMaxABP::minMax(bool maximizing, Move *bestMove)
{
    int alpha = INT_MIN;
    int beta = INT_MAX;
    minMaxHelper(searchDepth, alpha, beta, maximizing, bestMove);
}

MinMaxABP::~MinMaxABP()
{
}

int MinMaxABP::minMaxHelper(uint depth, int alpha, int beta, bool maximizing, Move *bestMove)
{
    //    debugCount++;
    //    if(debugCount == 158564)
    //        int d = 0;
    if (depth == 0 || game->hasGameEnded(maximizing))
        return evaluate();

    int currEval;
    std::vector<Move> *moves = new std::vector<Move>();
    game->getMoves(moves);

    if (moves->size() == 0)
        return evaluate();

    // select a random move as the best move first
    srand(time(NULL));
    uint randChoice = rand() % moves->size();
    Move bm = (*moves)[randChoice];

    if (maximizing)
    {
        int maxEval = INT_MIN;
        for (auto mve : *moves)
        {
            backUpEPValue();
            makeMove(mve, depth, maximizing);
            currEval = minMaxHelper(depth - 1, alpha, beta, false, bestMove);
            unmakeMove(mve, depth, maximizing);
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
            makeMove(mve, depth, maximizing);
            currEval = minMaxHelper(depth - 1, alpha, beta, true, bestMove);
            unmakeMove(mve, depth, maximizing);
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

    Piece *currPiece;
    uint attackIdx;
    for (int i = 0; i < 16; i++)
    {
        // accumulate the white score if the piece has not been captured
        currPiece = (*whitePieces)[i];
        if (!currPiece->isCaptured())
        {
            attackIdx = currPiece->getTileNumber();
            whiteScore += currPiece->getBasePowerValue();
            if (i != 12)
            {
                if (game->blackAttacks[attackIdx] == SINGLE_DEFENDER)
                    whiteScore -= (currPiece->getBasePowerValue() / 2);
                else if (game->blackAttacks[attackIdx] > SINGLE_DEFENDER)
                    whiteScore += game->whiteAttacks[attackIdx];
                else
                    whiteScore += (game->whiteAttacks[attackIdx] - game->blackAttacks[attackIdx]);
            }
        }

        // accumulate the black score if the piece has not been captured
        currPiece = (*blackPieces)[i];
        if (!currPiece->isCaptured())
        {
            attackIdx = currPiece->getTileNumber();
            blackScore += currPiece->getBasePowerValue();
            if (i != 12)
            {
                if (game->whiteAttacks[attackIdx] == SINGLE_DEFENDER)
                    blackScore -= (currPiece->getBasePowerValue() / 2);
                else if (game->whiteAttacks[attackIdx] > SINGLE_DEFENDER)
                    blackScore += game->blackAttacks[attackIdx];
                else
                    blackScore += (game->blackAttacks[attackIdx] - game->whiteAttacks[attackIdx]);
            }
        }
    }
    return (whiteScore - blackScore);
}

int MinMaxABP::complexEbaluate()
{
    // TODO
    return 0;
}

void MinMaxABP::makeMove(Move m, int depth, bool maximizing)
{
    //    // save the attack grid
    //    if (maximizing)
    //        memcpy(backUPAttackGrids[depth], game->blackAttacks, 8 * 8 * sizeof(int));
    //    else
    //        memcpy(backUPAttackGrids[depth], game->whiteAttacks, 8 * 8 * sizeof(int));

    // TODO: potential additional steps to ensre move can be undone correctly
    simulateMove(m);
}

void MinMaxABP::unmakeMove(Move m, int depth, bool maximizing)
{
    // TODO: use backup pieces to restore the gird/board to the state before
    //          the move performed by the makeMove function.
    BackUpMove *bUpM = backUpMoves.top();
    backUpMoves.pop();

    uint gridIdx = bUpM->backUpStartPiece->getTileNumber();

    uint pieceIdx = bUpM->backUpStartPiece->getIndex();

    bool whiteTurn = bUpM->backUpStartPiece->isWhite();

    delete (*grid)[gridIdx]->getPiece();

    (*grid)[gridIdx]->setPiece(bUpM->backUpStartPiece);

    if (whiteTurn)
    {
        (*whitePieces)[pieceIdx] = bUpM->backUpStartPiece;
    }
    else
    {
        (*blackPieces)[pieceIdx] = bUpM->backUpStartPiece;
    }

    if (bUpM->backUpEndPiece == nullptr)
    {
        (*grid)[m.endTileNumb]->setPiece();
    }
    else
    {
        pieceIdx = bUpM->backUpEndPiece->getIndex();
        gridIdx = bUpM->backUpEndPiece->getTileNumber();
        whiteTurn = bUpM->backUpEndPiece->isWhite();
        delete (*grid)[gridIdx]->getPiece();
        (*grid)[gridIdx]->setPiece(bUpM->backUpEndPiece);

        // restore the piece in piece arrays
        if (whiteTurn)
            (*whitePieces)[pieceIdx] = bUpM->backUpEndPiece;
        else
            (*blackPieces)[pieceIdx] = bUpM->backUpEndPiece;
    }

    if (bUpM->backUpAdditionalPiece != nullptr)
    {
        pieceIdx = bUpM->backUpAdditionalPiece->getIndex();
        gridIdx = bUpM->backUpAdditionalPiece->getTileNumber();
        whiteTurn = bUpM->backUpAdditionalPiece->isWhite();

        // if the additional piece is a rook then a castle was performed
        // based on the rook index determine if it was a queen side
        // or king side castle. Reset d1/d8 or f1/f8.
        if (bUpM->backUpAdditionalPiece->getPieceSymbol() == rookID)
        {
            if (bUpM->backUpAdditionalPiece->getIndex() == 8)
            {
                (*grid)[gridIdx + 3]->setPiece();
            }
            else if (bUpM->backUpAdditionalPiece->getIndex() == 15)
            {
                (*grid)[gridIdx - 2]->setPiece();
            }
        }

        delete (*grid)[gridIdx]->getPiece();
        (*grid)[gridIdx]->setPiece(bUpM->backUpAdditionalPiece);
        if (whiteTurn)
            (*whitePieces)[pieceIdx] = bUpM->backUpAdditionalPiece;
        else
            (*blackPieces)[pieceIdx] = bUpM->backUpAdditionalPiece;
    }

    // rotate turn
    game->rotateTurn();

    //    // restore attack grid
    //    if (maximizing)
    //        memcpy(game->blackAttacks, backUPAttackGrid, 8 * 8 * sizeof(int));
    //    else
    //        memcpy(game->whiteAttacks, backUPAttackGrid, 8 * 8 * sizeof(int));
}

void MinMaxABP::simulateMove(Move m)
{
    uint startGridIdx, endGridIdx;

    startGridIdx = m.startTileNumb;
    endGridIdx = m.endTileNumb;

    BoardTile *startTile = (*grid)[startGridIdx];
    BoardTile *endTile = (*grid)[endGridIdx];

    // back up the start and end tiles for when the move is undone
    BackUpMove *bUpM = new BackUpMove();
    bUpM->backUpStartPiece = new Piece(*(startTile->getPiece()));
    if (endTile->getPiece() != nullptr)
        bUpM->backUpEndPiece = new Piece(*(endTile->getPiece()));

    // the king tile number before moving
    // used to determine which side to castle and
    // which rook to move.
    char movingPieceSymbol = startTile->getPieceSymbol();
    uint kingTNBefore;
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
            bUpM->backUpAdditionalPiece = new Piece(*((*grid)[endGridIdx - 2]->getPiece()));
            // perform rook movement
            (*grid)[endGridIdx - 2]->setMoved();
            (*grid)[endGridIdx + 1]->setPiece((*grid)[endGridIdx - 2]->getPiece());
            (*grid)[endGridIdx - 2]->removePiece();
        }
        // king side castle
        else if ((endTile->getTileNumber() - kingTNBefore) == 2)
        {
            // save the rook for when the move is undone
            bUpM->backUpAdditionalPiece = new Piece(*((*grid)[endGridIdx + 1]->getPiece()));
            // perform rook movement
            (*grid)[endGridIdx + 1]->setMoved();
            (*grid)[endGridIdx - 1]->setPiece((*grid)[endGridIdx + 1]->getPiece());
            (*grid)[endGridIdx + 1]->removePiece();
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
            uint tn = game->getEPTileNumber(!game->isWhiteTurn());
            // save the pawn
            bUpM->backUpAdditionalPiece = new Piece(*(*grid)[tn]->getPiece());
            // remove the captured pawn
            (*grid)[tn]->getPiece()->setCaptured();
            (*grid)[tn]->removePiece();
        }
    }

    // convert pawn to queen
    if (movingPieceSymbol == pawnID)
    {
        // because turn has switched over true means its black's turn
        if (game->isWhiteTurn())
        {
            // if a black pawn has reached the opposite end conver to queen
            if ((endGridIdx / 8) == 7)
            {
                // At the moment just select the queen
                uint pIndex = endTile->getPiece()->getIndex();
                delete endTile->getPiece();
                // TODO: Some heuristic of selectin most powerful pawn conversion
                endTile->setPiece(new Piece(false, queenID, endTile->getTileNumber(), (endGridIdx / 8), (endGridIdx % 8), pIndex, queenIDW));
            }
        }
        else
        {
            // if a white pawn has reached the opposite end conver to queen
            if ((endGridIdx / 8) == 0)
            {
                // At the moment just select the queen
                uint pIndex = endTile->getPiece()->getIndex();
                delete endTile->getPiece();
                // TODO: Some heuristic of selectin most powerful pawn conversion
                endTile->setPiece(new Piece(true, queenID, endTile->getTileNumber(), (endGridIdx / 8), (endGridIdx % 8), pIndex, queenIDB));
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
