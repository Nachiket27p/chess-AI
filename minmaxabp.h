#ifndef MINMAXABP_H
#define MINMAXABP_H

#include <stack>
#include <vector>
#include "boardtile.h"
#include "rules.h"
#include "utils.h"

// used to select the AI evaluation schema
enum EvaluationScheme
{
    def,
    basic,
    complex
};

// used to keep track of moves for when a move has
// to be undone.
typedef struct backUpMove
{
    Piece *backUpStartPiece;
    Piece *backUpEndPiece;
    Piece *backUpAdditionalPiece;
} BackUpMove;

class MinMaxABP
{
private:
    //    int debugCount = 0;
    BoardTile *(*grid)[8 * 8];
    Piece *(*whitePieces)[16];
    Piece *(*blackPieces)[16];
    Rules *game;
    //    bool color;
    int blackScore;
    int whiteScore;
    std::stack<backUpMove *> backUpMoves;
    std::stack<int> backUpEP;
    EvaluationScheme evalSchema;
    bool maxingColor;
    int searchDepth;

public:
    /**
     * @brief Construct a new Min Max alpha beta pruning AI.
     * 
     * @param _whitePieces Pointer to the list of white pieces array.
     * @param _blackPieces Pointer to the list of black pieces array.
     * @param _maxingColor The color which is being maximized by this AI.
     * @param depth The depth this min-max algorithm will search until.
     * @param _evalSchema The Evaluation scheme being used to find the best move.
     */
    MinMaxABP(BoardTile *(*_grid)[8 * 8], Piece *(*_whitePieces)[16], Piece *(*_blackPieces)[16], bool _maxingColor, int depth, EvaluationScheme _evalSchema);

    /**
     * @brief Called when truing to determine the best move for the current game board.
     * 
     * @param maximizing The color being maximized.
     * @param bestMove Pointer to the best move.
     */
    void minMax(bool maximizing, Move *bestMove);

    /**
     * @brief Get the Maxing Color for this min-max AI.
     * 
     * @return true Indicating white.
     * @return false Indicating black.
     */
    bool getMaxingColor() { return maxingColor; }

private:
    int minMaxHelper(uint depth, int alpha, int beta, bool maximizing, Move *bestMove);

    /**
     * @brief Used to evaluate the board state to determine
     * if this board state is likable for the maximizing/minimizing
     * side.
     * 
     * @return int A numerical value approximating the value of the
     *              board state.
     */
    int evaluate();

    /**
     * @brief A basic evaluation method which only cares
     * about the number of pieces on the board and the
     * only cares about the most valueable pieces.
     * 
     * @param maxingColor Currently unused
     * @return int A numerical value approximating the value of the
     *              board state.
     */
    int staticEvaluate();

    /**
     * @brief Uses a basic method of evaluation which involves
     * checking the board to determine how many attacks are being
     * made at this point and what the lowest value piece which
     * is attacking a potential position to which a piece is located.
     * 
     * @return int A numerical value approximating the value of the
     *              board state.
     */
    int basicEvaluate();

    /**
     * @brief TODO ...
     * 
     * @return int A numerical value approximating the value of the
     *              board state.
     */
    int complexEbaluate();

    /**
     * @brief Undoes the move previously made by using the stack
     * which keeps track of all the moves made in a LIFO fashion.
     * 
     * @param m The move which is being undone.
     */
    void unmakeMove(Move m, int depth, bool maximizing);

    /**
     * @brief Simulates the move on the actual game board which was
     * passed in when constructing the MinMaxABP object.
     * When simulating the move the pieces are moved on the board
     * but the ui is not updated to represent the changes. The method
     * is called implicitly by the makeMove function.
     * 
     * @param m The move being simulated.
     */
    void makeMove(Move m);

    /**
     * @brief Used to keep track of enpassant values such
     * that they can be undone when a move is undone.
     * 
     */
    void backUpEPValue();

    /**
     * @brief Used to restore the enpassant moves.
     * 
     */
    void restoreEPValue();

    inline void restoreAttackBoard();
};

#endif // MINMAXABP_H
