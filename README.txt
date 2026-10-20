# Checkers

A local two-player checkers game built with C++17 and SFML. Players share one window and use the mouse to select pieces and make moves on an 8 x 8 board.

## Features

- Graphical board with a highlight for the selected piece.
- Alternating turns between red and black; red moves first.
- Diagonal movement and single-jump captures.
- King promotion when a piece reaches the opposite end of the board.
- Kings can move one square or capture by jumping two squares in either diagonal direction.
- Winner announcement when one player has no pieces remaining.
- Move history written to moves.log when the window closes normally.

## Requirements

- A C++ compiler with C++17 support.
- SFML 2.x development headers and libraries for graphics, window, and system.
- arial.ttf in the working directory for the winner announcement.

This source uses the SFML 2 API and requires changes to build with SFML 3.

## Build and run

Keep these files in the same directory:

    main.cpp
    CheckersGame.cpp
    CheckersGame.h
    arial.ttf
    Makefile

From that directory, with SFML available in your compiler's header and library search paths:

    g++ -std=c++17 -O2 -Wall -Wextra main.cpp CheckersGame.cpp -o checkers -lsfml-graphics -lsfml-window -lsfml-system

On Linux or macOS:

    ./checkers

On Windows with a compatible MinGW/SFML installation:

    .\checkers.exe

You may need to configure include/library paths and make SFML's shared libraries available at runtime. Run from the project directory so the program can find arial.ttf.

### Using the supplied Makefile

The supplied Makefile builds FP (FP.exe on Windows):

    make

Run ./checkers on Linux/macOS or .\checkers.exe on Windows.

The Makefile's Windows paths currently point to Z:/CSCI200/include/ and Z:/CSCI200/lib/. Update INC_PATH and LIB_PATH for your own installation. Other platforms default to /usr/local/include/ and /usr/local/lib/. The Makefile also links SFML audio and network, although the game itself only uses graphics, window, and system.

## Controls

1. Click one of the current player's pieces to select it.
2. Click a legal destination square to move or capture.
3. Click the selected piece again to deselect it before choosing another piece.
4. Close the window to exit and save the move history.

There is currently no restart button or keyboard shortcut. Relaunch the application to start another game.

## Rules and current limitations

This project implements a simplified checkers ruleset:

- Ordinary pieces move and capture forward diagonally.
- Captures are optional; mandatory captures are not enforced.
- Each turn permits one move or one capture; multi-jump sequences are not implemented.
- Victory is detected when a player has no pieces remaining. A player with pieces but no legal moves is not currently detected as losing.
- Draw detection, an AI opponent, and online play are not implemented.

## Move log

Moves are collected in a std::vector<std::string> and saved to moves.log in the working directory when the window closes normally. Coordinates use zero-based (row, column) indexing.

Example entry:

    Red: (5,0)->(4,1)

If a player captures all opposing pieces, the log also records the winner. Each run overwrites the previous moves.log; it does not append to it. The log is a text record, not a saved game that can be loaded.

## Code structure

- main.cpp: creates the window, processes events, runs the render loop, and saves the move history on exit.
- CheckersGame.h: declares the Piece structure and CheckersGame class.
- CheckersGame.cpp: implements board setup, move validation, captures, promotion, victory detection, rendering, and logging.

The project demonstrates class design, header/source separation, event-driven input, board-state management, dynamic memory management, and file output. Rendering and game rules currently live in the same class.

## Possible improvements

- Enforce mandatory captures and multi-jump sequences.
- Detect when a player has no legal moves.
- Highlight legal destinations and display the current turn.
- Add a restart control and tests for game rules.
- Use value types or smart pointers for piece ownership.
- Add an AI opponent.

## Author and references

Author: Chris Hussain

References used during development: SFML.org, GeeksforGeeks.com, and CPlusPlus.com.
