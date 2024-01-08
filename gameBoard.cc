#include "gameBoard.h"
#include "blocks.h"
#include "levels.h"
#include <iostream>
#include "levelController.h"

gameBoard::gameBoard(Block *thisBlock, Block *nextBlock, levelController *myLevelController, int player): thisBlock{thisBlock}, nextBlock{nextBlock}, 
            myLevelController{myLevelController}, gameMap{std::vector<char>(11*18,' ')}, score{0}, player{player} 
{}

Level *gameBoard::myLevel() {
    return myLevelController->getLevel();
}
void gameBoard::newBlock(Block *newBlock) {
    delete thisBlock;
    thisBlock=nextBlock;
    nextBlock=newBlock;
}

/*
void gameBoard::setLevel(Level *myLevel) {
    this->myLevel=myLevel;
}
*/

void gameBoard::updateblock() {
    myLevel()->Blockgen();
    myLevel()->incCumulativeDropUnclear();
}

char &gameBoard::charAt(int x, int y) {
    return gameMap.at(x*11+y);
}
// the bottomleft is 0,0
void gameBoard::display() {
    for (int i=0; i<11+2; i++) {
        std::cout<<'-';
    }
    std::cout<<std::endl;
    for (int i=14; i>=0; i--) {
        std::cout<<'|';
        for (int j=0; j<11; j++) {
            if (j>=thisBlock->get_ycord()&&j<thisBlock->get_ycord()+thisBlock->get_m_width()
                &&i>=thisBlock->get_xcord()&&i<thisBlock->get_xcord()+thisBlock->get_m_height()
                &&thisBlock->getChar(i-thisBlock->get_xcord(),j-thisBlock->get_ycord())!=' ') {
                    int block_i=i-thisBlock->get_xcord();
                    int block_j=j-thisBlock->get_ycord();
                    std::cout<<thisBlock->getChar(block_i,block_j);
            } else {
                std::cout<<gameMap.at(i*11+j);
            }
        }
        std::cout<<'|';
        std::cout<<std::endl;
    }
    for (int i=0; i<11+2; i++) {
        std::cout<<'-';
    }
    std::cout<<std::endl;
}
void gameBoard::putChar(int x, int y, char c) {
    if (x>=0&&x<18&&y>=0&&y<11) {
        gameMap.at(x*11+y)=c;
    }
}

void gameBoard::moveBlock(int direction) {
    thisBlock->move(direction);
}

void gameBoard::downBlock(){
    if (!thisBlock->downable()) {
        copyToBoard();
        updateblock();
        checkRows();
        checkValid();
        myLevel()->constructOb();
    } else {
        thisBlock->down();
    }
}

bool gameBoard::downable() {
    return thisBlock->downable();
}

bool gameBoard::isValid() {
    /*
    if (thisBlock->get_ycord()+>)
    for (int i=0; i<thisBlock->get_m_height(); i++) {
        for (int j=0; j<thisBlock->get_m_width(); j++) {
            if (thisBlock->getChar(i,j)!=' '&&charAt(thisBlock->get_xcord()+i,thisBlock->get_ycord()+j)!=' ') {
                return false;
            }
        }
    }
    return true;
    */
   return thisBlock->valid();
}

void gameBoard::rotateblock(bool clockwise) {
    thisBlock->Rotate(clockwise);
}

void gameBoard::dropBlock(){
    while (thisBlock->downable()) {
        thisBlock->down();
    }
    copyToBoard();
    updateblock();
    checkRows();
    checkValid();
    myLevel()->constructOb();
}

void gameBoard::copyToBoard() {
    for (int i=0; i<thisBlock->get_m_height(); i++) {
        for (int j=0; j<thisBlock->get_m_width();j++) {
            if (thisBlock->getChar(i,j)!=' ') {
                charAt(thisBlock->get_xcord()+i,
                       thisBlock->get_ycord()+j)=thisBlock->getChar(i,j);
            }
        }
    }
    AbsBlock theAbsBlock{thisBlock->getFromLevel()};
    for (int i=0; i<thisBlock->get_m_height(); i++) {
        bool line_not_empty=false;
        for (int j=0; j<thisBlock->get_m_width();j++) {
            if (thisBlock->getChar(i,j)!=' ') line_not_empty=true;
        }
        if (line_not_empty) {
            theAbsBlock.addLines(i+thisBlock->get_xcord());
        }
    }
    AbsBlockList.emplace_back(theAbsBlock);
}

