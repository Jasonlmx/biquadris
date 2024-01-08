#include "blocks.h"
#include "gameBoard.h"
#include <iostream>


Block::Block(int m_height, int m_width, gameBoard* myGameboard, int fromLevel):
    m_height{m_height}, m_width{m_width}, x_cord{14}, y_cord{0}, myGameboard{myGameboard}, fromLevel{fromLevel} {};

Block::~Block() {}
void Block::setChar(int x, int y, char c) {
    myMap.at(m_width*x+y)=c;
}

int Block::get_m_height() {return m_height;}
int Block::get_m_width() {return m_width;}
int Block::get_xcord() {return x_cord;}
int Block::get_ycord() {return y_cord;}

char Block::getChar(int x, int y) {// std cord
    return myMap.at((m_width*(m_height-x-1)+y));
}
void Block::down() {
    if (downable()){
        x_cord--;
    }
}   

bool Block::downable() {
    //bool result=true;
    if (x_cord==0) {
        return false;
    }
    for (int i=0; i<m_height; i++) {
        for (int j=0; j<m_width; j++) {
            if (getChar(i,j)!=' '&&
                myGameboard->charAt(x_cord+i-1,y_cord+j)!=' ') {
                return false;
            }
        }
    }
    return true;
}

bool Block::valid() {
    if (y_cord<0||y_cord>=11) {
        return false;
        
    }
    if (x_cord<0||x_cord>14) {
        return false;
        
    }
    for (int i=0; i<m_height; i++) {
        for (int j=0; j<m_width; j++) {
            if (getChar(i,j)!=' '&&
                (y_cord+j>=11||myGameboard->charAt(x_cord+i,y_cord+j)!=' ')) {
                return false;
            }
        }
    }
    return true;
}

void Block::set_cord(int x, int y) {
    x_cord=x;
    y_cord=y;
}

bool Block::valid(std::vector<char> &map) {
    if (y_cord<0||y_cord>=11) {
        return false;
    }
    if (x_cord<0||x_cord>14) {
        return false;
    }
    for (int i=0; i<m_height; i++) {
        for (int j=0; j<m_width; j++) {
            if (map.at(m_width*(m_height-i-1)+j)!=' '&&
                (y_cord+j>=11||myGameboard->charAt(x_cord+i,y_cord+j)!=' ')) {
                return false;
            }
        }
    }
    return true;
}

void Block::display() {
    for (int i=0; i<m_height; ++i) {
        for (int j=0; j<m_width; ++j) {
            std::cout<<myMap.at(i*m_width+j)<<" ";
        }
        std::cout<<std::endl;
    }
}

std::string Block::getLine(int num_line) {
    std::string line{""};
    if (num_line<m_height) {
        for (int j=0; j<m_width; ++j) {
            line+=myMap.at(num_line*m_width+j);
            line+=' ';
        }
    }
    //std::cout<<line<<std::endl;
    return line;
}

void Block::Rotate(bool clockwise=true){
    std::vector <char> newMap(m_width*m_height,' ');
    for (int i=0; i<m_height; i++) {
            for (int j=0; j<m_height; j++) {
                if (clockwise) newMap.at(j*m_width+m_width-i-1)=myMap.at(i*m_width+j);
                else newMap.at((m_height-j-1)*m_width+i)=myMap.at(i*m_width+j);
            }
    }    
    // count empty lines before the first row where non_empty char appears
    int index=m_height-1;
    int num_empty_row=0;
    bool is_empty_row=true;
    while  (is_empty_row&&index>=0){
        for (int j=0; j<m_width; j++) {
            if (newMap.at(index*m_width+j)!=' ') is_empty_row=false;
        }
        if (is_empty_row) num_empty_row++;
        index--;
    }
    // move the graph down
    for (int i=m_height-num_empty_row-1; i>=0; i--) {
        for (int j=0; j<m_width; j++) {
            newMap.at((i+num_empty_row)*m_width+j)=newMap.at(i*m_width+j);
            //newMap.at(i*m_width+j)=' ';
        }
    }
    // erase the top-most
    for (int i=0; i<num_empty_row; i++) {
        for (int j=0; j<m_width; j++) {
            newMap.at(i*m_width+j)=' ';
            //newMap.at(i*m_width+j)=' ';
        }
    }
    // count empty columns before the first columns where non_empty char appears
    index=0;
    int num_empty_col=0;
    bool is_empty_col=true;
    while  (is_empty_col&&index<m_width){
        for (int j=0; j<m_height; j++) {
            if (newMap.at(j*m_width+index)!=' ') is_empty_col=false;
        }
        if (is_empty_col) num_empty_col++;
        index++;
    }

    // move the graph left
    for (int i=0; i<m_height; i++) {
        for (int j=num_empty_col; j<m_width; j++) {
            newMap.at(i*m_width+j-num_empty_col)=newMap.at(i*m_width+j);
            //newMap.at(i*m_width+j)=' ';
        }
    }
    
    // erase the right-most
    for (int i=0; i<m_height; i++) {
        for (int j=m_width-num_empty_col; j<m_width; j++) {
            newMap.at(i*m_width+j)=' ';
            //newMap.at(i*m_width+j)=' ';
        }
    }


    // remember to check validity here
    if (valid(newMap)){
        for (int i=0; i<m_width*m_height; ++i) {
            myMap.at(i)=newMap.at(i);
        }
    }
    if (is_Heavy) {
        down();
    }
    
}

