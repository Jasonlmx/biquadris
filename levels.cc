#include "levels.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>

Level::Level(gameBoard *myGameboard, bool blind_on, bool heavy_on, bool random_on, int myLevel, int player, int randSeed):
            myGameboard{myGameboard}, blind_on{blind_on}, heavy_on{heavy_on}, random_on{random_on}, myLevel{myLevel}, player{player}, randSeed{randSeed}, genIndex{0}, maxIndex{0}
            {std::srand(randSeed);}

Block *Level::typeBlock(char type) {
    Block *myBlock=nullptr;
    if (type=='I') {
        myBlock=new I_Block{myGameboard, myLevel};
    } else if (type=='J') {
        myBlock=new J_Block{myGameboard, myLevel};
    } else if (type=='L') {
        myBlock=new L_Block{myGameboard, myLevel};
    } else if (type=='O') {
        myBlock=new O_Block{myGameboard, myLevel};
    } else if (type=='S') {
        myBlock=new S_Block{myGameboard, myLevel};
    } else if (type=='Z') {
        myBlock=new Z_Block{myGameboard, myLevel};
    } else if (type=='T') {
        myBlock=new T_Block{myGameboard, myLevel};
    }
    return myBlock;
}

void Level::doHeavy() {
    if (heavy_on) {
        if (myGameboard->downable()) {
            myGameboard->downBlock();
        }
        myGameboard->downBlock();
    }
}

void Level::moveBlock(int direction) {
    myGameboard->moveBlock(direction);
    doHeavy();
    specialAction();
}

void Level::specialAction() {}

void Level::makeBlockheavy(Block *theBlock) {
    //no implementation
}

void Level::setBlind(bool state) {
    blind_on=state;
}

void Level::setHeavy(bool state) {
    heavy_on=state;
}

void Level::setRandom(bool state) {
    random_on=state;
}

bool Level::getBlind() {return blind_on;}

bool Level::getHeavy() {return heavy_on;}

bool Level::getRandom() {return random_on;}

void Level::forceChange(char type) {
    Block *theBlock=typeBlock(type);
    makeBlockheavy(theBlock);
    myGameboard->forceChange(typeBlock(type));
}

void Level::checkValid() {
    myGameboard->checkValid();
}

void Level::downBlock() {
    myGameboard->downBlock();
}

void Level::rotateBlock(bool clockwise) {
    myGameboard->rotateblock(clockwise);
}

void Level::dropBlock() {
    myGameboard->dropBlock();
}

void Level::display_data_text() {
    for (int i=0; i<13; ++i) {std::cout<<'-';}
    std::cout<<std::endl;
    std::cout<<"Level :   "<<myLevel<<std::endl;
    std::cout<<"Score :   "<<myGameboard->getScore()<<std::endl;
}

void Level::display_text() {
    display_data_text();
    displayBoard_text();
    display_nextBlock();
    
}


