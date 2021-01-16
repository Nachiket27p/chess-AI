#ifndef BOARDTILE_H
#define BOARDTILE_H

#include <QLabel>
#include "theme.h"
#include "piece.h"

class BoardTile : public QLabel
{
private:
    // Coordinated on the board.
    int row, col;
    // Unique grid spot number.
    int tileNumber;
    // Identifies if this tile is a dark tile.
    bool isDarkTile;
    // Pointer to the piece currently on this tile.
    Piece *piece;

private:
    //    /**
    //     * @brief Used to check if the piece selected can move and if so which
    //     * direction it can move in, calls the canMove function from the Rules class.
    //     */
    //    void enforceRules(bool playerMove = true);

public:
    /**
     * @brief Construct a new Board Tile object. This class inherites the QLabel class.
     * 
     * @param _row The row on which this tile is located on the board.
     * @param _col The column on whih this tile is located on the board.
     * @param _tileNumber The uniqie tile number for this tile.
     * @param _isDarkTile Set 'true' if the tile is black, 'false' if tile is light.
     * @param _parent The QWidget object on which this tile will be rendered.
     */
    BoardTile(int _row, int _col, int _tileNumber, bool _isDarkTile, QWidget *_parent)
        : QLabel(_parent), row(_row), col(_col), tileNumber(_tileNumber), isDarkTile(_isDarkTile)
    {
        piece = nullptr;
    }

    /**
     * @brief Used to check if the piece selected can move and if so which
     * direction it can move in, calls the canMove function from the Rules class.
     */
    void enforceRules(bool playerMove = true);

    /**
     * @brief Used to check if the game has ended via a check/stale mate. This function
     * calls 'hasGameEnded' function in the Rules class to help determine if game has ended.
     * If the game has ended a dialog is displayed with a message informing the player
     * how the game has ended.
     */
    void checkGameEnd();

    /**
     * @brief Copy constructor.
     * @param b2 BoardTile which is being copied into this one.
     */
    BoardTile(const BoardTile &b);

    /**
     * @brief Set the Piece object.
     * 
     * @param piece The piece object to be placed on this tile.
     */
    void setPiece(Piece *_piece = nullptr);

    /**
     * @brief Populates the tile with the the piece identified by 'symbol' parameter passed in.
     * 
     */
    void displayTile();

    /**
     * @brief Used to set the color of the tile on the board, after pieces are moved or
     * removed from the board.
     * 
     */
    void displayTileColor();

    /**
     * @brief Removes a piece from this tile by clearing the style of the tile.
     * 
     */
    void removePiece();

    /**
     * @brief Returns the character identifying the piece located on this tile. If
     * there is no piece then return null terminator.
     * 
     * @return char The unique character code.
     */
    char getPieceSymbol();

    /**
     * @brief Returns the row on which this tile is located (Zero based index).
     * 
     * @return int Row on the board (Zero based index)
     */
    int getRow() { return row; }

    /**
     * @brief Returns the column on which this tile is located (Zero based index).
     * 
     * @return int Column on the board (Zero based index)
     */
    int getCol() { return col; }

    /**
     * @brief Return the tile number on of this tile.
     * 
     * @return int Tile number.
     */
    int getTileNumber() { return tileNumber; }

    /**
     * @brief Determines if the piece on the board is white or black. Note false is
     * returned even if it is unoccupied.
     * 
     * @return true If piece on this tile is white.
     * @return false If this piee is black or unoccupied.
     */
    bool getPieceColor();

    /**
     * @brief Get the Piece object on this tile.
     * 
     * @return Piece* The piece on this tile.
     */
    Piece *getPiece() { return piece; }

    /**
     * @brief Determines if the tile is occupied by a piece.
     * 
     * @return true If there is a piece on this tile.
     * @return false If the ther is no piece on this tile.
     */
    bool isOccupied();

    /**
     * @brief Registers mouse events on this tile, increlemts the static variable 
     * 'selected' and calls the enforce rules function.
     * 
     */
    void mousePressEvent(QMouseEvent *);

    /**
     * @brief Returns the number of times the piece on this tile has moved,
     * if no piece is on the tile then 0 is returned.
     * 
     * @return int Number of moved the piece on this tile has made in total.
     */
    int hasMoved();

    /**
     * @brief Set the piece on this tile to moved by calling the setMoved function
     * within the Piece class.
     */
    void setMoved();

    /**
     * @brief Used to call the AI min-max alpha-beta pruning algorithm.
     * 
     */
    static void aiMove();
};

#endif // BOARDTILE_H
