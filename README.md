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
* Extensive modifications have been been made such that limited similarities exist.
* Extensive testing has not been performed, there may be some bugs especially with the AI.
* The game allows the ability to play 2-player or player vs. AI, and AI vs. AI.
* Although there are three different play types are available, small changes have to be made to the source code do so.
  * To change to 2-player mode start the program and select '**File**' and then select '**2-Player**'.

  * To change to player vs AI mode start the program and select '**File**' and then select '**Vs. AI**'.

  * To view AI vs AI use start the program and select '**File**' on the top left and select AI vs. AI.
    * To modify the AI the depth, and type of evaluation can be modified by changing making changes to the '**board.cpp**' file at the very end where the tow AI are constructed.
    * Although this mode is slightly buggy and ends up in a situation where nearing the end of the game the same set of moves are repeated and no progress is made.

> ## About Min-Max Alpha-Beta pruning algo
The algorithm is encapsulate within the MinMaxABP class, it can be initialized with a two different evaluation methods:
* **Default Heuristic** - Which only takes into account the amount of pieces left on the board for both sides, it does this using a set of static powervalues assigned to different types of pieces. These values were obtained from the standard values assigned to chess pieces (**https://en.wikipedia.org/wiki/Chess_piece_relative_value**) and multiplied by 10 to allow for more fine control.
  * pawn = 10, rook = 50, knight = 30, bishop = 30, queen = 90, king = 900

* **Basic Heuristic** - Does everything the default argument does but also takes into account how many pieces are reinforcing a piece and how many of the opponents pieces are attacking the piece. This was an attempt to allow for more human behavior when deciding to move pieces when there are multiple outcomes with the same value using the default evaluation.

* **Advanced Heuristic** - Not implemented yet.

> ## Overview of how moves are validated, check is detected, check/stale mate is detected
* Moves are highlighted and validated by the Rules class, which upon selecting a piece to move highlights all the valid moves which can be taken.
* To aid in detecting a check, two grids are kept for each side, the grid keeps track of all the positions each side can attack/move to. If the (row,col) position which the king is located on has a positive value on the opponents attack grid then the king is in check.
* Check mate and stale mate is detected by requesting valid moves for all the remaining pieces of black/white side and if there is a move then neither is true. If ther are no moves available and there is a check then it is a checkmate, if there are no moves and there is no check then it is a stalemate.
* Fore a detailed overview please refer to the rules.cpp file which contains all the code which determines valid moves.

> ## Performance
* The performance was evaluated on the following system.
    * Linux with kernel 5.8
    * i7-7700HQ
    * 16GB ram
* Initially 2d arrays were used for the boardTiles, and attack grids. This meant they were created as a 1d array of pointers to 8 other arrays of size 8. This meant there was no guarantee that this 2d array as a whole would be located in the same memory region. This means when data is bough into cache there is a good chance that not all 64 elements would be bough in the same block. This could lead to higher chances of having to swap out cache blocks due to cache-misses. This was rectified by creating a 1d array of size 64 which can be referenced with some index math.
  * This performance before and after was measured by timing how long it took AI vs AI took to come to a stalemate.
    * Both AI were set to depth 4 and used the default evaluation heuristics.
  * The performance is shown below.
          
        Before                               \             After
        real    0m39.121s           ----------\            real    0m32.387s
        user    0m27.414s           ----------/            user    0m23.016s
        sys     0m9.755s                     /             sys     0m7.912s
  * This shows a ~18% performance boost.
  * Something to note is that the debugging window which displayed the attack grid values was still enabled so the absolute time values are not important but rather the percentage improvement.

* The next optimization was removing the need to create **new QPixmap** for the icons of each piece and rather using a unorderedmap which help pointers to the QPixmap objects for each icon. This also meant the copy constructor for the **Piece** class was modified to not need to create a **new QPixmap** only reassign the appropriate pointer obtained from the unordered map.
  * As expected this lead to a large performance boost as shown in the numbers below.

        Before                                \             After
        real    0m32.387s            ----------\            real    0m16.288s
        user    0m23.016s            ----------/            user    0m14.512s
        sys     0m7.912s                      /             sys     0m0.104s
  * This shows a ~50% performance boost.
  * Someting to note is that the debugging window which displayed the attack grid values was still enabled so the absolute time values are not inportant but rather the percentage impeovement.

* The final optimization is less of an optimization an more of an expected disabeling of debugging window.
  * The performance boost is enormous as expected due the the large number of ui calls which were being made to display the attack grid values.
        
        Before                                \             After
        real    0m16.288s            ----------\            real    0m3.935s
        user    0m14.512s            ----------/            user    0m2.520s
        sys     0m0.104s                      /             sys     0m0.091s
  * This shows a ~75% performance boost.

* As shown by the results for the best performance disable the debugging window, and use -o3 optimization.
* With these optimizations a depth of 7 can be used without having to wait a significant amount of time for the AI to make its move. The time taken can vary but the wait time for the AI to take its turn is ~10-15s.
* Increasing the depth does increase the memory requirements so consider your system specs before increasing the depth. Also your processor will affect the compute times.
        
        
        