void Level::displayBoard_text() {
    for (int i=0; i<11+2; i++) {
        std::cout<<'-';
    }
    std::cout<<std::endl;
    for (int i=14; i>=0; i--) {
        std::cout<<'|';
        for (int j=0; j<11; j++) {
            if ((getBlind())&&(i>=2&&i<=11)&&(j>=2&&j<=8)) {
                std::cout<<'?';
            } else {
                if (j>=myGameboard->myBlock()->get_ycord()&&j<myGameboard->myBlock()->get_ycord()+myGameboard->myBlock()->get_m_width()
                &&i>=myGameboard->myBlock()->get_xcord()&&i<myGameboard->myBlock()->get_xcord()+myGameboard->myBlock()->get_m_height()
                &&myGameboard->myBlock()->getChar(i-myGameboard->myBlock()->get_xcord(),j-myGameboard->myBlock()->get_ycord())!=' ') {
                    int block_i=i-myGameboard->myBlock()->get_xcord();
                    int block_j=j-myGameboard->myBlock()->get_ycord();
                    std::cout<<myGameboard->myBlock()->getChar(block_i,block_j);
                } else {
                    std::cout<<myGameboard->charAt(i,j);
                }
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

void Level::display_nextBlock() {
    std::cout<<"Next:"<<std::endl;
    myGameboard->myNextBlock()->display();
}

void Level::constructOb() {}

void Level::incCumulativeDropUnclear() {
    num_dropUnclear++;
}

void Level::clearCumulativeDropUnclear() {
    num_dropUnclear=0;
}

std::string Level::getStatus() {
    std::string status="";
    if (blind_on) {
        status+="B";
    } else {
        status+="*";
    }
    status+=" ";
    if (heavy_on) {
        status+="H";
    } else {
        status+="*";
    }
    status+=" ";
    if (random_on) {
        status+="R";
    } else {
        status+="*";
    }
    return status;
}

gameBoard *Level::myBoard() {
    return myGameboard;
}

int Level::getLevel() {
    return myLevel;
}

void Level::setGenSequence(std::string source) {
    std::ifstream ifs {source};
    genSequence.clear();
    maxIndex=0;
    char type;
    while (ifs>>type) {
        genSequence.emplace_back(type);
        maxIndex++;
    }
}

Level::~Level() {}

Level0::Level0(gameBoard *myGameboard, bool blind_on, bool heavy_on, bool random_on, int player, int randSeed, std::string seq_file)
                :Level{myGameboard, blind_on, heavy_on, random_on, 0, player, randSeed} 
{
    std::string file_name=seq_file;
    std::ifstream ifs {file_name};
    char type;
    while (ifs>>type) {
        genSequence.emplace_back(type);
        maxIndex++;
    }
    if (maxIndex==0) {
        throw std::out_of_range ("The file "+seq_file+" doesn't exist or is empty");
    }
}

void Level0::Blockgen() {
    myBoard()->newBlock(typeBlock(genSequence.at(genIndex%maxIndex)));
    genIndex++;
}

Level0::~Level0() {}

Level1::Level1(gameBoard *myGameboard, bool blind_on, bool heavy_on, bool random_on, int player, int randSeed)
                :Level{myGameboard, blind_on, heavy_on, random_on, 1, player, randSeed} {}

void Level1::Blockgen() {
    int choice=rand()%6;
    Block *genedBlock=nullptr;
    if (choice==0) {
        int secondChoice=rand()%2;
        if (secondChoice==0) {
            genedBlock=typeBlock('S');
        } else {
            genedBlock=typeBlock('Z');
        }
    } else if (choice==1) {
        genedBlock=typeBlock('I');
    } else if (choice==2) {
        genedBlock=typeBlock('O');
    } else if (choice==3) {
        genedBlock=typeBlock('L');
    } else if (choice==4) {
        genedBlock=typeBlock('J');
    } else if (choice==5) {
        genedBlock=typeBlock('T');
    }
    myBoard()->newBlock(genedBlock);
}

Level1::~Level1() {}

Level2::Level2(gameBoard *myGameboard, bool blind_on, bool heavy_on, bool random_on, int player, int randSeed)
                :Level{myGameboard, blind_on, heavy_on, random_on, 2, player, randSeed} {}

void Level2::Blockgen() {
    int choice=rand()%7;
    Block *genedBlock=nullptr;
    if (choice==0) {
        genedBlock=typeBlock('Z');
    } else if (choice==1) {
        genedBlock=typeBlock('I');
    } else if (choice==2) {
        genedBlock=typeBlock('O');
    } else if (choice==3) {
        genedBlock=typeBlock('L');
    } else if (choice==4) {
        genedBlock=typeBlock('J');
    } else if (choice==5) {
        genedBlock=typeBlock('T');
    } else {
        genedBlock=typeBlock('S');
    }
    myBoard()->newBlock(genedBlock);
}

Level2::~Level2() {}

Level3::Level3(gameBoard *myGameboard, bool blind_on, bool heavy_on, bool random_on, int player, int randSeed)
                :Level{myGameboard, blind_on, heavy_on, random_on, 3, player, randSeed} {}

void Level3::Blockgen() {
    int choice=rand()%9;
    Block *genedBlock=nullptr;
    if (getRandom()) {
        if (choice==0||choice==1) {
            genedBlock=typeBlock('Z');
        } else if (choice==2) {
            genedBlock=typeBlock('I');
        } else if (choice==3) {
            genedBlock=typeBlock('O');
        } else if (choice==4) {
            genedBlock=typeBlock('L');
        } else if (choice==5) {
            genedBlock=typeBlock('J');
        } else if (choice==6) {
            genedBlock=typeBlock('T');
        } else {
            genedBlock=typeBlock('S');
        }
    } else {
        genedBlock=typeBlock(genSequence.at(genIndex%maxIndex));
        genIndex++;
    }
    
    genedBlock->setHeavy(true);
    myBoard()->newBlock(genedBlock);
}

void Level3::makeBlockheavy(Block *theBlock) {
    theBlock->setHeavy(true);
}

Level3::~Level3() {}

Level4::Level4(gameBoard *myGameboard, bool blind_on, bool heavy_on, bool random_on, int player, int randSeed)
                :Level{myGameboard, blind_on, heavy_on, random_on, 4, player, randSeed} {}

void Level4::Blockgen() {
    int choice=rand()%9;
    Block *genedBlock=nullptr;
    if (getRandom()) {
        if (choice==0||choice==1) {
            genedBlock=typeBlock('Z');
        } else if (choice==2) {
            genedBlock=typeBlock('I');
        } else if (choice==3) {
            genedBlock=typeBlock('O');
        } else if (choice==4) {
            genedBlock=typeBlock('L');
        } else if (choice==5) {
            genedBlock=typeBlock('J');
        } else if (choice==6) {
            genedBlock=typeBlock('T');
        } else {
            genedBlock=typeBlock('S');
        }
    } else {
        genedBlock=typeBlock(genSequence.at(genIndex%maxIndex));
        genIndex++;
    }
    
    genedBlock->setHeavy(true);
    myBoard()->newBlock(genedBlock);
}

void Level4::specialAction() {

} 

void Level4::constructOb() {
    if (num_dropUnclear>=maxTolerance) {
        clearCumulativeDropUnclear();
        int height=14;
        while (height>0&&myBoard()->charAt(height-1, 5)==' ') {
            height--;
        }
        myBoard()->putChar(height, 5, '*');
        myBoard()->makeCharAbs(height);
        myBoard()->checkRows();
    }
}

void Level4::makeBlockheavy(Block *theBlock) {
    theBlock->setHeavy(true);
}

Level4::~Level4() {}

