#ifndef PIECE_H
#define PIECE_H

#include <QPixmap>

class Piece
{
private:
    uint moved;
    bool white;
    char pieceSymbol;
    uint tileNumber;
    uint row;
    uint col;
    bool captured;
    uint basePowerValue;
    uint index;
    QPixmap *icon;

public:
    /**
     * @brief Construct a new Piece object
     * 
     * @param _white Set to true if white piece, false otherwise.
     * @param _pieceSymbol Use piece characters defined in utils.h.
     * @param _index The index of this piece in the piece arrays.
     * @param _row The row location on the grid.
     * @param _col The column location on the grid.
     * @param iconPath The filepath to the piece symbol.
     */
    Piece(bool _white, char _pieceSymbol, uint _tileNumber, uint _row, uint _col, uint _index, QString iconPath);

    /**
     * @brief Copy constructor for Piece.
     * @param p The Piece which is being copied into this one.
     */
    Piece(const Piece &p);

    /**
     * @brief Return the number of moves this piece has made in total.
     * 
     * @return uint Number of moves.
     */
    uint hasMoved();

    /**
     * @brief Increment the move counter.
     * 
     */
    void setMoved();

    /**
     * @brief Returns true if this piece has been captured.
     * 
     * @return true If the piece is no longer on the board.
     * @return false If the piece is on the board.
     */
    bool isCaptured();

    /**
     * @brief Set the captured variable to indicate the piece is no
     * longer on the board.
     */
    void setCaptured();

    /**
     * @brief Get row on which this piece is located on the grid.
     * 
     * @return uint Row location.
     */
    uint getRow();

    /**
     * @brief Get the column on which this piece is located on the grid.
     * 
     * @return uint Column location.
     */
    uint getCol();

    /**
     * @brief If the piece is white or black.
     * 
     * @return true If the piece is white.
     * @return false If the piece is black.
     */
    bool isWhite();

    /**
     * @brief Get the piece symbol.
     * 
     * @return char Unique piece symbol.
     */
    char getPieceSymbol();

    /**
     * @brief Get the QPixmap object for this piece.
     * 
     * @return QPixmap* Pointer to image object.
     */
    QPixmap *getIcon();

    /**
     * @brief Update the position of this piece on the board.
     * 
     * @param _row Row position on board.
     * @param _col Column position on board.
     */
    void updatePosition(uint _row, uint _col, uint _tileNumber);

    /**
     * @brief Get the index of this piece in the array of pieces.
     * 
     * @return uint Index in piece array.
     */
    uint getTileNumber();

    /**
     * @brief Reset this piece to the row and column specified,
     * if no row and column is specified the piece stays in the
     * same place but the move counter is set to 0 and captured flag
     * is also set to false.
     * 
     * @param _row Row to which this piece will be moved.
     * @param _col Column to which this piece will be moved.
     */
    void resetPiece(uint _row, uint _col);

    /**
     * @brief Returns the base power for this piece.
     * 
     * @return uint The base power value for this piece.
     */
    uint getBasePowerValue();

    /**
     * @brief Get the index of this piece in the pieces array.
     * 
     * @return uint The index at which position this piece is located.
     */
    uint getIndex();
};

#endif // PIECE_H
