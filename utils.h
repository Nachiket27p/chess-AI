#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <QMessageBox>
#include "boardtile.h"

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

// get the current path of this file and use it to locate the images for the pieces.
// This method of locating the resources means the images of the pieces must always
// be in build directory.
static std::string currExecPath = get_current_dir_name();
static const QString whitePath = QString::fromUtf8((currExecPath + "/pieces/white/").c_str());
static const QString blackPath = QString::fromUtf8((currExecPath + "/pieces/black/").c_str());
// this function checks if the pieces directory exists to inform user on problem
static bool isPiecesDirExist()
{
    struct stat buffer;
    return (stat((currExecPath + "/pieces").c_str(), &buffer) == 0);
}
// message to display if the pieces directory cannot be found.
static const QString noPiecesDirMsg = "Could not locate \"pieces\" directory. Pleace make sure the \"pieces\" directory and it's contents are in the root directory of the program.";
// Base string for the path location of the svg images of the pieces
// static const QString whitePath = "/home/nachiket/Documents/qt5-workspace/chess-AI/pieces/white/";
// static const QString blackPath = "/home/nachiket/Documents/qt5-workspace/chess-AI/pieces/black/";

// macro used to create confirm dialogs
#define CONFIRM_DIALOG(PARENT, RTNVAL, MESSAGE)                       \
    QMessageBox msgBox(PARENT);                                       \
    msgBox.setText(MESSAGE);                                          \
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel); \
    msgBox.setDefaultButton(QMessageBox::Ok);                         \
    RTNVAL = msgBox.exec();

#define PAWN_PROMOTION_DIALOG(PARENT, PIECE, TURN)              \
    QMessageBox msgBox(this->parentWidget());                   \
    msgBox.setText("What would you like to promote to?");       \
    msgBox.addButton(tr("Bishop"), QMessageBox::AcceptRole);    \
    msgBox.addButton(tr("Knight"), QMessageBox::RejectRole);    \
    msgBox.addButton(tr("Rook"), QMessageBox::DestructiveRole); \
    msgBox.addButton(tr("Queen"), QMessageBox::ActionRole);     \
    int promoPiece = msgBox.exec();                             \
    if (promoPiece == QMessageBox::AcceptRole)                  \
    {                                                           \
        PIECE = "bishop_black.svg";                             \
        if (TURN)                                               \
            PIECE = "bishop_white.svg";                         \
    }                                                           \
    else if (promoPiece == QMessageBox::RejectRole)             \
    {                                                           \
        PIECE = "knight_black.svg";                             \
        if (TURN)                                               \
            PIECE = "knight_white.svg";                         \
    }                                                           \
    else if (promoPiece == QMessageBox::DestructiveRole)        \
    {                                                           \
        PIECE = "rook_black.svg";                               \
        if (TURN)                                               \
            PIECE = "rook_white.svg";                           \
    }                                                           \
    else if (promoPiece == QMessageBox::ActionRole)             \
    {                                                           \
        PIECE = "queen_black.svg";                              \
        if (TURN)                                               \
            PIECE = "white_black.svg";                          \
    }

// checks if the variable is in bound
#define WITHIN_BOUNDS(var) (((var) >= 0) && ((var) < 8))

#endif // UTILS_H
