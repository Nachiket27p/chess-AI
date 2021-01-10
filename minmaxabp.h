#ifndef MINMAXABP_H
#define MINMAXABP_H

#include <unordered_map>
#include <vector>
#include "boardtile.h"
#include "rules.h"
#include "utils.h"

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
    BoardTile *startTile;
    BoardTile *endTile;

public:
    MinMaxABP(BoardTile *(*_grid)[8][8], Piece *(*_whitePieces)[16], Piece *(*_blackPieces)[16], bool _color);
    int minMax(int depth, bool maximizing, bool maxingColor, Move *bestMove);

private:
    int evaluate(bool maxingColor);
    void makeMove(Move m);
    void unmakeMove(Move m);
    void simulateMove(Move m);
};

#endif // MINMAXABP_H
