#include "board.h"
#include "ui_board.h"
#include "utils.h"
#include "rules.h"

Piece *whitePieces[16];
Piece *blackPieces[16];
BoardTile *grid[8][8];
Border *borders[4];
Theme *currentTheme;
Rules *game;

int validMoves[64];
int vmIdx = 0;
bool isWhiteTurn = true;

//ssssssssssssssssssssssssssssss
void Board::setDebugWindowAccess(DebugWindow *dbw)
{
    game->setDebugWindowAccess(dbw);
}

Board::Board(QWidget *parent) : QMainWindow(parent), ui(new Ui::Board)
{
    ui->setupUi(this);

    createPieces(whitePieces, blackPieces);

    initializeGrid(grid, this);

    initializePieces(grid, whitePieces, blackPieces);

    game = game->getInstance();
}

Board::~Board()
{
    delete ui;
}


void Board::createPieces(Piece *whitePieces[16], Piece *blackPieces[16])
{
    int col = 0;
    int pidx = 0;
    while(col < 8)
    {
        whitePieces[pidx] = new Piece(true, pawnID, col, 6, col , whitePath + "pawn_white.svg");
        blackPieces[pidx] = new Piece(false, pawnID, col, 1, col, blackPath + "pawn_black.svg");
        col++;
        pidx++;
    }
    col = 0;

    whitePieces[pidx] = new Piece(true, rookID, col, 7, col , whitePath + "rook_white.svg");
    blackPieces[pidx] = new Piece(false, rookID, col, 0, col, blackPath + "rook_black.svg");
    col++; pidx++;

    whitePieces[pidx] = new Piece(true, knightID, col, 7, col , whitePath + "knight_white.svg");
    blackPieces[pidx] = new Piece(false, knightID, col, 0, col, blackPath + "knight_black.svg");
    col++; pidx++;

    whitePieces[pidx] = new Piece(true, bishopID, col, 7, col , whitePath + "bishop_white.svg");
    blackPieces[pidx] = new Piece(false, bishopID, col, 0, col, blackPath + "bishop_black.svg");
    col++; pidx++;

    whitePieces[pidx] = new Piece(true, queenID, col, 7, col , whitePath + "queen_white.svg");
    blackPieces[pidx] = new Piece(false, queenID, col, 0, col, blackPath + "queen_black.svg");
    col++; pidx++;

    whitePieces[pidx] = new Piece(true, kingID, col, 7, col , whitePath + "king_white.svg");
    blackPieces[pidx] = new Piece(false, kingID, col, 0, col, blackPath + "king_black.svg");
    col++; pidx++;

    whitePieces[pidx] = new Piece(true, bishopID, col, 7, col , whitePath + "bishop_white.svg");
    blackPieces[pidx] = new Piece(false, bishopID, col, 0, col, blackPath + "bishop_black.svg");
    col++; pidx++;

    whitePieces[pidx] = new Piece(true, knightID, col, 7, col , whitePath + "knight_white.svg");
    blackPieces[pidx] = new Piece(false, knightID, col, 0, col, blackPath + "knight_black.svg");
    col++; pidx++;

    whitePieces[pidx] = new Piece(true, rookID, col, 7, col , whitePath + "rook_white.svg");
    blackPieces[pidx] = new Piece(false, rookID, col, 0, col, blackPath + "rook_black.svg");
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

void Board::initializeGrid(BoardTile *grid[8][8], QWidget *_parent)
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
}

void Board::initializePieces(BoardTile *grid[8][8], Piece *whitePieces[16], Piece *blackPieces[16])
{
    int wr, wc, br, bc;
    for(int i = 0; i < 16; i++)
    {
        wr = whitePieces[i]->getRow();
        wc = whitePieces[i]->getCol();
        br = blackPieces[i]->getRow();
        bc = blackPieces[i]->getCol();

        grid[wr][wc]->setPiece(whitePieces[i]);
        grid[wr][wc]->displayTile();
        grid[br][bc]->setPiece(blackPieces[i]);
        grid[br][bc]->displayTile();
    }
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
                if (grid[i][j]->isOccupied())
                    grid[i][j]->removePiece();
            }

        initializePieces(grid, whitePieces, blackPieces);
    }
}

void Board::updateTheme(Theme::themes selection)
{
    currentTheme->setTheme(selection);

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            grid[i][j]->displayTile();
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

void Board::on_actionGreen_triggered()
{
    updateTheme(Theme::green);
}
