#ifndef BOARD_H
#define BOARD_H

#include <QMainWindow>
#include <QtWidgets>
#include "boardtile.h"
#include "piece.h"
#include "debugwindow.h"

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

namespace Ui
{
    class Board;
}

/**
 * @brief This class is the main class which start the QT user interface and
 * is parent to the BoardTile which is used to create an 8x8 grid representing
 * board.
 */
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

private:
    // The ui object associated with this main window
    Ui::Board *ui;

    // object represents the borders
    Border *borders[4];

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
    void initializeGrid(BoardTile *grid[8][8], Board *_parent);

    /**
     * @brief Initializes the pieces on the board. This function sould be called after
     * 'Piece *blackPieces[16]' and 'Piece *blackPiece[16]' have been initialized.
     *
     * @param grid The grid of tiles representing the board.
     */
    void initializePiecesOnGrid(BoardTile *grid[8][8], Piece *whitePieces[16], Piece *blackPieces[16]);

    /**
     * @brief Used to reset the pieces back to their original positions on the board.
     */
    void resetPieces();

    /**
     * @brief Saves the game state. TODO
     * @param tiles
     * @return
     */
    bool saveGame(BoardTile *tiles);

public slots:
    /**
     * @brief This slot is public as it is used to reset the game when a check or stale mate is detected.
     */
    void on_actionNew_Game_triggered();

    // These private slots are used to handel the actions performed by the user in the GUI menubar.
private slots:
    /**
     * @brief This private slot is triggered when the application is quit.
     */
    void on_actionExit_triggered();
    /**
     * @brief This slot is triggered when the theme is set to default.
     */
    void on_actionDefault_triggered();
    /**
     * @brief This slot is triggered when the wood theme is selected.
     */
    void on_actionWood_triggered();
    /**
     * @brief This slot is triggered when the green theme is selected.
     */
    void on_actionGreen_triggered();
};

#endif // BOARD_H
