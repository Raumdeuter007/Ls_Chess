# L's Chess

Welcome to L's Chess: a battlefield of 64 squares, where strategy reigns supreme. With kings, queens, knights, and pawns, every move shapes the outcome. No dice, just intellect. It's a game of endless depth, offering a lifetime of challenge and discovery. Welcome to the timeless pursuit of victory.

## Setup
This program is made using Simple and Fast Multimedia Libraries [SFML](https://www.sfml-dev.org/) compatible with the MinGW G++ Compiler (x86_64-win32-seh-rev1, 13.1.0).

**To Play the Game**, clone this repository in your computer and run L's Chess.exe. Alternatively, you can download L's Chess.msi (within the repository) and install it.

**To Modify the Game**, copy the script in main.cpp and piece.h into your Visual Studio Community Project with SFML linked to it. 
If you do not have SFML linked to your project, follow the steps below to create your environment:

1. Install the mingw64 folder of the repository in your C Drive.
2. Search for "Edit environment variables ...." as shown below: <p></p>  
![Step 2](Assets/step1.png)
    Make sure that you access account variables and not system variables!
<p></p>

3. Upon clicking, the following window will open: <p></p>
    ![Step 3](Assets/step2.png)
    Click on the Path Variable and edit it.
    <p></p>

4. Add this Path into your computer:
    ```bash
    C:\mingw64\bin
    ```
    Like This:

    ![Step 4](Assets/step3.png)

<p></p>
Now the compiler will run successfully for your program.
<p></p>
To compile the code after modifying the script (keep it in the same folder), execute the following in your command prompt:<p></p>

```bash
mingw32-make -f MakeFile
```

This will run the following commands

```bash
g++ -Isrc/include -c main.cpp
g++ main.o -o "L's Chess" -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

This integrates SFML in your program and L's Chess.exe file is created. Now run the L's Chess.exe program to start the game.

## Features

* Players can choose between 5 time-controls which are: 
    * 2 + 1 (Bullet)
    * 3 + 2 (Blitz with increment)
    * 5 + 0 (Blitz without increment)
    * 10 + 0 (Rapid without increment)
    * 10 + 5 (Rapid with increment)
* The game handles the basic moves of all types of pieces and the special moves (_en passant_, _castling_ and _pawn promotion_) and also checks for _checks_, _pins_ and _checkmate_.
* On Timeout, the opponent wins _irrespective_ of _insufficient material_. 
* _Draw by Stalemate_ is detected by the program. 
* Last 10 moves of the game are shown in chess notation at the side of the board.
* On a conclusive end, the game is recorded in the file, *record.txt*. 
* Sound Effects for moves, captures, checks and checkmate implemented.
 
## Suggested Improvements

* Draw the sprites of the captured pieces with the point difference for the person with more material.
* Other types of draws (_Repitition_, _Insufficient Material_, _50-move rule etc_) can be integrated into the program.
* Notation needs to be checked for same type of piece that can move to the same square (row character or the rank number is added for _uniqueness_)
* More time controls may be added to cater for classical chess.
* A database may be maintained to record every game played in the program (_can be integrated with network to store accounts_).
* Drag-and-Drop for pieces would elevate user experience (UX).
* The game could be linked to a network to allow players from different computers to play together with a server (_For very ambitious programmers_).

## Authors and Acknowledgement

This game was made by *Muhammad Luqman Arshad* as a final project for the course _Object Oriented Programming_ instructed by _Mr. Aamir Raheem_ at _FAST-NUCES Spring '24_.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) for details.