#ifndef LEVELS_INCLUDED
#define LEVELS_INCLUDED
#include "gameBoard.h"
#include <string>

class Level {
    gameBoard *myGameboard;
    bool blind_on, heavy_on, random_on;
    int myLevel;
    
protected:
    int player; 
    int randSeed;
    std::vector<char> genSequence;
    int genIndex;
    int maxIndex;
    int num_dropUnclear=0;
    Block *typeBlock(char type);
    virtual void makeBlockheavy(Block *theBlock);
    void display_data_text();
    void display_text();
    void display_nextBlock();
    void displayBoard_text();
public:
    Level(gameBoard *myGameboard, bool blind_on, bool heavy_on, bool random_on, int myLevel, int player, int randSeed);

    // void Blockgen(): virtual function
    virtual void Blockgen()=0;
    // doHeavy() drops the block by 2 each move/rotation if event heavy is activated
    // if reaches the bottom/ collide with others before the process completed, it will stop droping
    void doHeavy();

    // moveBlock(int): takes a direction either 1 or -1(means left or right), and move the current block of the board
    // to the direction if valid
    // otherwise the block stays at the same position as before the function was called
    void moveBlock(int direction);

    // virtual function
    virtual void specialAction();

    // setBlind(bool) set the state of event blind to bool <state>
    void setBlind(bool state);

    //setHeavy(bool) set the state of event heavy to bool <state>
    void setHeavy(bool state);

    //setRandom(bool) set the state of event random to bool <state>   
    void setRandom(bool state);

    // getBlind() returns a bool representing the state of event blind
    bool getBlind();

    // getHeavy() returns a bool representing the state of event heavy
    bool getHeavy();

    // getRandom() returns a bool representing the state of event random
    bool getRandom();

    // forceChange(char) takes a block type char T, and change the current block on the board by the given type
    // the block will have the same property as other blocks the level generated
    void forceChange(char type);

    // checkValid() check if the current block of the board is valid, and will lead to gameover if not. otherwise, nothing will happen
    void checkValid();

    // downBlock(): move the current block of the board a row down if possible, 
    // will cause the row check if hit another blocks or the bottom line 
    void downBlock();

    // rotateblock(bool) takes a bool representing if it should rotate clockwise, and rotate the current block of the board as
    // the direction if possible. Otherwise nothing happens
    void rotateBlock(bool clockwise);

    // dropBlock() move the current block of the board until evokes the row check (to the bottom line or hit with other blocks)
    void dropBlock();

    // getLevel() returns an int representing which level it is
    int getLevel();

    // setGenSequence(string) takes a string as the path of the source file
    // read the block type in the file as the member of the level's genSequence
    // will clear the genSequence each time when it is called before reading the sequence
    void setGenSequence(std::string source);

    // incCumulativeDropUnclear() add 1 to the counter for level 4, which will build a "*" obstruction
    // every 5 consecutive block drops without any lines filled.
    void incCumulativeDropUnclear();

    // clearCumulativeDropUnclear() set the counter to 0 for level 4, which will build a "*" obstruction
    // every 5 consecutive block drops without any lines filled.
    void clearCumulativeDropUnclear();

    // getStatus() returns a string representing the state of the event of the level
    // in form "B H R", if any state is not true, its symbol will be replaced by "*"
    std::string getStatus();

    virtual void constructOb();

    // myBoard() returns the ptr to current board
    gameBoard *myBoard();
    virtual ~Level();
};

class Level0 :public Level{
public:
    Level0(gameBoard *myGameboard, bool blind_on, bool heavy_on, bool random_on, int player, int randSeed, std::string seq_file);
    void Blockgen() override;
    ~Level0();
};
class Level1:public Level{
public:
    Level1(gameBoard *myGameboard, bool blind_on, bool heavy_on, bool random_on, int player, int randSeed);
    void Blockgen() override;
    ~Level1();
};

class Level2:public Level{
public:
    Level2(gameBoard *myGameboard, bool blind_on, bool heavy_on, bool random_on, int player, int randSeed);
    void Blockgen() override;
    ~Level2();
};

class Level3:public Level{
public:
    Level3(gameBoard *myGameboard, bool blind_on, bool heavy_on, bool random_on, int player, int randSeed);
    void Blockgen() override;
    // makeBlockheavy(Block *) takes a block ptr thrBlock, set theblock heavy
    void makeBlockheavy(Block *theBlock) override;
    ~Level3();
};

class Level4:public Level{
    const int maxTolerance=5;
public:
    Level4(gameBoard *myGameboard, bool blind_on, bool heavy_on, bool random_on, int player, int randSeed);
    void Blockgen() override;
    void specialAction() override;
    // makeBlockheavy(Block *) takes a block ptr thrBlock, set theblock heavy
    void makeBlockheavy(Block *theBlock) override;
    void constructOb() override;
    ~Level4();
};

#endif
