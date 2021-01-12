#ifndef RULES_H
#define RULES_H

#include <vector>
#include "boardtile.h"
#include "debugwindow.h"
#include "piece.h"
#include "utils.h"

#define CHECK_MATE 100
#define STALE_MATE 101

extern BoardTile *grid[8][8];

typedef struct CheckPos
{
    int row;
    int col;
} checkPos;

/**
 * @brief The Rules class is a singleton class which is responsible for
 * checking if moves made are valid. It one public function 'canMove'
 * which is used to chec if the move is valid.
 */
class Rules
{
private:
    // used to keep track of whise turn it is
    // ture means white turn and false means black turn
    bool turn;

    // Used to keep single instance of this class
    static Rules *instance;

    // vairables used to keep track of the valid moves
    int validMoves[64];
    int vmIdx = 0;

    // Used internally to determine if a requested tile can move.
    bool okToMove;

    // used to keep track of if there is a check.
    bool isCheck;
    std::vector<checkPos> checkPieces;

    // used to keep track the tile number of the pawn
    // which is vulnerable to the en passant attack
    // If value is zero then there is no en passant, even
    // theough there is a tilenumber 0, it is impossible
    // for there to be an en passant with tile number 0.
    int whiteEPL;
    int whiteEPR;
    int blackEPL;
    int blackEPR;

    DebugWindow *dbw;
    std::vector<Piece *> blackKingDefenders;
    std::vector<Piece *> whiteKingDefenders;

public:
    // two boards white and black
    int whiteAttacks[8][8] = {};
    int blackAttacks[8][8] = {};

    // used to keep track which tile is selected
    int selected;
    BoardTile *selectedTile;

private:
    /**
     * @brief Private constructor to keep only 1 instane of this calls.
     */
    Rules()
    {
        turn = true;
        okToMove = false;
        isCheck = false;

        // initialize the attack board
        for (int col = 0; col < 8; col++)
        {
            // the only places the pieces can attack at the beginning
            // is the 1 row beyond the pawns
            whiteAttacks[5][col] = 1;
            blackAttacks[2][col] = 1;
        }

        // initialize the flags for en passant
        whiteEPL = 0;
        whiteEPR = 0;
        blackEPL = 0;
        blackEPR = 0;

        // initialize the selection variables
        selected = 0;
        selectedTile = nullptr;
    };

public:

    bool isWhiteTurn();

    void setTurn(bool _turn);

    void rotateTurn();

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
     * @brief This function is called at the end of every move to scan the
     * board and determine if there is a check.
     * 
     */
    void scanForCheck();

    /**
     * @brief This function is used to determine all possible
     * attack position of all sam color piece on the board.
     * Example:
     * If a pawn is blocking a check then the piece is obviously not
     * allowed to move to cause a check.
     * 
     */
    void updateAttackBoard();

    /**
     * @brief This is called to reset the list of pieces which are
     * currently blocking the king from a check. This function also
     * populates the list of pieces which are blocking a check.
     * 
     */
    void resetKingDefenders();

    /**
     * @brief This function is used to check if en passant move can be
     * performed. It records the tilenumber of the pawn which can be
     * captured using the en passant.
     * 
     * @param tile The pawn tile which is being checked to see
     *              if it can be captured with en passant.
     */
    void canEnPassant(BoardTile *tile);

    /**
     * @brief Used to reset the en passant flags because, en passant
     * can only be utilized immediately afte the opponent triggers
     * the opportunity to en passant.
     * 
     * @param white True if it is white turn, false if it is black turn.
     */
    void resetEnPassant(bool whiteTurn);

    /**
     * @brief Returns the tile number of the piece which can be captured by
     * en passant. If no en passant is available then returns -1.
     * 
     * @param white True if it is white turn, false if it is black turn.
     * @return int Tile number of piece which can be captured if possible,
     *              otherwise -1.
     */
    int getEPTileNumber(bool whiteTurn);

    /**
     * @brief This function is used to determine if the game has
     * reached a check/stale mate situation. The return value
     * indicates if it is a check/stale mate or neither.
     * 
     * @param turn If it is white(true) or black(false) turn.
     * @return int 'CHECK_MATE=100' or 'STALE_MATE=101' or 0 if neither.
     */
    int hasGameEnded(bool whiteTurn);

    /**
     * @brief Highlights the tiles to which the piece can move to.
     */
    void highlightTiles();

    /**
     * @brief Unhilights any highlighted tiles.
     */
    void unhighlightTiles();

    bool isValidMove(int tileNumb);

    void resetVmIdx();

    void getMoves(std::vector<Move> &moves, int startTileNumb);

    // used to update the debugging window.
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

    /**
     * @brief Used to check if the knight is causing a check.
     * 
     * @param row The row position of the king.
     * @param col The column position of the king.
     * @return true If a check is caused.
     * @return false If no check is caused.
     */
    bool checkKnight(int row, int col);

    /**
     * @brief called by scanCheckHelper to determine if the piece
     * at the given row and column is capable of causing a check.
     * 
     * @param row The row position on the board.
     * @param col The column position on the board.
     * @param pieceType The type of piece being checked.
     * @return true If the piece at the position is causing a check.
     * @return false If the piece at the position if not causing a check.
     */
    inline bool checkKnightHelper(int row, int col, char pieceType);

    /**
     * @brief setCheck Private helper function to set check.
     */
    inline void setCheck();

    /**
     * @brief A private helper function which is used to check if
     * a particular piece is blocking a check.
     * 
     * @param p Piece being checked.
     * @return true If it is blocking a check.
     * @return false If it is not blocking a check.
     */
    inline bool isKingDefender(Piece *p);

    /**
     * @brief Used to determine if the piece is a blocking a check
     * which direction is is allowed to move in.
     * 
     * @param dr Can be +1/0/-1 to determine if the row value increased or decreased.
     * @param dc Can be +1/0/-1 to determine if the column value increased or decreased.
     * @param tile The tile being checked.
     * @param king The BoardTile on whcih the king is located.
     * @param attackBoardVal The attack value on the grid where the 'tile' is located.
     * @return true If the piece on the 'tile' can move.
     * @return false If the piece on the 'tile' cannot move.
     */
    inline bool canKingDefenderMove(int dr, int dc, BoardTile *tile, BoardTile *king, int attackBoardVal);

    /**
     * @brief Used to check if a castle can be performed. This function is called 
     * from the enforceKing function. If the king can castle then the castle
     * position is added to the list of valid moved.
     * 
     * @param tile The tile on which the king is located.
     * @return true If the king can castle and a move was added to validMoved
     * @return false If the king cannot castle.
     */
    bool canCastle(BoardTile *tile);

    /**
     * @brief Used toe check if en passant can be performed based on the
     * the private flags whiteEPL, whiteEPR, blackEPL, blackEPR. If en passant
     * move can be performed the move is added to the list of valid moves.
     * 
     * @param tileNumb The tile numer of the pawn which can capture with
     *                  enpassant.
     * @return true If the capture can be performed.
     * @return false If the capture cannot be performed.
     */
    bool enPassant(int tileNumb);
};

#endif // RULES_H
