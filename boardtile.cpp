#include <QMenu>
#include "utils.h"
#include "boardtile.h"
#include "rules.h"
#include "board.h"

// Get an instance of the rules class.
extern Rules *game;
// Access the extern variables which determine the run and the current theme.
extern Theme *currentTheme;

void BoardTile::setPiece(Piece *piece)
{
    this->piece = piece;
    piece->updatePosition(row, col);
}

void BoardTile::displayTile()
{
    // if there is a piece on this tile display the icon
    // otherwise clear it.
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
    // based on if it is the dark or light tile set the correct background color
    if (isDarkTile)
    {
        setStyleSheet(QString("QLabel {background-color: ") +
                      currentTheme->getDarkBackground() +
                      QString("} :hover {background-color: ") +
                      currentTheme->getHoverBackground() +
                      QString(" color: ") +
                      currentTheme->getHoverColorWhite() +
                      QString("}"));
    }
    else
    {
        setStyleSheet(QString("QLabel {background-color: ") +
                      currentTheme->getLightBackground() +
                      QString("} :hover {background-color: ") +
                      currentTheme->getHoverBackground() +
                      QString(" color: ") +
                      currentTheme->getHoverColorWhite() +
                      QString("}"));
    }
}

void BoardTile::removePiece()
{
    piece = nullptr;
}

char BoardTile::getPieceSymbol()
{
    // if this tile has a piece on it then return the piece
    // symbol otherwise return null terminator.
    if (piece != nullptr)
    {
        return piece->getPieceSymbol();
    }
    return '\0';
}

bool BoardTile::getPieceColor()
{
    // if there is a piece on this tile then return the color
    // true = white, false = white. Not if there is no piece it still
    // returns false.
    if (piece != nullptr)
    {
        return piece->isWhite();
    }
    return false;
}

bool BoardTile::isOccupied()
{
    // if there is a piece on this tile then return true otherwise,
    // return false.
    if (piece != nullptr)
    {
        return true;
    }
    return false;
}

void BoardTile::mousePressEvent(QMouseEvent *)
{
    // increment the selected counter to determine if a tile has been selected.
    ++game->selected;
    // call the function to determine the valid moves and if the
    // selected move is valid.
    enforceRules();
}

int BoardTile::hasMoved()
{
    // if there is a piece on this tile call the hasMoved function
    // on the piece object and return the number of moves this piece
    // has made in toal through this game.
    if (piece != nullptr)
        return piece->hasMoved();

    return 0;
}

void BoardTile::setMoved()
{
    // call the setMoved function to increment the move counter for
    // this piece.
    if (piece != nullptr)
        piece->setMoved();
}

void BoardTile::checkGameEnd()
{
    // call the hasGameEnded function from the Rules class to determine
    // if there is a check/stale mate.
    int endVal = game->hasGameEnded(game->isWhiteTurn());
    int rtnVal = 0;
    // if the game is over display a dialog.
    if (endVal == CHECK_MATE)
    {
        CONFIRM_DIALOG(this->parentWidget(), rtnVal, "Check Mate.");
    }
    else if (endVal == STALE_MATE)
    {
        CONFIRM_DIALOG(this->parentWidget(), rtnVal, "Stale Mate.");
    }
    // ask user if they want to reset the game
    if (rtnVal)
    {
        Board *b = (Board *)this->parentWidget();
        // reset game
        b->on_actionNew_Game_triggered();
    }
}

BoardTile::BoardTile(const BoardTile &b)
{
    this->row = b.row;
    this->col = b.col;
    this->tileNumber = b.tileNumber;
    this->isDarkTile = b.isDarkTile;
    this->piece = b.piece;
}