void gameBoard::makeCharAbs(int lines) {//only for level 4
    AbsBlock theAbsBlock{4};
    theAbsBlock.addLines(lines);
    AbsBlockList.emplace_back(theAbsBlock);
}

void gameBoard::checkRows() {
    int total_filled_lines=0;
    int height_index=0;
    while (height_index<15) {
        bool line_filled=true;
        for (int j=0; j<11; j++) {
            if (charAt(height_index,j)==' ') line_filled=false;
        }
        if (line_filled) {
            myLevel()->clearCumulativeDropUnclear();
            ++total_filled_lines;
            // cancel lines in AbsBlocklist
            for (auto &n:AbsBlockList) {
                n.deleteLines(height_index);
            }
            // delete empty AbsBlock
            for (std::vector<AbsBlock>::size_type k=0; k<AbsBlockList.size();) {
                if (AbsBlockList.at(k).isEmpty()) {
                    // add score
                    score+=AbsBlockList.at(k).getScore();
                    AbsBlockList.erase(AbsBlockList.begin()+k);
                } else {
                    ++k;
                }  
            }
            // move the graph down
            for (int k=height_index+1; k<15; k++) {
                for (int l=0; l<11; l++) {
                    charAt(k-1,l)=charAt(k,l);
                }
                
            }
            // erase the first line
            for (int l=0; l<11; l++) {
                charAt(14,l)=' ';
            }
            // need to add score
        } else {
            height_index++;
        }
    }
    if (total_filled_lines>0) {// add scores
        lineScoring(total_filled_lines);
        myLevelController->getLevel()->setBlind(false);
    }
    if (total_filled_lines>=2) {// update event
        if (myLevelController->getOverOp()==0) {
            // choose an event
            std::cout<<"You have reached a bonus:"<<std::endl;
            char choice;
            bool valid=false;
            if (!valid) {
                std::cout<<"Choose an event: B: blind H: heavy F: force"<<std::endl;
                std::cin>>choice;
                if (choice=='B') {
                    myLevelController->getLevel(myLevelController->toggleLevel((myLevelController->getLevel()->getLevel())))->setBlind(true);
                    valid=true;
                } else if (choice=='H'){
                    myLevelController->getLevel(myLevelController->toggleLevel((myLevelController->getLevel()->getLevel())))->setHeavy(true);
                    valid=true;
                } else if (choice=='F'){
                    char T;
                    std::cout<<"Choose a block you want your Op to change:"<<std::endl;
                    std::cout<<"I O S Z T L J"<<std::endl;
                    std::cin>>T;
                    myLevelController->forceChange(T);
                    valid=true;
                }
            }
        }
        
    }
}

void gameBoard::forceChange(Block *theBlock) {
    int x=thisBlock->get_xcord();
    int y=thisBlock->get_ycord();
    int fromLevel=thisBlock->getFromLevel();
    delete thisBlock;
    thisBlock=theBlock;
    thisBlock->set_cord(x,y);
    thisBlock->setFromLevel(fromLevel);
    checkValid();
}

void gameBoard::checkValid() {
    if (!isValid()) {
        gameOver();
    }
}

void gameBoard::gameOver() {
    myLevelController->setOver(player);
}

int gameBoard::getScore() {
    return score;
}

void gameBoard::setScore(int new_score) {
    score=new_score;
}

void gameBoard::lineScoring(int lines) {
    score+=(myLevel()->getLevel()+lines)*(myLevel()->getLevel()+lines);
}

//void gameBoard::newBlock() {}
Block *gameBoard::myBlock() {
    return thisBlock;
}

Block *gameBoard::myNextBlock() {
    return nextBlock;
}

gameBoard::~gameBoard() {delete thisBlock; delete nextBlock;}
