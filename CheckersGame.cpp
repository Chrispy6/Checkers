/* 
* Author: Chris Hussain
*
* Resources: SFML.org GeeksforGeeks.com CPlusPlus.com
* 
* P.S. the king only moves one space diagonally in any direction for my program.
*/

#include "CheckersGame.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib> 

using namespace std;    

// constants for drawing board
const int TILE = 80;
const int PAD  = 20;
const sf::Color LIGHT(238,238,210);
const sf::Color DARK (118,150, 86);
const sf::Color REDC (211, 47, 47);
const sf::Color BLKC ( 0, 0, 0);
const sf::Color SELC ( 33,150,243,160);

CheckersGame::CheckersGame()
{
    // makes sure that everything is ready for a new game to start
    for (int r=0;r<8;++r)
        for (int c=0;c<8;++c)
            board[r][c] = nullptr;
    reset();
}

CheckersGame::~CheckersGame()
{
    reset();  // deletes remaining pieces
}

//allocate one new Piece and put it on the board array
void addPiece(Piece* b[8][8],int r,int c,Piece::Colour col)
{
    Piece* p  = new Piece;
    p->position = sf::Vector2i(r,c);
    p->colour   = col;
    p->king     = false;
    b[r][c]     = p;
}

//  Reset the entire game by clearing memory, place starting pieces, and reset move history
void CheckersGame::reset()
{
    //deletes what might still be on the board
    for (int r=0;r<8;++r)
        for (int c=0;c<8;++c) {
            delete board[r][c];
            board[r][c] = nullptr;
        }
    
    // adds black pieces
    for (int r=0;r<3;++r)
        for (int c=0;c<8;++c)
            if ((r+c)%2) addPiece(board,r,c,Piece::BLACK);
    
    // adds red pieces
    for (int r=5;r<8;++r)
        for (int c=0;c<8;++c)
            if ((r+c)%2) addPiece(board,r,c,Piece::RED);

    redTurn = true;
    pieceSelected = false;
    moves.clear();
}

//  helper functions

//checks board bounds
bool CheckersGame::inside(int r,int c) const
{ return r>=0 && r<8 && c>=0 && c<8; }

// converts pixel position to tile position
bool CheckersGame::pixelToSquare(int px,int py,int &row,int &col) const
{
    col = (px - PAD) / TILE;
    row = (py - PAD) / TILE;
    return inside(row,col);
}

//checks if player can select on this row and column
bool CheckersGame::canSelect(int r,int c) const
{
    if (!board[r][c]) return false;
    return ( redTurn && board[r][c]->colour==Piece::RED) || (!redTurn && board[r][c]->colour==Piece::BLACK );
}

//checks if the checker piece is allowed to move
bool CheckersGame::canMove(int fr, int fc, int tr, int tc) const
{
    if (!inside(tr,tc) || board[tr][tc]) return false;  // must land on empty    
    Piece* p = board[fr][fc];
    int dir = (p->colour==Piece::RED) ? -1 : 1;  //forward direction
    int dr = tr - fr,  dc = tc - fc; // changes in row and column 

    // checker pieces can only go forward
    if (abs(dr)==1 && abs(dc)==1)
        return p->king ? true : (dr==dir);

    // jump and capturing
    if (abs(dr)==2 && abs(dc)==2)
    {
        int mr = fr + dr/2, mc = fc + dc/2;
        if (!board[mr][mc] || board[mr][mc]->colour==p->colour) return false;
        if (!p->king && dr!=2*dir) return false; // checker must jump forward
        return true;
    }
    return false;
}

// checks if game is over
bool CheckersGame::checkVictory()
{
    int redCnt = 0, blkCnt = 0;

    // counts remaining pieces if 0 then other team wins
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (board[r][c])
                (board[r][c]->colour == Piece::RED ? redCnt : blkCnt)++;

    // if someone has zero pieces its game over
    if (redCnt == 0 || blkCnt == 0)
    {
        gameOver = true;
        winner   = (redCnt == 0) ? "Black" : "Red";

        // log who won to log file
        moves.push_back("== " + winner + " wins ==");
        return true;
    }
    return false;
}

