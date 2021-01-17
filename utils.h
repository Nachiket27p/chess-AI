#ifndef UTILS_H
#define UTILS_H

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <QMessageBox>

// unique character identifiers for pieces
static const char pawnID = 'p';
static const char rookID = 'r';
static const char knightID = 'k';
static const char bishopID = 'b';
static const char kingID = 'K';
static const char queenID = 'q';

// names of the icon files in the pieces/<black/white>/ directory
static const QString pawnIconName = "pawn.svg";
static const QString rookIconName = "rook.svg";
static const QString knightIconName = "knight.svg";
static const QString bishopIconName = "bishop.svg";
static const QString queenIconName = "queen.svg";
static const QString kingIconName = "king.svg";

// get the current path of this file and use it to locate the images for the pieces.
// This method of locating the resources means the images of the pieces must always
// be in build directory.
static std::string currExecPath = get_current_dir_name();
static const QString whitePath = QString::fromUtf8((currExecPath + "/resources/pieces/white/").c_str());
static const QString blackPath = QString::fromUtf8((currExecPath + "/resources/pieces/black/").c_str());
// this function checks if the pieces directory exists to inform user on problem
static bool isPiecesDirExist()
{
    struct stat buffer;
    return (stat((currExecPath + "/resources").c_str(), &buffer) == 0);
}
// message to display if the pieces directory cannot be found.
static const QString noPiecesDirMsg = "Could not locate \"resources\" directory. Pleace make sure the \"resources\" directory and it's contents are in the root directory of the program.";
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

#define PAWN_PROMOTION_DIALOG(PARENT, PIECE)                    \
    QMessageBox msgBox(this->parentWidget());                   \
    msgBox.setText("What would you like to promote to?");       \
    msgBox.addButton(tr("Bishop"), QMessageBox::AcceptRole);    \
    msgBox.addButton(tr("Knight"), QMessageBox::RejectRole);    \
    msgBox.addButton(tr("Rook"), QMessageBox::DestructiveRole); \
    msgBox.addButton(tr("Queen"), QMessageBox::ActionRole);     \
    int promoPiece = msgBox.exec();                             \
    if (promoPiece == QMessageBox::AcceptRole)                  \
    {                                                           \
        PIECE = bishopIconName;                                 \
    }                                                           \
    else if (promoPiece == QMessageBox::RejectRole)             \
    {                                                           \
        PIECE = knightIconName;                                 \
    }                                                           \
    else if (promoPiece == QMessageBox::DestructiveRole)        \
    {                                                           \
        PIECE = rookIconName;                                   \
    }                                                           \
    else if (promoPiece == QMessageBox::ActionRole)             \
    {                                                           \
        PIECE = queenIconName;                                  \
    }

// checks if the variable is in bound
#define WITHIN_BOUNDS(var) (((var) >= 0) && ((var) < 8))

// used to obtain moves from the rules class
typedef struct move
{
    int startTileNumb;
    int endTileNumb;
} Move;

// used to set default power values for the pieces
static std::unordered_map<char, int> basePowerValues = {{pawnID, 10},
                                                        {rookID, 50},
                                                        {knightID, 30},
                                                        {bishopID, 30},
                                                        {queenID, 90},
                                                        {kingID, 900}};

#endif // UTILS_H
