#include "board.h"
#include "ui_board.h"
#include "utils.h"

BoardTile *board[8][8];
Border *borders[4];
Theme *currentTheme;

int validMoves[64];
int listIndex = 0;
bool isWhiteTurn = true;

Board::Board(QWidget *parent) : QMainWindow(parent), ui(new Ui::Board)
{
    ui->setupUi(this);

    initialize(board, this);
}

Board::~Board()
{
    delete ui;
}

bool Board::checkResources()
{
    if (!isPiecesDirExist())
    {
        int rtnVal = 0;
        CONFIRM_DIALOG(rtnVal, noPiecesDirMsg);
        return false;
    }
    return true;
}

void Border::setOutline(int x, int y, bool vertical)
{
    if (!vertical)
        setGeometry(x, y, 820, 10);
    else
        setGeometry(x, y, 10, 820);

    setStyleSheet(QString("QLabel {background-color: rgb(45, 45, 45); color: black;}"));
}

void initialize(BoardTile *grid[8][8], QWidget *_parent)
{
    currentTheme = new Theme();
    currentTheme->setTheme(Theme::def);

    for (int i = 0; i < 4; i++)
        borders[i] = new Border(_parent);

    borders[0]->setOutline(100, 70, false);
    borders[1]->setOutline(100, 880, false);
    borders[2]->setOutline(100, 70, true);
    borders[3]->setOutline(910, 70, true);

    int tnum = 0;
    int y = 80;
    for (int row = 0; row < 8; row++) // set up tiles
    {
        int x = 110;
        for (int col = 0; col < 8; col++)
        {
            grid[row][col] = new BoardTile(row, col, tnum++, ((row + col) & 1), _parent);
            grid[row][col]->setGeometry(x, y, 100, 100);
            grid[row][col]->displayTileColor();
            x += 100;
        }
        y += 100;
    }

    initializePieces(grid);
}

void initializePieces(BoardTile *grid[8][8])
{
    // pawns
    for (int col = 0; col < 8; col++)
    {
        grid[1][col]->setPiece(pawnID, false);
        grid[6][col]->setPiece(pawnID, true);
    }

    // black pieces
    grid[0][0]->setPiece(rookID, false);
    grid[0][1]->setPiece(knightID, false);
    grid[0][2]->setPiece(bishopID, false);
    grid[0][3]->setPiece(queenID, false);
    grid[0][4]->setPiece(kingID, false);
    grid[0][5]->setPiece(bishopID, false);
    grid[0][6]->setPiece(knightID, false);
    grid[0][7]->setPiece(rookID, false);

    // white pieces
    grid[7][0]->setPiece(rookID, true);
    grid[7][1]->setPiece(knightID, true);
    grid[7][2]->setPiece(bishopID, true);
    grid[7][3]->setPiece(queenID, true);
    grid[7][4]->setPiece(kingID, true);
    grid[7][5]->setPiece(bishopID, true);
    grid[7][6]->setPiece(knightID, true);
    grid[7][7]->setPiece(rookID, true);
}

void Board::on_actionNew_Game_triggered()
{
    int rtnval = 0;
    CONFIRM_DIALOG(rtnval, "Are you sure you want to start a new game?");

    if (rtnval == QMessageBox::Ok)
    {
        isWhiteTurn = true;

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
            {
                if (board[i][j]->isOccupied())
                    board[i][j]->removePiece();
            }

        initializePieces(board);
    }
}

void Board::updateTheme(Theme::themes selection)
{
    currentTheme->setTheme(selection);

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board[i][j]->populateTile(board[i][j]->getPieceSymbol(), board[i][j]->getPieceColor());
}

void Board::on_actionExit_triggered()
{
    // Code to save game
    this->close();
}

void Board::on_actionDefault_triggered()
{
    updateTheme(Theme::def);
}

void Board::on_actionWood_triggered()
{
    updateTheme(Theme::wood);
}
