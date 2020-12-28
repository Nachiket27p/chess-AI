#ifndef BOARD_H
#define BOARD_H

#include <QMainWindow>
#include <QtWidgets>
#include "boardtile.h"
#include "piece.h"
#include "debugwindow.h"

namespace Ui
{
    class Board;
}

class Board : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Construct a board object.
     * 
     * @param parent The parent widget of this QMainWindow.
     */
    explicit Board(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Board object
     * 
     */
    ~Board();

    /**
     * @brief This function is used to check if the resources required for the program
     * are available.
     * 
     * @return true If the resources are avaialble.
     * @return false If the resources cannot be located.
     */
    bool checkResources();

    //sssssssssssssssssssssssss
    void setDebugWindowAccess(DebugWindow *dbw);

private:
    // The ui object associated with this main window
    Ui::Board *ui;

    /**
     * @brief This function is used to change the theme of the board using the enums
     * located in the themes.h file.
     * 
     */
    void updateTheme(Theme::themes);

    void createPieces(Piece *whitePieces[16], Piece *blackPieces[16]);

    /**
     * @brief Initializes the tiles on the board grid.
     *
     * @param grid The grid of tiles representing the board.
     * @param _parent The parent widget to this board.
     */
    void initializeGrid(BoardTile *grid[8][8], QWidget *_parent);

    /**
     * @brief Initializes the pieces on the board.
     *
     * @param grid The grid of tiles representing the board.
     */
    void initializePieces(BoardTile *grid[8][8], Piece *whitePieces[16], Piece *blackPieces[16]);

    // These private slots are used to handel the actions performed by the user in the GUI menubar.
private slots:
    void on_actionNew_Game_triggered();
    void on_actionExit_triggered();
    void on_actionDefault_triggered();
    void on_actionWood_triggered();
    void on_actionGreen_triggered();
};

/**
 * @brief This class is used to construct the border around the board.
 * 
 */
class Border : public QLabel
{
public:
    /**
     * @brief Construct a new Border object
     * 
     * @param parent The parent widget to this object.
     */
    Border(QWidget *parent) : QLabel(parent) {}

    /**
     * @brief Specify the position and length of the border line.
     * 
     */
    void setOutline(int, int, bool);
};

#endif // BOARD_H
