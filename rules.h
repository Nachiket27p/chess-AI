#ifndef RULES_H
#define RULES_H

#include <vector>
#include "boardtile.h"
#include "debugwindow.h"
#include "piece.h"

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

    DebugWindow *dbw;
    std::vector<Piece *> blackKingDefenders;
    std::vector<Piece *> whiteKingDefenders;

public:
    // two boards white and black
    int whiteAttacks[8][8] = {};
    int blackAttacks[8][8] = {};

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
        for (int col = 0; col < 8; col++)
        {
            // the only places the pieces can attack at the beginning
            // is the 1 row beyond the pawns
            whiteAttacks[5][col] = 1;
            blackAttacks[2][col] = 1;
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

    void resetKingDefenders();

    //ssssssssssssssssssssssssssssssssssssss
    void setDebugWindowAccess(DebugWindow *dbw);

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
     * @brief Helper function to ensure the rook, bishop, and queen are only
     *          allowed to move to valid locations.
     * 
     * @param row The row on the grid.
     * @param col The column on the grid.
     * @param dr The increment in the row dimension.
     * @param dc The increment in the col dimension.
     * @param ok Only gets set to 'true' if a valid move is added to the list of valid moves.
     * @param tile The tile which is being checked.
     */
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
     * @brief This is a helper function which is ued to add a valid move to the 
     * list of valid moves. It also take into consideration if there is
     * is a check and only allows valid moves.
     * 
     * @param tileNumber The unique id used to when ther is a check, and used as the
     * unique id add to the valid move array.
     * @param ok Only gets set to 'true' if a valid move is added to the list of valid moves.
     */
    inline void addValidMove(int tileNumber, bool *ok);

    /**
     * @brief This is a special helper function used for the king, as it has special
     * requirements for valid moves when ther is a check.
     * 
     * @param row The row coordiante being checked.
     * @param col The column coordinate being checked.
     * @param tileNumber The unique tile id which gets added to the valid list of moves.
     * @param ok Only gets set to 'true' if a valid move is added to the list of valid moves.
     */
    inline void addValidMoveKing(int row, int col, int tileNumber, bool *ok);

    /**
     * @brief This helper function is used to update the corresponding attack board
     * when a move for that color/side is made.
     * 
     * @param pieces The list of pieces whose attack tiles need to be computed.
     * @param attackBoard The 8x8 grid on which a true value is set if a piece can attack that spot.
     * @param pieceColor The color to indicate which sides board to update.
     */
    inline void updateAttackBoardHelper(Piece *pieces[16], int attackBoard[8][8], int pieceColor);

    /**
     * @brief Helper function used to computer the attack tile for rook, bishop, and queen.
     * 
     * @param row The start row index being checked.
     * @param col The start column index being checked.
     * @param dr Can be +1/0/-1 to determine if the row value increased or decreased.
     * @param dc Can be +1/0/-1 to determine if the column value increased or decreased.
     * @param pieceColor If the piece is black or white.
     * @param attackGrid The grid which is being updated.
     */
    inline void attackeRBQHelper(int row, int col, int dr, int dc, bool pieceColor, int attackGrid[8][8]);

    /**
     * @brief  Used to find the check source if a check is detected using the arrack grid.
     * Calls other helper function to aid in this task.
     * 
     */
    inline void locateCheckSource();

    /**
     * @brief Helper function used to scan for a check when one is detected.
     * 
     * @param row The start row index of the king being checked.
     * @param col The start column index of the king being checked.
     * @param dr Can be +1/0/-1 to determine if the row value increased or decreased.
     * @param dc Can be +1/0/-1 to determine if the column value increased or decreased.
     * @param checkPiece 
     * @return true If a check was located.
     * @return false If a check was not located.
     */
    inline bool scanCheckHelper(int row, int col, int dr, int dc, char *checkPiece);

    bool checkKnight(int row, int col);

    inline bool checkTile(int row, int col, char pieceType);

    /**
     * @brief setCheck Private helper function to set check.
     */
    inline void setCheck();

    inline bool isKingDefender(Piece* p);

    inline bool canKingDefenderMove(int dr, int dc, BoardTile *tile, BoardTile *king, int attackBoardVal);

    bool canCastle(BoardTile *tile);
};

#endif // RULES_H