bool Block::movable(int direction) {
    y_cord+=direction;
    bool result=valid();
    y_cord-=direction;
    return result;
}

void Block::move(int direction) {
    if (movable(direction)) {
        y_cord+=direction;
    }
    if (is_Heavy) {
        down();
    }
}

void Block::setFromLevel(int level) {
    fromLevel=level;
}

int Block::getFromLevel() {
    return fromLevel;
}



void Block::locate(int x, int y) {
    x_cord=x;
    y_cord=y;
}

void Block::setHeavy(bool state) {
    is_Heavy=state;
}

I_Block::I_Block(gameBoard* myGameboard, int fromLevel): Block{4,4,myGameboard, fromLevel} {
    myMap.reserve(16);
    myMap={' ',' ',' ',' ',
           ' ',' ',' ',' ',
           ' ',' ',' ',' ',
           'I','I','I','I'};
}
I_Block::~I_Block() {}

J_Block::J_Block(gameBoard* myGameboard, int fromLevel): Block(3,3,myGameboard, fromLevel) {
    myMap.reserve(9);
    myMap={' ',' ',' ',
           'J',' ',' ',
           'J','J','J',};
}
J_Block::~J_Block() {}

L_Block::L_Block(gameBoard* myGameboard, int fromLevel): Block(3,3,myGameboard, fromLevel) {
    myMap.reserve(9);
    myMap={' ',' ',' ',
           ' ',' ','L',
           'L','L','L',};
}
L_Block::~L_Block() {}

O_Block::O_Block(gameBoard* myGameboard, int fromLevel): Block(3,3,myGameboard, fromLevel) {
    myMap.reserve(9);
    myMap={' ',' ',' ',
           'O','O',' ',
           'O','O',' ',};
}
O_Block::~O_Block() {}

S_Block::S_Block(gameBoard* myGameboard, int fromLevel): Block(3,3,myGameboard, fromLevel) {
    myMap.reserve(9);
    myMap={' ',' ',' ',
           ' ','S','S',
           'S','S',' ',};
}
S_Block::~S_Block() {}

Z_Block::Z_Block(gameBoard* myGameboard, int fromLevel): Block(3,3,myGameboard, fromLevel) {
    myMap.reserve(9);
    myMap={' ',' ',' ',
           'Z','Z',' ',
           ' ','Z','Z',};
}
Z_Block::~Z_Block() {}

T_Block::T_Block(gameBoard* myGameboard, int fromLevel): Block(3,3,myGameboard,fromLevel) {
    myMap.reserve(9);
    myMap={' ',' ',' ',
           'T','T','T',
           ' ','T',' ',};
}
T_Block::~T_Block() {}

AbsBlock::AbsBlock(int level_generated): level_generated{level_generated}, lines{0} {}

void AbsBlock::addLines(int line) {
    
    bool non_repeat=true;
    for (auto n:lines_occupied) {
        if (n==line) {
            non_repeat=false;
        }
    }
    if (non_repeat) {
        lines_occupied.emplace_back(line);
        lines++;
    }
}

bool AbsBlock::isEmpty() {
    return lines==0;
}

void AbsBlock::deleteLines(int line) {
    for (int i=0; i<lines;) {
        if (lines_occupied.at(i)==line) {
            lines_occupied.erase(lines_occupied.begin()+i);
            lines--;
        } else {
            if (lines_occupied.at(i)>line) {
                lines_occupied.at(i)--;
            }
            ++i;
        }
    }
}

int AbsBlock::getScore() {
    if (lines==0) {
        return (level_generated+1)*(level_generated+1);
    }
    return 0;
}

void AbsBlock::printContents(){
    for (int i=0; i<lines; i++) {
        std::cout<<lines_occupied.at(i)<<" ";
    }
    std::cout<<std::endl;
}

AbsBlock::~AbsBlock() {}
