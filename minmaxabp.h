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
    BoardTile *(*grid)[8][8];
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
    //    MinMaxABP(BoardTile *(*_grid)[8][8], Piece *(*_whitePieces)[16], Piece *(*_blackPieces)[16], bool _color, bool _maxingColor, EvaluationScheme _evalSchema = EvaluationScheme::def);
    MinMaxABP(BoardTile *(*_grid)[8][8], Piece *(*_whitePieces)[16], Piece *(*_blackPieces)[16], bool _maxingColor, int depth, EvaluationScheme _evalSchema);
    void minMax(bool maximizing, Move *bestMove);
    bool getMaxingColor() { return maxingColor; }
    ~MinMaxABP();

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
     * @brief Calls the cimulate method to perform a move. Once this
     * method has been called the unmake move has to be called
     * otherwise the gameboard will be left in aunexpected state.
     * 
     * @param m The move being performed.
     */
    void makeMove(Move m, int depth, bool maximizing);

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
    void simulateMove(Move m);

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
};

#endif // MINMAXABP_H