//executes a move
void CheckersGame::doMove(int fr,int fc,int tr,int tc)
{
    Piece* p = board[fr][fc];
    board[fr][fc]= nullptr;
    p->position  = sf::Vector2i(tr,tc);
    board[tr][tc]= p;

    // capture?
    if (abs(tr-fr)==2) {
        int mr=(fr+tr)/2, mc=(fc+tc)/2;
        delete board[mr][mc];
        board[mr][mc]=nullptr;
    }

    // if a chekcer piece reaches the other side then it becomes a king
    if ((p->colour==Piece::RED   && tr==0) || (p->colour==Piece::BLACK && tr==7)) p->king=true;

    // Logs moves to log file
    stringstream ss;
    ss<<(redTurn?"Red":"Black")<<": ("<<fr<<","<<fc<<")->("<<tr<<","<<tc<<")";
    moves.push_back(ss.str());
}

void CheckersGame::handleClick(int px,int py)
{
    if (gameOver) return; // exit if game over
    int r,c;
    if (!pixelToSquare(px,py,r,c)) return;

    if (!pieceSelected) { // if nothing is selected then you can attempt to select a piece
        if (canSelect(r,c)) { pieceSelected=true; selRow=r; selCol=c; }
    } else { // if piece is already selected the it deselects that piece so you can select a different one if you choose
        if (r==selRow && c==selCol) { pieceSelected=false; return; }
        if (canMove(selRow,selCol,r,c)) {
            doMove(selRow,selCol,r,c);
            if (checkVictory()) return;   // checks for victory
            pieceSelected=false;
            redTurn=!redTurn; // switches turns
        }
    }
}

// drawing/displaying
void CheckersGame::draw(sf::RenderWindow &win) const
{
    sf::RectangleShape sq(sf::Vector2f((float)TILE,(float)TILE));

    // draws chekcerboard and creates highlight when you select a piece
    for (int r=0;r<8;++r)
        for (int c=0;c<8;++c) {
            sq.setPosition(PAD + c*TILE, PAD + r*TILE);
            sq.setFillColor( ((r+c)%2) ? DARK : LIGHT );
            win.draw(sq);
            if (pieceSelected && r==selRow && c==selCol) {
                sq.setFillColor(SELC); win.draw(sq);
            }
        }

    // pieces are just simple circles
    sf::CircleShape disk(TILE*0.4f);
    disk.setOrigin(disk.getRadius(),disk.getRadius());

    for (int r=0;r<8;++r)
        for (int c=0;c<8;++c) if (board[r][c]) {
            Piece* p = board[r][c];
            disk.setPosition(PAD + c*TILE + TILE/2.f,
                             PAD + r*TILE + TILE/2.f);
            disk.setFillColor(p->colour==Piece::RED ? REDC : BLKC);
            win.draw(disk);
            if (p->king) {  // yellow ring for kings
                disk.setOutlineThickness(4);
                disk.setOutlineColor(sf::Color::Yellow);
                win.draw(disk);
                disk.setOutlineThickness(0);
            }
        }
    if (gameOver)
    {
        //displays winner if game over
        sf::Font font;
        if (!font.getInfo().family.size())
            font.loadFromFile("arial.ttf");

        sf::Text txt(winner + " wins!", font, 36);
        txt.setFillColor(sf::Color::Black);
        txt.setStyle(sf::Text::Bold);
        // center to window
        sf::FloatRect bounds = txt.getLocalBounds();
        txt.setOrigin(bounds.width/2, bounds.height/2);
        txt.setPosition(PAD + 4*TILE, PAD + 4*TILE);
        win.draw(txt);
    }
    
}

// generates log file for viewing 
void CheckersGame::saveMoves(const string &file) const
{
    ofstream ofs(file.c_str());
    for (size_t i = 0; i < moves.size(); ++i)
        ofs << moves[i] << '\n';
}