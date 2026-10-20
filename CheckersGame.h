/*
* Author: Chris Hussain
*
* Resources: SFML.org GeeksforGeeks.com CPlusPlus.com
*/

#ifndef CHECKERS_GAME_H
#define CHECKERS_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Struct for each piece
struct Piece {
    enum Colour { RED, BLACK }; // only two colors that pieces can be
    sf::Vector2i position;   // vector of position of checker peice
    Colour colour;
    bool king; // potential promotion to king
};

class CheckersGame
{
public:
    CheckersGame(const CheckersGame&) = delete;
    CheckersGame& operator=(const CheckersGame&) = delete;
    CheckersGame(); // Constructor
    ~CheckersGame(); // deconstructor

    void reset(); // restart the game
    void handleClick(int px, int py); // Checks where mouse is click in pixel coords
    void draw(sf::RenderWindow &win) const; // draws checkers game
    void saveMoves(const std::string &file) const; // log file for all moves during a game

private:    
    // helper functions
    bool pixelToSquare(int px,int py,int &row,int &col) const;
    bool inside(int r,int c) const;
    bool canSelect(int r,int c) const;
    bool canMove(int fr,int fc,int tr,int tc) const;
    void doMove(int fr,int fc,int tr,int tc);

    Piece* board[8][8]; // nullptr = empty
    bool redTurn;
    bool pieceSelected;
    int selRow, selCol;
    std::vector<std::string> moves; // vector of moves done for log file

    bool checkVictory(); // kind of obvious what this does
    bool gameOver = false; // for stopping further clicking
    std::string winner; // whoever won
};

#endif