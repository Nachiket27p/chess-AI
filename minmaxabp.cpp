#include "minmaxabp.h"

MinMaxABP::MinMaxABP(BoardTile *(*_grid)[8][8], Piece *(*_whitePieces)[16], Piece *(*_blackPieces)[16], bool _color)
{
    game = game->getInstance();
    grid = _grid;
    whitePieces = _whitePieces;
    blackPieces = _blackPieces;
    color = _color;
    startTile = nullptr;
    endTile = nullptr;
}

int MinMaxABP::minMax(int depth, bool maximizing, bool maxingColor, Move *bestMove)
{
    if (depth == 0 || game->hasGameEnded(maxingColor))
    {
        return evaluate(maxingColor);
    }

    std::vector<Move> moves;

    if (maximizing)
    {
        int max_eval = INT_MIN;
    }
}

int MinMaxABP::evaluate(bool maxingColor)
{
    if (maxingColor)
        return (whiteScore - blackScore);
    else
        return (blackScore - whiteScore);
}

void MinMaxABP::makeMove(Move m)
{
}

void MinMaxABP::unmakeMove(Move m)
{
}

void MinMaxABP::simulateMove(Move m)
{
//    // if the same tile is clicked twice do unhighlight the possible moves
//    if (tileNumber == game->selectedTile->getTileNumber())
//    {
//        game->selectedTile->displayTileColor();
//        game->unhighlightTiles();
//        game->resetVmIdx();
//        game->selected = 0;
//        return;
//    }

//    // the king tile number before moving
//    // used to determine which side to castle and
//    // which rook to move.
//    int kingTNBefore;
//    // if the piece being moved is the the king the update the king pointer
//    if (game->selectedTile->getPieceSymbol() == kingID)
//    {
//        kingTNBefore = game->selectedTile->getTileNumber();
//    }

//    // check if en passant move is being performed
//    bool ep = false;
//    if ((game->selectedTile->getPieceSymbol() == pawnID) &&
//        (this->col != game->selectedTile->getCol()) && !this->isOccupied())
//    {
//        ep = true;
//    }
//    if (game->isValidMove(tileNumber))
//    {
//        // if the tile to which this piece is being moved is
//        // occupied by the opponents piece then set the piece as
//        // captured
//        if (isOccupied())
//            this->piece->setCaptured();

//        // replace the piece on this tile
//        setPiece(game->selectedTile->getPiece());
//        // remove the piece from the selected tile & display the empty tile
//        game->selectedTile->removePiece();
//        game->selectedTile->displayTile();
//        // mark the piece as being moved
//        this->setMoved();
//        // display the piece on the new position and unhighlight other tiles
//        this->displayTile();
//        // unhilight the unselected tiles
//        game->unhighlightTiles();
//        // reset the valid moves index to 0 so the validMoves array does not
//        // need to be cleared
//        game->resetVmIdx();
//        // reset the board to have no tiles selected
//        game->selected = 0;
//        // change the turn variable to indicate it is the opponents turn now
//        game->rotateTurn();
//    }
//    else
//    {
//        // If the move was not valid/successful then set the selected value back to 1
//        // to indicate that game state has returned to a position where the player has
//        // selected a piece.
//        game->selected = 1;
//        return;
//    }

//    // Casteling
//    if (piece->getPieceSymbol() == kingID)
//    {
//        // Queen side castle
//        if ((kingTNBefore - tileNumber) == 2)
//        {
//            grid[row][col - 2]->setMoved();
//            grid[row][col + 1]->setPiece(grid[row][col - 2]->getPiece());
//            grid[row][col - 2]->removePiece();
//            grid[row][col - 2]->displayTile();
//            grid[row][col + 1]->displayTile();
//        }
//        // king side castle
//        else if ((kingTNBefore - tileNumber) == -2)
//        {
//            grid[row][col + 1]->setMoved();
//            grid[row][col - 1]->setPiece(grid[row][col + 1]->getPiece());
//            grid[row][col + 1]->removePiece();
//            grid[row][col + 1]->displayTile();
//            grid[row][col - 1]->displayTile();
//        }
//    }

//    // en passant
//    if (piece->getPieceSymbol() == pawnID)
//    {
//        game->canEnPassant(this);
//        // if ep was set to true above then perform enpassant by
//        // removing the opposings pawn.
//        if (ep)
//        {
//            int tn = game->getEPTileNumber(!game->isWhiteTurn());
//            int r = tn / 8;
//            int c = tn % 8;
//            grid[r][c]->getPiece()->setCaptured();
//            grid[r][c]->removePiece();
//            grid[r][c]->displayTile();
//        }
//    }

//    // convert pawn to queen
//    if (this->piece->getPieceSymbol() == pawnID)
//    {
//        // because turn has switched over true means its black's turn
//        if (game->isWhiteTurn())
//        {
//            // if a black pawn has reached the opposite end conver to queen
//            if (row == 7)
//            {
//                int index = this->piece->getIndex();
//                delete piece;
//                QString pp = "";
//                PAWN_PROMOTION_DIALOG(this->parentWidget(), pp);
//                piece = new Piece(false, queenID, index, row, col, blackPath + pp);
//                this->displayTile();
//            }
//        }
//        else
//        {
//            // if a white pawn has reached the opposite end conver to queen
//            if (row == 0)
//            {
//                int index = this->piece->getIndex();
//                delete piece;
//                QString pp = "";
//                PAWN_PROMOTION_DIALOG(this->parentWidget(), pp);
//                piece = new Piece(true, queenID, index, row, col, whitePath + pp);
//                this->displayTile();
//            }
//        }
//    }

//    // reset en passant
//    game->resetEnPassant(!game->isWhiteTurn());
//    // update the attack board
//    game->updateAttackBoard();
//    // scan for check after a piece has moved
//    game->scanForCheck();
//    // check if the game is over
//    this->checkGameEnd();
}
