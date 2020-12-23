#ifndef RULES_H
#define RULES_H

#include "boardtile.h"

extern BoardTile *grid[8][8];
extern int validMoves[64];
extern int vmIdx;

/**
 * @brief The Rules class is a singleton class which is responsible for
 * checking if moves made are valid. It one public function 'canMove'
 * which is used to chec if the move is valid.
 */
class Rules
{
private:
    // Used to keep single instance of this class
    static Rules *instance;

    // Used internally to determine if a requested tile can move.
    bool okToMove;

    // used to keep track of if there is a check.
    bool isCheck;
    int checkRow;
    int checkCol;

    // pointer always keep track of where the king piece is
    BoardTile *blackKing;
    BoardTile *whiteKing;

    // two boards white and black
    bool whiteAttacks[8][8] = {};
    bool blackAttacks[8][8] = {};

private:
    /**
     * @brief Private constructor to keep only 1 instane of this calls.
     */
    Rules()
    {
        okToMove = false;
        isCheck = false;
        checkRow = -1;
        checkCol = -1;

        // set the initial postions of the kings
        blackKing = grid[0][4];
        whiteKing = grid[7][4];

        // initialize the attack board
        for(int col = 0; col < 8; col++)
        {
            // the only places the pieces can attack at the beginning
            // is the 1 row beyond the pawns
            whiteAttacks[2][col] = 1;
            blackAttacks[6][col] = 1;
        }
    };

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

    /**
     * @brief Used to update the position of the king if is is being moved.
     * @param isWhite If the piece is white or black.
     * @param newTile If The pointer to the new tile on which the king is located.
     */
    void setKingPos(bool isWhite, BoardTile *newTile);

    void scanForCheck();

    void updateAttackBoard();

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

    inline void enforceRBQHelper(int row, int col, int dr, int dc, bool *ok, BoardTile *tile);

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

    /**
     * @brief setCheck Private helper function to set check.
     */
    inline void setCheck();

    inline void addValidMove(int tileNumber, bool *ok);

    inline void addValidMoveKing(int row, int col, int tileNumber, bool *ok);

    inline void updateAttackBoardHelper(Piece *pieces[16], bool attackBoard[8][8], bool pieceColor);

    inline void attackeRBQHelper(int row, int col, int dr, int dc, bool pieceColor, bool attackGrid[8][8]);

    inline void locateCheckSource();

    inline bool scanCheckHelper(int row, int col, int dr, int dc, char *checkPiece);

    inline bool checkKnight(int row, int col);

    inline bool checkTile(int row, int col, char pieceType);

};

#endif // RULES_H
