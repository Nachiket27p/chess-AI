#include "board.h"
#include "ui_board.h"
#include "utils.h"
#include "rules.h"
#include "minmaxabp.h"

// keeps track of the black and white pieces on the board
Piece *whitePieces[16];
Piece *blackPieces[16];

// 2d array of BoadrTile objects which represent the board
BoardTile *grid[8][8];

Theme *currentTheme;
Rules *game;
MinMaxABP *mmabp;

Board::Board(QWidget *parent) : QMainWindow(parent), ui(new Ui::Board)
{
    // initialize the ui.
    ui->setupUi(this);

    QPixmap bkgnd("resources/board.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    // create the pieces.
    createPieces(whitePieces, blackPieces);

    // initialize the grid/board.
    initializeGrid(grid, this);

    // initialize the pieces onto the grid.
    initializePiecesOnGrid(grid, whitePieces, blackPieces);

    game = game->getInstance();
    bool maximizingColor = false;
    mmabp = new MinMaxABP(&grid, &whitePieces, &blackPieces, maximizingColor, false, EvaluationScheme::basic);
}

Board::~Board()
{
    delete ui;
}

void Board::createPieces(Piece *whitePieces[16], Piece *blackPieces[16])
{
    int col = 0;
    int pidx = 0;
    int blackTileNumb = 8;
    int whiteTileNumb = 48;
    // create the pawns
    while (col < 8)
    {
        whitePieces[pidx] = new Piece(true, pawnID, whiteTileNumb + col, 6, col, pidx, whitePath + pawnIconName);
        blackPieces[pidx] = new Piece(false, pawnID, blackTileNumb + col, 1, col, pidx, blackPath + pawnIconName);
        col++;
        pidx++;
    }
    // reset variables to initialize the second row of pieces
    col = 0;
    blackTileNumb = 0;
    whiteTileNumb = 56;

    // create the A column rooks
    whitePieces[pidx] = new Piece(true, rookID, whiteTileNumb + col, 7, col, pidx, whitePath + rookIconName);
    blackPieces[pidx] = new Piece(false, rookID, blackTileNumb + col, 0, col, pidx, blackPath + rookIconName);
    col++;
    pidx++;
    // create the B  column knights
    whitePieces[pidx] = new Piece(true, knightID, whiteTileNumb + col, 7, col, pidx, whitePath + knightIconName);
    blackPieces[pidx] = new Piece(false, knightID, blackTileNumb + col, 0, col, pidx, blackPath + knightIconName);
    col++;
    pidx++;
    // create the C column bishops
    whitePieces[pidx] = new Piece(true, bishopID, whiteTileNumb + col, 7, col, pidx, whitePath + bishopIconName);
    blackPieces[pidx] = new Piece(false, bishopID, blackTileNumb + col, 0, col, pidx, blackPath + bishopIconName);
    col++;
    pidx++;
    // create the queens
    whitePieces[pidx] = new Piece(true, queenID, whiteTileNumb + col, 7, col, pidx, whitePath + queenIconName);
    blackPieces[pidx] = new Piece(false, queenID, blackTileNumb + col, 0, col, pidx, blackPath + queenIconName);
    col++;
    pidx++;
    // create the kings
    whitePieces[pidx] = new Piece(true, kingID, whiteTileNumb + col, 7, col, pidx, whitePath + kingIconName);
    blackPieces[pidx] = new Piece(false, kingID, blackTileNumb + col, 0, col, pidx, blackPath + kingIconName);
    col++;
    pidx++;
    // create the F column bishops
    whitePieces[pidx] = new Piece(true, bishopID, whiteTileNumb + col, 7, col, pidx, whitePath + bishopIconName);
    blackPieces[pidx] = new Piece(false, bishopID, blackTileNumb + col, 0, col, pidx, blackPath + bishopIconName);
    col++;
    pidx++;
    // create the G column knights
    whitePieces[pidx] = new Piece(true, knightID, whiteTileNumb + col, 7, col, pidx, whitePath + knightIconName);
    blackPieces[pidx] = new Piece(false, knightID, blackTileNumb + col, 0, col, pidx, blackPath + knightIconName);
    col++;
    pidx++;
    // create the H column rooks
    whitePieces[pidx] = new Piece(true, rookID, whiteTileNumb + col, 7, col, pidx, whitePath + rookIconName);
    blackPieces[pidx] = new Piece(false, rookID, blackTileNumb + col, 0, col, pidx, blackPath + rookIconName);
}

bool Board::checkResources()
{
    // check if the pieces are in the folder/directory so they
    // can be displayed.
    if (!isPiecesDirExist())
    {
        int rtnVal = 0;
        CONFIRM_DIALOG(this, rtnVal, noPiecesDirMsg);
        return false;
    }
    return true;
}

void Border::setOutline(int x, int y, bool vertical)
{
    // if the border being set is vertical or horizontal
    if (!vertical)
        setGeometry(x, y, 820, 10);
    else
        setGeometry(x, y, 10, 820);

    // set the style sheet
    setStyleSheet(QString("QLabel {background-color: rgb(45, 45, 45); color: black;}"));
}

void Board::initializeGrid(BoardTile *grid[8][8], Board *_parent)
{
    // create the theme object and set it to the default.
    currentTheme = new Theme();
    currentTheme->setTheme(Theme::green);

    // create the borders around the grid
    for (int i = 0; i < 4; i++)
        borders[i] = new Border(_parent);

    // set size of each side of grid border
    borders[0]->setOutline(45, 82, false);
    borders[1]->setOutline(45, 892, false);
    borders[2]->setOutline(45, 82, true);
    borders[3]->setOutline(855, 82, true);

    // create the board tiles which will represent the board and hold the pieces.
    int tnum = 0;
    int y = 92;
    for (int row = 0; row < 8; row++) // set up tiles
    {
        int x = 55;
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

void Board::initializePiecesOnGrid(BoardTile *grid[8][8], Piece *whitePieces[16], Piece *blackPieces[16])
{
    int wr, wc, br, bc;
    // use the pieces to initialize the grid.
    for (int i = 0; i < 16; i++)
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

void Board::resetPieces()
{
    int sr = 8;
    for (int col = 0; col < 8; col++)
    {
        // black pieces
        blackPieces[col]->resetPiece(1, col);
        blackPieces[sr + col]->resetPiece(0, col);

        // white pieces
        whitePieces[col]->resetPiece(6, col);
        whitePieces[sr + col]->resetPiece(7, col);
    }
}

bool Board::saveGame(BoardTile *tiles)
{
    std::ofstream file;
    file.open("savefile");

    // save state of board

    file.close();

    return true;
}

void Board::on_actionNew_Game_triggered()
{
    int rtnval = 0;
    // prompt the user if they are sure they want to start a new game
    CONFIRM_DIALOG(this, rtnval, "Are you sure you want to start a new game?");

    // if the response if 'ok' then reset all the pieces on the grid.
    if (rtnval == QMessageBox::Ok)
    {
        //        isWhiteTurn = true;
        game->setTurn(true);

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                grid[i][j]->removePiece();
                grid[i][j]->displayTile();
            }
        }
        resetPieces();
        initializePiecesOnGrid(grid, whitePieces, blackPieces);
    }
}

void Board::updateTheme(Theme::themes selection)
{
    // set the new theme
    currentTheme->setTheme(selection);

    // update all the tiles on the board by going through the 8x8 BoardTile array
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
