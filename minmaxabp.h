#ifndef MINMAXABP_H
#define MINMAXABP_H

#include <stack>
#include <vector>
#include "boardtile.h"
#include "rules.h"
#include "utils.h"

typedef struct backUpMove
{
    Piece *backUpStartPiece;
    Piece *backUpEndPiece;
    Piece *backUpAdditionalPiece;
} BackUpMove;


class MinMaxABP
{
private:
    BoardTile *(*grid)[8][8];
    Piece *(*whitePieces)[16];
    Piece *(*blackPieces)[16];
    Rules *game;
    bool color;
    int blackScore;
    int whiteScore;
    std::stack<backUpMove*> backUpMoves;
    std::stack<int> backUpEP;

public:
    MinMaxABP(BoardTile *(*_grid)[8][8], Piece *(*_whitePieces)[16], Piece *(*_blackPieces)[16], bool _color);
    int minMax(int depth, int *alpha, int *beta, bool maximizing, bool maxingColor, Move *bestMove);

private:
    int evaluate(bool maxingColor);
    int initialEvaluate(bool maxingColor);
    void makeMove(Move m);
    void unmakeMove(Move m, bool turn);
    void simulateMove(Move m);
    void backUpEPValue();
    void restoreEPValue();
};

#endif // MINMAXABP_H
