#ifndef INCLUDED_LEVELCONTROLLER
#define INCLUDED_LEVELCONTROLLER
#include <iostream>
#include "levels.h"
#include "gameBoard.h"
#include "window.h"

class levelController {
    std::vector <gameBoard *> myGameboards;
    std::vector <Level *> myLevels;
    std::vector <int> levelNums;
    std::vector <int> isOver;
    int randSeed;
    int controllingLevel;
    const int maxPlayer=2;
    int highScore=0;
    bool isGraphical;
    Xwindow *myWindow=nullptr;
    const int win_x_size=220;
    const int win_y_size=320;
    std::string p1_seq;
    std::string p2_seq;
    int startLevel=0;

    void display_player_graphical();
    void display_player();
    void display_highScore_graphical();
    void display_highScore();
    void display_board_graphical();
    void display_text();
    void display_data_graphical();
    void display_graph();
    void sequencedinterpreter(std::ifstream &ifs);
    void decider(std::string command);
    int colour_bijection(char T);
    Level *newLevel(int n, gameBoard *theBoard, int player);
    Level *typeLevel(int n, bool blind_state, bool heavy_state, bool random_state);
    void changeLevel(bool inc);
    void changeToControl(int target);
    void restart();
    int getControllingLevel();
public:
    levelController(int randSeed, bool isGraphical, std::string p1_seq, std::string p2_seq, int startLevel);
    // commandinterpreter() is the main loop of the game
    // takes in command by std::cin and interpret
    // produce output and interact with users
    void commandinterpreter();

    // toggleLevel(int) takes an int origin and return the index of the other player (if orgin=0, then return 1, else return 0)
    int toggleLevel(int origin);

    // setOver() set the state of the current player as is gameOver
    void setOver();

    // allOver() returns if both players are in state gameOver
    bool allOver();

    // display() produce output representing the score, the blocks and the board to std::cout
    // if isGraphical field is true, it will alse open a graphical window to display the game  
    void display();

    // setOver(int) takes an int level, set the state of player in index level as gameOver
    void setOver(int level);

    // getOver() returns if the current controlling player is in the state gameOver
    int getOver();

    // getOverOp() returns if the opponent of the current controlling player is in the state gameOver
    int getOverOp();

    // forceChange(char) takes a block type char T, and change the current block on the board 
    // of the opponents of the controlling palyer by the given type
    // the block will have the same property as other blocks the level generated
    // without validty. If not valid, the opponent is considered as gameOver
    void forceChange(char T);

    // getLevel() returns the ptr to the controlling player's level
    Level *getLevel();
    
    // getLevel() takes an int level and returns the ptr to the index <level>-th player's level
    Level *getLevel(int level);
    ~levelController();
};

#endif
