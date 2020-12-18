#ifndef BOARDTILE_H
#define BOARDTILE_H

#include <QLabel>
#include "theme.h"

class BoardTile : public QLabel
{
private:
    // Coordinated on the board
    int row, col;
    // Unique grid spot number
    int tileNumber;
    // Identifies if this tile is a dark tile
    bool isDarkTile;
    // Identified if this tile is occupied by a piece
    bool occupied;
    // Identifies if this tile is occupied by a white piece
    bool isWhite;
    // Identified the specific piece on this tile
    char pieceSymbol;

    // static counter used to determine when a player tries to move a piece
    static int selected;

    // static BoardTile object which keeps track of the piece piece seected
    static BoardTile *selectedTile;

private:
    /**
     * @brief Used to check if the piece selected can move and if so which
     * direction it can move in, calls the canMove function from the Rules class.
     */
    void enforceRules();

    /**
     * @brief Removes the highlights from the tiles once a different tile has
     * been selcted. This is a helper to the 'enforceRules' function.
     */
    void unhighlightTiles();

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
        : QLabel(_parent), row(_row), col(_col), tileNumber(_tileNumber), isDarkTile(_isDarkTile),
          occupied(false)
    {
        // Nothing to do
    }

    /**
     * @brief Set the piece onto this tile, the 'symbol' param identified the tyle 
     * piece being placed on this tile. The piece codes are located in 'util.h'.
     * 
     * @param symbol Character code representing the peice code.
     * @param isWhite Flag to represent if the piece being placed on here is a white.
     *                  'true' if white, 'false' if black.
     */
    void setPiece(const char symbol, bool isWhite);

    /**
     * @brief Populates the tile with the the piece identified by 'symbol' parameter passed in.
     * 
     * @param symbol Character code representing the peice code.
     * @param isWhite Flag to represent if the piece being placed on here is a white.
     *                  'true' if white, 'false' if black.
     */
    void populateTile(const char symbol, bool isWhite);

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
     * @brief Returns the character identifying the piece located on this tile.
     * 
     * @return char The unique character code.
     */
    char getPieceSymbol() { return pieceSymbol; }

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
     * @brief Determines if the piece on the board is white or black.
     * 
     * @return true If piece on this tile is white.
     * @return false If this piee is black or unoccupied.
     */
    bool getPieceColor() { return isWhite; }

    /**
     * @brief Determines if the tile is occupied by a piece.
     * 
     * @return true If there is a piece on this tile.
     * @return false If the ther is no piece on this tile.
     */
    bool isOccupied() { return occupied; }

    /**
     * @brief Registers mouse events on this tile, increlemts the static variable 
     * 'selected' and calls the enforce rules function.
     * 
     */
    void mousePressEvent(QMouseEvent *);
};

#endif // BOARDTILE_H
