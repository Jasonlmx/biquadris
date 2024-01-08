#ifndef GAMEBOARD_INCLUDED
#define GAMEBOARD_INCLUDED
#include "blocks.h" 
class Level;
class levelController;

// gameboard B
class gameBoard {
    Block *thisBlock;
    Block *nextBlock;
    levelController *myLevelController; // changed to levelController ptr
    std::vector<char> gameMap;
    std::vector<AbsBlock> AbsBlockList;
    int score;
    int player;
    void copyToBoard();
    void gameOver();
    void setScore(int new_score);
    void lineScoring(int lines);
public:
    gameBoard(Block *thisBlock, Block *nextBlock, levelController *myLevelController, int player);

    // myLevel() returns the level that now in response for the current board
    Level *myLevel();

    // newBlock(Block *) deallocates the space of thisBlock, move nextBlock to thisBlock and 
    // assigns newBlock to nextBlock
    void newBlock(Block *newBlock);
    //void setLevel(Level *myLevel);

    // updateblock() calls the block generator of the level of the current board
    // calls the level's special obstruction construct function
    void updateblock();

    // &charAt(int, int) takes a pair of coordinates x, y, returns a reference to the char at coordinate x, y
    char &charAt(int x, int y);

    // display(): print the contents of the characters on the board, along with the current block
    // discarded, only for test purpose
    void display();// to be changed to level

    // putChar(int, int, char): takes a pair of coordinates x, y and a char c, if (x,y) is on the board, replace
    // the char at (x,y) with c, otherwise it doesn't do anything
    void putChar(int x, int y, char c);

    // makeCharAbs(int) takes an int line, make a AbsBlock for this char as a block and put it into the AbsblockList
    // only for the special block generated in level 4 when a consecutive of 5 blocks don't fill any lines
    void makeCharAbs(int lines);

    // moveBlock(int): take a int as a direction (-1 or 1: left or right) move thisBlock to the direction
    // usage the same as Block::move(int)
    void moveBlock(int direction);

    // downBlock(): move thisBlock a row down if possible, will cause the row check if hit another blocks or the bottom line 
    void downBlock();

    // downable() returns if thisBlock is valid to move down by 1 row
    bool downable();

    // isValid() returns if thisBlock is on the board, and is not colliding with some other blocks
    bool isValid();

    // rotateblock(bool) takes a bool representing if it should rotate clockwise, and rotate the block as
    // the direction if possible. Otherwise nothing happens
    void rotateblock(bool clockwise=true);

    // dropBlock() move the block down until evokes the row check (to the bottom line or hit with other blocks)
    void dropBlock();

    // checkRows() counts the total row that is fully filled on the map, erase them, and moving rows on the top down by the number of 
    // rows erased. This also add the score to score, and lead to bonus event if the number of such rows exceeds 1
    void checkRows();

    // forceChange(Block *) takes a block ptr theBlock, deallocate the space of thisBlock and assigned thisBlock by theBlock
    // regardless of validity. If invalid, lead to gameover of this player
    void forceChange(Block *theBlock);

    // checkValid() check if thisBlock is valid, and will lead to gameover if not. otherwise, nothing will happen
    void checkValid();

    // getScore() returns a int representing the total score of the player in this round
    int getScore();
    //void newBlock();
    ~gameBoard();

    // myBlock() returns a ptr pointing at thisBlock
    Block *myBlock();

    // myNextBlock() returns a ptr pointing at nextBlock
    Block *myNextBlock();
};
#endif
