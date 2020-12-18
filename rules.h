#ifndef RULES_H
#define RULES_H

#include "boardtile.h"

extern BoardTile *board[8][8];
extern int validMoves[64];
extern int listIndex;

/**
 * @brief The Rules class is a singleton class which is responsible for
 * checking if moves made are valid. It one public function 'canMove'
 * which is used to chec if the move is valid.
 */
class Rules
{
private:
    // Used internally to determine if a requested tile can move.
    bool okToMove;

    // Used to keep single instance of this class
    static Rules *instance;

private:
    /**
     * @brief Private constructor to keep only 1 instane of this calls.
     */
    Rules();

public:
    /**
     * @brief Returns the instance of the class. If an instance does not exist
     * construts a new one and returns it.
     * @return Instane of Rules.
     */
    static Rules *getInstance();

    /**
     * @brief Used to determine if the specific tile/piece can move.
     * 
     * @param tile The tile which contains the piece which can move.
     * @return true If the piece on tile passed in can move.
     * @return false If the piece on the tile passed in cannot move.
     */
    bool canMove(BoardTile *tile);

private:
    /**
     * @brief Private function which determines if the pawn can move.
     * 
     * @param tile The tile object representing the pawn.
     * @return true If the pawn can move.
     * @return false If the pawn cannot move.
     */
    bool enforcePawn(BoardTile *tile);

    /**
     * @brief Private function which determines if the rook can move.
     * 
     * @param tile The tile object representing the rook.
     * @return true If the rook can move.
     * @return false If the rook cannot move.
     */
    bool enforceRook(BoardTile *tile);

    /**
     * @brief Private function which determines if the knight can move.
     * 
     * @param tile The tile object representing the knight.
     * @return true If the knight can move.
     * @return false If the knight cannot move.
     */
    bool enforceKnight(BoardTile *tile);

    /**
     * @brief Private function which determines if the bishop can move.
     * 
     * @param tile The tile object representing the bishop.
     * @return true If the bishop can move.
     * @return false If the bishop cannot move.
     */
    bool enforceBishop(BoardTile *tile);

    /**
     * @brief Private function which determines if the queen can move.
     * 
     * @param tile The tile object representing the queen.
     * @return true If the queen can move.
     * @return false If the queen cannot move.
     */
    bool enforceQueen(BoardTile *tile);

    /**
     * @brief Private function which determines if the king can move.
     * 
     * @param tile The tile object representing the king.
     * @return true If the king can move.
     * @return false If the king cannot move.
     */
    bool enforceKing(BoardTile *tile);

    /**
     * @brief Highlights the tiles to which the piece can move to.
     * 
     */
    void highlightTiles();
};

#endif // RULES_H
