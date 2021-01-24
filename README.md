# Min-Max Alpha-Beta pruning Chess AI

> ## Requirements
* QT 5
* gcc/g++9
* CMake
* Visual Studio code / QT Creator
* There is a directory called '**resources**' which must be placed in the same directory as the executable file for pieces and board to load.
* If QT Creator is used then remember to place the '**resources**' directory in the build/executable directory.
* If the performance seems slow compile with '**-o3**' flag, if using VSC change cmake build version to '**Release**.


> ## INFO
* The base UI was inspired by **https://github.com/rbborashan/Chess-QT**.
* Extensive modifications have beem been made such that limited similarities exist.
* Extensive testing has not been performed, there may be some bugs especially with the AI.
* The game allows the ability to play 2-player or player vs. AI, and AI vs. AI.
* Althouth there are three ddifferent play types are available, small changes have to be made to the source code do so.
  * To change to 2-player mode, modify the file **boardTile.cpp** by commenting out:

        // call AI                                                          // // call AI
        if (playerMove)                                    \                // if (playerMove)
        {                                           --------\               // {
            aiMove(mmabp->getMaxingColor());        --------/               //     aiMove(mmabp->getMaxingColor());
        }                                                  /                // }

  * To change to player vs AI mode, modify the file **boardTile.cpp** by un-commenting out:

        // // call AI                                                          // call AI
        // if (playerMove)                                    \                if (playerMove)
        // {                                           --------\               {
        //     aiMove(mmabp->getMaxingColor());        --------/                   aiMove(mmabp->getMaxingColor());
        // }                                                  /                }

  * To watch AI vs AI use start the program and select '**File**' on the top left and select AI vs. AI.
    * To modify the AI the depth, and type of evaluation can be modified by changing making changes to the '**board.cpp**' file at the very end where the tow AI are constructed.
    * Although this mode is slightly buggy and ends up in a situation where nearing the end of the game the same set of moves are repeated and no progrss is made.

> ## About Min-Max Alpha-Beta pruning algo
The algorithm is encapsulted within the MinMaxABP class, it can be initialized with a two different evaluation methods:
* **Default** - Which only takes into account the amount of pieces left on the board for both sides, it does this using a set of static powervalues assigned to different types of pieces. These values were obtained from the standard values assigned to chess pieces (**https://en.wikipedia.org/wiki/Chess_piece_relative_value**) and multiplied by 10 to allow for more fine control.
  * pawn = 10, rook = 50, knight = 30, bishop = 30, queen = 90, king = 900

* **Basic** - Does everything the default argument does but also takes into account how many pieces are reinforcing a piece and how many of the opponents pieces are attacking the piece. This was an attempt to allow for more human behavior when deciding to move pieces when there are multiple outcomes with the same value using the default evaluation.

* **Advanced** - Not implemented yet.

> ## Overview of how moves are validated, check is detected, check/stale mate is detected
* Moves are highlighed and validated by the Rules class, which upon selecting a piece to move highlights all the valid moves which can be taken.
* To aid in detecting a check, two grids are kept for each side, the grid keeps track of all the positions each side can attack/move to. If the (row,col) position which the king is located on has a positive value on the opponents attack grid then the king is in check.
* Check mate and stale mate is detected by requesting valid moves for all the remaining pieces of black/white side and if there is a move then neither is true. If ther are no moves available and ther is a check then it is a checkmate, if there are no moves and there is no check then it is a stalemate.
* Fore a detailed overview please refer to the rules.cpp file which contains all the code which determines valid moves.
