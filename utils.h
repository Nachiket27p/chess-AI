#ifndef UTILS_H
#define UTILS_H

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <QMessageBox>
#include <QPixmap>

// unique character identifiers for pieces
const char pawnID = 'p';
const char rookID = 'r';
const char knightID = 'k';
const char bishopID = 'b';
const char kingID = 'K';
const char queenID = 'q';

// white piece for accessing QPixmap pointers.
const int pawnIDW = U'♙';
const int rookIDW = U'♖';
const int knightIDW = U'♘';
const int bishopIDW = U'♗';
const int kingIDW = U'♔';
const int queenIDW = U'♕';
// black
const int pawnIDB = U'♟';
const int rookIDB = U'♜';
const int knightIDB = U'♞';
const int bishopIDB = U'♝';
const int kingIDB = U'♚';
const int queenIDB = U'♛';

// names of the icon files in the pieces/<black/white>/ directory
const QString pawnIconName = "pawn.svg";
const QString rookIconName = "rook.svg";
const QString knightIconName = "knight.svg";
const QString bishopIconName = "bishop.svg";
const QString queenIconName = "queen.svg";
const QString kingIconName = "king.svg";

// get the current path of this file and use it to locate the images for the pieces.
// This method of locating the resources means the images of the pieces must always
// be in build directory.
const std::string currExecPath = get_current_dir_name();
const QString whitePath = QString::fromUtf8((currExecPath + "/resources/pieces/white/").c_str());
const QString blackPath = QString::fromUtf8((currExecPath + "/resources/pieces/black/").c_str());
// this function checks if the pieces directory exists to inform user on problem
static bool isPiecesDirExist()
{
    struct stat buffer;
    return (stat((currExecPath + "/resources").c_str(), &buffer) == 0);
}
// message to display if the pieces directory cannot be found.
const QString noPiecesDirMsg = "Could not locate \"resources\" directory. Pleace make sure the \"resources\" directory and it's contents are in the root directory of the program.";
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

#define PAWN_PROMOTION_DIALOG(PARENT, RTNVAL)                   \
    QMessageBox msgBox(this->parentWidget());                   \
    msgBox.setText("What would you like to promote to?");       \
    msgBox.addButton(tr("Bishop"), QMessageBox::AcceptRole);    \
    msgBox.addButton(tr("Knight"), QMessageBox::RejectRole);    \
    msgBox.addButton(tr("Rook"), QMessageBox::DestructiveRole); \
    msgBox.addButton(tr("Queen"), QMessageBox::ActionRole);     \
    RTNVAL = msgBox.exec();

// checks if the variable is in bound
#define WITHIN_BOUNDS(var) (((var) < 8))

// used to obtain moves from the rules class
typedef struct move
{
    uint startTileNumb;
    uint endTileNumb;
} Move;

// used to set default power values for the pieces
const std::unordered_map<char, uint> basePowerValues = {{pawnID, 10},
                                                        {rookID, 50},
                                                        {knightID, 30},
                                                        {bishopID, 30},
                                                        {queenID, 90},
                                                        {kingID, 900}};

#define idx(row, col) (((row)*8) + (col))

#endif // UTILS_H
