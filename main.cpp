/*
* Author: Chris Hussain
*
* Resources: SFML.org GeeksforGeeks.com CPlusPlus.com
*/

#include <SFML/Graphics.hpp>
#include "CheckersGame.h"
using namespace std;

// Constants for board size
const int TILE = 80;
const int PAD  = 20;
const int BOARD_PIXELS = 8*TILE + 2*PAD;


int main()
{
    sf::RenderWindow window(sf::VideoMode(BOARD_PIXELS,BOARD_PIXELS),"Checkers"); // create a window
        
    window.setFramerateLimit(60); // game crashes if frame limit is too low or too fast rather
    CheckersGame game; // launch game

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
            if (e.type == sf::Event::MouseButtonPressed)
                game.handleClick(e.mouseButton.x, e.mouseButton.y); // to click on the checker pieces
        }

        window.clear();
        game.draw(window); // draws checkers game
        window.display(); // displays checkers game
    }
    game.saveMoves("moves.log"); // writes move history
    return 0;
}
