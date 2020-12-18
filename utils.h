#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>

static bool saveGame(BoardTile *tiles)
{
    std::ofstream file;
    file.open("savefile");

    // save state of board

    file.close();

    return true;
}

// unique character identifiers for pieces
static const char pawnID = 'p';
static const char rookID = 'r';
static const char knightID = 'k';
static const char bishopID = 'b';
static const char kingID = 'K';
static const char queenID = 'q';

#endif // UTILS_H
