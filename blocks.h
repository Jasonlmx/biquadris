#ifndef BLOCKS_INCLUDED
#define BLOCKS_INCLUDED
#include <vector>
#include <string>

class gameBoard;
// this is gameboard A

class Block {// the cordinate determined by the bottom-leftmost block of myMap
protected:    
    int m_height, m_width, x_cord, y_cord;
    std::vector<char> myMap;
    gameBoard* myGameboard;
    bool is_Heavy=false;
    int fromLevel;
public:
    Block(int m_height, int m_width, gameBoard* myGameboard, int fromLevel);

    // get_m_height(): returns the maxium height of the char map of this block as a int
    int get_m_height();

    // get_m_width(): returns the maxium width of the char map of this block as a int
    int get_m_width();

    // get_xcord(): returns the current xcordinate on gameboard of this block as a int
    int get_xcord();

    // get_ycord(): returns the current ycordinate on gameboard of this block as a int
    int get_ycord();

    // getLine(int) take a int represent the  line, returns a string containing the char of num_linet-th line of the char map
    std::string getLine(int num_line);

    // Rotate(bool): take a boolean clockwise, rotate the block 1. clockwise=true clockwise 2. clockwise=false counterclockwise
    // if valid in the gameboard. Otherwise the map won't change
    void Rotate(bool clockwise);

    // setChar(int, int, char): takes the coordinates x, y, the char c, and 
    // set the char in the map with position(x,y) by c
    void setChar(int x, int y, char c);

    // getChar(int, int) takes the coordinates x, y, returns the char at position (x,y) on the char map
    char getChar(int x, int y);

    // down(): moves the block down a line. If it collide with another block or the frame, it won't move and will stay 
    // in the same position as before
    void down();

    // downable(): returns a bool represent if it is valid for the block to move down a line
    bool downable();

    // set_cord(int, int): takes the coordinates x and y, and move the block to position (x,y) ignoring the validity
    void set_cord(int x, int y);
    // valid() returns if the current block stays on the board, and without colliding to other blocks on the board.
    bool valid();

    // valid(std::vector <char>): takes a char_map, and assume the char map is in the position of the block, and return if
    // it is valid
    bool valid(std::vector<char> &map);

    // movable(int): takes a direction either 1 or -1(means left or right), and return if it is valid to move to that direction by 
    // 1 column
    bool movable(int direction); // direction: -1-left 1-right

    // move(int): takes a direction either 1 or -1(means left or right), and move the block to the direction if valid
    // otherwise the block stays at the same position before the function was called
    void move(int direction);

    // setFromLevel(int) takes an int level, and set the value of fromLevel to level
    void setFromLevel(int level);

    // getFromLevel() returns the value of FromLevel
    int getFromLevel();

    // locate(int, int) acts the same as set_cord(int, int)
    void locate(int x, int y); 

    // setHeavy(bool) takes a boolean state and set the value of heavy_on to state
    void setHeavy(bool state);
    
    //test
    void display();

    virtual ~Block()=0;
};


class I_Block: public Block {
public:
    I_Block(gameBoard* myGameBoard, int fromLevel);
    ~I_Block();
};

class J_Block: public Block {
public:
    J_Block(gameBoard* myGameBoard, int fromLevel);
    ~J_Block();
};

class L_Block: public Block {
public:
    L_Block(gameBoard* myGameBoard, int fromLevel);
    ~L_Block();
};

class O_Block: public Block {
public:
    O_Block(gameBoard* myGameBoard, int fromLevel);
    ~O_Block();
};

class S_Block: public Block {
public:
    S_Block(gameBoard* myGameBoard, int fromLevel);
    ~S_Block();
};

class Z_Block: public Block {
public:
    Z_Block(gameBoard* myGameBoard, int fromLevel);
    ~Z_Block();
};

class T_Block: public Block {
public:
    T_Block(gameBoard* myGameBoard, int fromLevel);
    ~T_Block();
};

class AbsBlock {
    std::vector <int> lines_occupied;
    int level_generated;
    int lines;
public:
    AbsBlock(int level_generated);

    // addLines(int) append a line into lines_occupied, and increase lines by one
    void addLines(int line);

    // isEmpty() returns a boolean, represent if the lines_occupied is empty
    bool isEmpty();

    // getScore() returns a int score=(level_generate+1)^2 if is empty, otherwise return 0
    int getScore();

    // deleteLines(int) erase the line with value <line> in lines_occupied(if exist), and decrease all
    // other values in lines_occupied that is larger than <line> by 1
    void deleteLines(int line);

    // printContents() prints the values in lines_occupied, each value is in a seperated line
    // for test only
    void printContents();//for test
    
    ~AbsBlock();
};
#endif