void BoardTile::enforceRules()
{
    // if a piece is selected then determine the valid moves for the selected
    // piece.
    if (game->selected == 1)
    {
        // there must be a piece on the tile selected and
        // the piece selected must also be the correct color
        if ((piece != nullptr) && (piece->isWhite() == game->isWhiteTurn()))
        {
            // call the canMove function from the Rules class to determine the
            // valid position on the board where this piece can move.
            if (game->canMove(this))
            {
                // set the style sheet
                game->selectedTile = this;
                if (piece->isWhite())
                {
                    game->selectedTile->setStyleSheet(QString("QLabel {background-color:") +
                                                currentTheme->getHoverBackground() +
                                                "color: " +
                                                currentTheme->getHoverColorWhite() +
                                                QString("}"));
                }
                else
                {
                    game->selectedTile->setStyleSheet(QString("QLabel {background-color:") +
                                                currentTheme->getHoverBackground() +
                                                "color: " +
                                                currentTheme->getHoverColorBlack() +
                                                QString("}"));
                }
                // if the piece can move then highlight the correct tiles.
                game->highlightTiles();
            }
            else
            {
                // the piece cannot move then reset the selected flag.
                game->selected = 0;
            }
        }
        else
        {
            // if an invalid piece is initially selected reset the flag.
            game->selected = 0;
        }
    }
    else
    {
        // if the same tile is clicked twice do unhighlight the possible moves
        if (tileNumber == game->selectedTile->getTileNumber())
        {
            game->selectedTile->displayTileColor();
            game->unhighlightTiles();
            game->resetVmIdx();
            game->selected = 0;
            return;
        }

        // the king tile number before moving
        // used to determine which side to castle and
        // which rook to move.
        int kingTNBefore;
        // if the piece being moved is the the king the update the king pointer
        if (game->selectedTile->getPieceSymbol() == kingID)
        {
            kingTNBefore = game->selectedTile->getTileNumber();
        }

        // check if en passant move is being performed
        bool ep = false;
        if ((game->selectedTile->getPieceSymbol() == pawnID) &&
            (this->col != game->selectedTile->getCol()) && !this->isOccupied())
        {
            ep = true;
        }
        if (game->isValidMove(tileNumber))
        {
            // if the tile to which this piece is being moved is
            // occupied by the opponents piece then set the piece as
            // captured
            if (isOccupied())
                this->piece->setCaptured();

            // replace the piece on this tile
            setPiece(game->selectedTile->getPiece());
            // remove the piece from the selected tile & display the empty tile
            game->selectedTile->removePiece();
            game->selectedTile->displayTile();
            // mark the piece as being moved
            this->setMoved();
            // display the piece on the new position and unhighlight other tiles
            this->displayTile();
            // unhilight the unselected tiles
            game->unhighlightTiles();
            // reset the valid moves index to 0 so the validMoves array does not
            // need to be cleared
            game->resetVmIdx();
            // reset the board to have no tiles selected
            game->selected = 0;
            // change the turn variable to indicate it is the opponents turn now
            game->rotateTurn();
        }
        else
        {
            // If the move was not valid/successful then set the selected value back to 1
            // to indicate that game state has returned to a position where the player has
            // selected a piece.
            game->selected = 1;
            return;
        }

        // Casteling
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

        // en passant
        if (piece->getPieceSymbol() == pawnID)
        {
            game->canEnPassant(this);
            // if ep was set to true above then perform enpassant by
            // removing the opposings pawn.
            if (ep)
            {
                int tn = game->getEPTileNumber(!game->isWhiteTurn());
                int r = tn / 8;
                int c = tn % 8;
                grid[r][c]->getPiece()->setCaptured();
                grid[r][c]->removePiece();
                grid[r][c]->displayTile();
            }
        }

        // convert pawn to queen
        if (this->piece->getPieceSymbol() == pawnID)
        {
            // because turn has switched over true means its black's turn
            if (game->isWhiteTurn())
            {
                // if a black pawn has reached the opposite end conver to queen
                if (row == 7)
                {
                    int index = this->piece->getIndex();
                    delete piece;
                    QString pp = "";
                    PAWN_PROMOTION_DIALOG(this->parentWidget(), pp);
                    piece = new Piece(false, queenID, index, row, col, blackPath + pp);
                    this->displayTile();
                }
            }
            else
            {
                // if a white pawn has reached the opposite end conver to queen
                if (row == 0)
                {
                    int index = this->piece->getIndex();
                    delete piece;
                    QString pp = "";
                    PAWN_PROMOTION_DIALOG(this->parentWidget(), pp);
                    piece = new Piece(true, queenID, index, row, col, whitePath + pp);
                    this->displayTile();
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
        this->checkGameEnd();
    }
}
