CHECKERS

PROBLEM DESCRIPTION
A simple two‑player checkers game written in C++ that opens a graphical 8 × 8 board with SFML, lets two players click pieces to move and capture, 
promotes kings, keeps turn order, and logs every move. The goal is to demonstrate class design, basic list usage, file I/O, and use of SFML.

PROGRAM DOCUMENTATION
Build (Linux / macOS / MinGW):
    g++ src/*.cpp -Iinclude -lsfml-graphics -lsfml-window -lsfml-system -std=c++17 -O2 -o checkers
    Requires the SFML development libraries in your compiler’s search path.
Run:
    ./FP
  A 680 × 680 window opens.  Click a piece then a legal square to move or capture.
  Closing the window appends the move history and final result to moves.log.
  Text shows a “Red/Black wins!” once the game ends
Potential extensions:
  – Forced‑capture rule enforcement
  – Highlight legal moves on hover
  – AI opponent (I wanted to try this but didn't have the knowledge or time)
  – International “flying kings” rules
  – Animated piece movement

CLASS DESCRIPTION
Custom Class: CheckersGame
Encapsulated data: 8×8 Piece* board, turn flag, selection state, move history list.
Encapsulated functionality: board reset, pixel to square conversion, move validation,
capture logic, promotion, draw routine, victory detection, and move and win log writing.
Creating this class isolated all game logic from the GUI loop, kept global scope
clean, and can make future extensions straightforward.

LIST DATA STRUCTURE
Chosen structure: std::vector<std::string> moves
Why: A vector offers simple push‑back and fast iteration
ideal fit for an ever‑growing move list that is written out once at
program end. No random deletions were needed, so a linked list wouldn't be needed.

FILE I/O
File: moves.log 
Purpose: A record of every move and the final outcome so a
player can review a completed game. Shows basic output‑file creation.
It could be necessary if you would like to improve your checkers gameplay 
by looking over how you played.

PROJECT MODIFICATIONS
The initial proposal aimed for international rules with “flying kings,” multi jump
chains and an AI opponent if you didn't have friends to play with. 
the final version implements American rules (one square kings and single captures).
A frame rate cap was added after play testing revealed runaway CPU usage that
froze my computer. A victory overlay and last line log entry
were added so players know who won without inspecting the console.

REFLECTIONS
This project solidified my understanding of class design, header and source
separation, and event driven graphics. Debugging pointer mistakes helped me 
learn how pointers really worked. I also learned to throttle my render loop early and
to prototype with minimal rules before tackling more complex issues. Overall, the
project helped me learn to consider user needs when making an application.
