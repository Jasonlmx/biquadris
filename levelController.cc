#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "levelController.h"

levelController::levelController(int randSeed, bool isGraphical, std::string p1_seq, std::string p2_seq, int startLevel): randSeed{randSeed}, controllingLevel{0}, isGraphical{isGraphical},
    p1_seq{p1_seq}, p2_seq{p2_seq}, startLevel{startLevel}
{
    //myGameboard->setLevel(myLevel);
    for (int i=0; i<maxPlayer; i++) {
        gameBoard *theBoard=new gameBoard{nullptr, nullptr, this, i};
        myGameboards.emplace_back(theBoard);
        myLevels.emplace_back(newLevel(startLevel, theBoard, i));
        levelNums.emplace_back(startLevel);
        isOver.emplace_back(0);
        myLevels.at(i)->Blockgen();
        myLevels.at(i)->Blockgen();
    }
    if (isGraphical) {
        myWindow=new Xwindow(win_x_size,win_y_size);
    }
    
};

void levelController::changeToControl(int target) {
    if (isOver.at(target)==0) {
        controllingLevel=target;
    }
}

int levelController::toggleLevel(int origin) {
    if (controllingLevel==0) {
        return 1;
    }
    return 0;
}

void levelController::display_player_graphical() {
    int p1_x=10;
    int p1_y=70;
    int p2_x=120;
    int p2_y=p1_y;
    if (controllingLevel==0) {
        if (isOver.at(0)==1) {
            myWindow->drawString(p1_x, p1_y, "%P1 Gameover!");
        } else {
            myWindow->drawString(p1_x, p1_y, "*P1's turn!");
        }
        if (isOver.at(1)==1) {
            myWindow->drawString(p2_x, p2_y, "%P2 Gameover!");
        }
    } else {
        if (isOver.at(0)==1) {
            myWindow->drawString(p1_x, p1_y, "%P1 Gameover");
        } 
        if (isOver.at(1)==1) {
            myWindow->drawString(p2_x, p2_y, "%P2 Gameover!");
        } else {
            myWindow->drawString(p2_x, p2_y, "*P2's turn");
        }
    }
}

void levelController::display_player() {
    if (controllingLevel==0) {
        if (isOver.at(0)==1) {
            std::cout<<std::left<<std::setw(13)<<"%P1 Gameover";
        } else {
            std::cout<<std::left<<std::setw(13)<<"*P1's turn";
        }
        if (isOver.at(1)==1) {
            std::cout<<"   "<<std::left<<std::setw(13)<<"%P2 Gameover"<<std::endl;
        } else {
            std::cout<<std::endl;
        }
    } else {
        if (isOver.at(0)==1) {
            std::cout<<std::left<<std::setw(16)<<"%P1 Gameover";
        } else {
            std::cout<<"                ";
        }
        if (isOver.at(1)==1) {
            std::cout<<"%P2 Gameover"<<std::endl;
        } else {
            std::cout<<"*P2's turn"<<std::endl;
        }
    }
}

void levelController::display_highScore_graphical() {
    int hi_score_x=20;
    int hi_score_y=50;
    myWindow->drawString(hi_score_x, hi_score_y, "***  Highest Score: ");
    myWindow->drawString(hi_score_x+120, hi_score_y, std::to_string(highScore));
    myWindow->drawString(hi_score_x+160, hi_score_y, "***");
}

void levelController::display_highScore() {
    int maxPlayerScore=std::max(myGameboards.at(0)->getScore(), myGameboards.at(1)->getScore());
    highScore=std::max(maxPlayerScore, highScore);
    std::cout<<"*** Highest Score: "<<std::right<<std::setw(5)<<highScore<<" ***"<<std::endl;
}

void levelController::display_data_graphical() {
    int p1_x=10;
    int p2_x=120;
    int start_y=80;
    int interval=10;
    std::string split="---------------";
    myWindow->drawString(p1_x, start_y, split);
    myWindow->drawString(p2_x, start_y, split);
    myWindow->drawString(p1_x, start_y+1*interval, "Level :   "+std::to_string(myLevels.at(0)->getLevel()));
    myWindow->drawString(p2_x, start_y+1*interval, "Level :   "+std::to_string(myLevels.at(1)->getLevel()));
    myWindow->drawString(p1_x, start_y+2*interval, "Score :   "+std::to_string(myGameboards.at(0)->getScore()));
    myWindow->drawString(p2_x, start_y+2*interval, "Score :   "+std::to_string(myGameboards.at(1)->getScore()));
    myWindow->drawString(p1_x, start_y+3*interval, split);
    myWindow->drawString(p2_x, start_y+3*interval, split);
    myWindow->drawString(p1_x, start_y+4*interval, "Event :   "+myLevels.at(0)->getStatus());
    myWindow->drawString(p2_x, start_y+4*interval, "Event :   "+myLevels.at(1)->getStatus());
    myWindow->drawString(p1_x, start_y+5*interval, split);
    myWindow->drawString(p2_x, start_y+5*interval, split);
}

int levelController::colour_bijection(char T) {
    int theColour=Xwindow::White;
    if (T=='I') {
        theColour=Xwindow::Blue;
    } else if (T=='J') {
        theColour=Xwindow::Green;
    } else if (T=='L') {
        theColour=Xwindow::Cyan;
    } else if (T=='O') {
        theColour=Xwindow::Yellow;
    } else if (T=='S') {
        theColour=Xwindow::Brown;
    } else if (T=='Z') {
        theColour=Xwindow::Magenta;
    } else if (T=='T') {
        theColour=Xwindow::Orange;
    } else if (T=='?') {
        theColour=Xwindow::Black;
    } else if (T=='*') {
        theColour=Xwindow::Red;
    }
    return theColour;
}

void levelController::display_board_graphical() {
    int p1_x=10;
    int p2_x=120;
    int pixel_size=8;
    int start_y=130;
    std::string split="+-------------+";
    for (int i=14; i>=0; i--) {
        for (int k=0; k<maxPlayer; k++) {
            int start_x=0;
            if (k==0) {
                start_x=p1_x;
            } else {
                start_x=p2_x;
            }
            gameBoard *myGameboard=myGameboards.at(k);;
            Level *myLevel=myLevels.at(k);
            for (int j=0; j<11; j++) {
                if ((myLevel->getBlind())&&(i>=2&&i<=11)&&(j>=2&&j<=8)) {
                    myWindow->fillRectangle(start_x+j*pixel_size, start_y+(14-i)*pixel_size, pixel_size, pixel_size, colour_bijection('?'));
                } else {
                    if (j>=myGameboard->myBlock()->get_ycord()&&j<myGameboard->myBlock()->get_ycord()+myGameboard->myBlock()->get_m_width()
                    &&i>=myGameboard->myBlock()->get_xcord()&&i<myGameboard->myBlock()->get_xcord()+myGameboard->myBlock()->get_m_height()
                    &&myGameboard->myBlock()->getChar(i-myGameboard->myBlock()->get_xcord(),j-myGameboard->myBlock()->get_ycord())!=' ') {
                        int block_i=i-myGameboard->myBlock()->get_xcord();
                        int block_j=j-myGameboard->myBlock()->get_ycord();
                        myWindow->fillRectangle(start_x+j*pixel_size, start_y+(14-i)*pixel_size, pixel_size, pixel_size, colour_bijection(myGameboard->myBlock()->getChar(block_i,block_j)));
                    } else  {
                        if (myGameboard->charAt(i,j)!=' ') {
                            myWindow->fillRectangle(start_x+j*pixel_size, start_y+(14-i)*pixel_size, pixel_size, pixel_size, colour_bijection(myGameboard->charAt(i,j)));
                        }
                    }
                }
            }
        }
    }
    myWindow->drawString(p1_x, start_y+16*pixel_size, split);
    myWindow->drawString(p2_x, start_y+16*pixel_size, split);
    myWindow->drawString(p1_x, start_y+16*pixel_size+10, "Next :");
    myWindow->drawString(p2_x, start_y+16*pixel_size+10, "Next :");
    int max_line_num=std::max(myGameboards.at(0)->myNextBlock()->get_m_height(),myGameboards.at(1)->myNextBlock()->get_m_height());
    for (int i=0; i<max_line_num; i++) {
        for (int k=0; k<maxPlayer; k++) {
            int start_x=0;
            if (k==0) {
                start_x=p1_x;
            } else {
                start_x=p2_x;
            }
            for (int j=0; j<max_line_num; j++) {
                Block *theBlock=myGameboards.at(k)->myNextBlock();
                if (theBlock->get_m_height()>i&&theBlock->get_m_width()>j) {
                    char myChar=theBlock->getChar(i,j);
                    if (myChar!=' '){
                        myWindow->fillRectangle(start_x+j*pixel_size, start_y+16*pixel_size+10+(max_line_num-i)*pixel_size, pixel_size, pixel_size, colour_bijection(myChar));
                    }
                }
        }
        }
    }
}

void levelController::display_text() {
    display_highScore();
    display_player();
    std::cout<<"-------------"<<"   "<<"-------------"<<std::endl;
    std::cout<<"Level :"<<std::right<<std::setw(6)<<myLevels.at(0)->getLevel()<<"   "<<"Level :"<<std::setw(6)<<myLevels.at(1)->getLevel()<<std::endl;
    std::cout<<"Score :"<<std::right<<std::setw(6)<<myGameboards.at(0)->getScore()<<"   "<<"Score :"<<std::setw(6)<<myGameboards.at(1)->getScore()<<std::endl;
    std::cout<<"-------------"<<"   "<<"-------------"<<std::endl;
    std::cout<<"Event :"<<std::right<<std::setw(6)<<myLevels.at(0)->getStatus()<<"   "<<"Event :"<<std::right<<std::setw(6)<<myLevels.at(1)->getStatus()<<std::endl;
    std::cout<<"+-----------+"<<"   "<<"+-----------+"<<std::endl;
    for (int i=14; i>=0; i--) {
        for (int k=0; k<maxPlayer; k++) {
            gameBoard *myGameboard=myGameboards.at(k);;
            Level *myLevel=myLevels.at(k);
            std::cout<<'|';
            for (int j=0; j<11; j++) {
                if ((myLevel->getBlind())&&(i>=2&&i<=11)&&(j>=2&&j<=8)) {
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
            std::cout<<"|"; 
            if (k==0) {
                std::cout<<"   "; 
            }
        }
        std::cout<<std::endl;
    }
    std::cout<<"+-----------+"<<"   "<<"+-----------+"<<std::endl;
    std::cout<<"Next :       "<<"   "<<"Next :       "<<std::endl;
    int max_line_num=std::max(myGameboards.at(0)->myNextBlock()->get_m_height(),myGameboards.at(1)->myNextBlock()->get_m_height());
    for (int i=0; i<max_line_num; i++) {
        std::cout<<std::left<<std::setw(13)<<myGameboards.at(0)->myNextBlock()->getLine(i)<<"   "<<std::left<<std::setw(13)<<myGameboards.at(1)->myNextBlock()->getLine(i)<<std::endl;
    }
}

void levelController::display_graph() {
    myWindow->fillRectangle(0,0, win_x_size, win_y_size, Xwindow::White);
    myWindow->drawString(30, 30, "Biquadris Graphical Display");
    display_highScore_graphical();
    display_player_graphical();
    display_data_graphical();
    display_board_graphical();
}

void levelController::display() {
    display_text();
    if (isGraphical) {
        display_graph();
    }
}

void levelController::setOver() {
    isOver.at(controllingLevel)=1;
}

void levelController::setOver(int level) {
    isOver.at(level)=1;
}

bool levelController::allOver() {
    for (int i=0; i<maxPlayer; i++) {
        if (isOver.at(i)==0) {
            return false;
        }
    }
    return true;
}

int levelController::getOver() {return isOver.at(controllingLevel);}

int levelController::getOverOp() {return isOver.at(controllingLevel==0?1:0);}

int levelController::getControllingLevel() {return controllingLevel;}

void levelController::decider(std::string command) {
    if (!allOver()) {
            if (command=="left"){
                getLevel()->moveBlock(-1);
                changeToControl(toggleLevel(controllingLevel));
                display();
            } else if (command=="right") {
                getLevel()->moveBlock(1);
                changeToControl(toggleLevel(controllingLevel));
                display();
            } else if (command=="down") {
                getLevel()->downBlock();
                changeToControl(toggleLevel(controllingLevel));
                display();
            } else if (command=="clockwise") {
                getLevel()->rotateBlock(true);
                changeToControl(toggleLevel(controllingLevel));
                display();
            } else if (command=="counterclockwise") {
                getLevel()->rotateBlock(false);
                changeToControl(toggleLevel(controllingLevel));
                display();
            } else if (command=="drop") {
                getLevel()->dropBlock();
                changeToControl(toggleLevel(controllingLevel));
                display();
            } else if (command=="levelup") {
                changeLevel(true);
                display();
            } else if (command=="leveldown") {
                changeLevel(false);
                display();
            } else if (command=="norandom") {
                std::string file_name;
                std::cin>>file_name;
                if (getLevel()->getLevel()>=3) {
                    getLevel()->setGenSequence(file_name);
                    getLevel()->setRandom(false);
                } else {
                    std::cout<<"norandom only avaliable in level 3 or 4"<<std::endl;
                }
                display();
            } else if (command=="random") {
                getLevel()->setRandom(true);
            } else if (command=="sequence") {
                std::string file_name;
                std::cin>>file_name;
                std::ifstream ifs{file_name};
                sequencedinterpreter(ifs);
            } else if (command=="restart") {
                restart();
                display();
            } else if (command=="toggle") {
                changeToControl(toggleLevel(controllingLevel));
                display();
            } else if (command=="force") {
                char T;
                std::cin>>T;
                if (isOver.at(toggleLevel(controllingLevel))==0) {
                    myLevels.at(toggleLevel(controllingLevel))->forceChange(T);
                }
                display();
            } else if (command=="I"||command=="J"||command=="Z"||command=="L"||command=="T"||command=="S"||command=="O") {
                myLevels.at(controllingLevel)->forceChange(command[0]);
                myLevels.at(controllingLevel)->checkValid();
                if (isOver.at(controllingLevel)==1) {
                    changeToControl(toggleLevel(controllingLevel));
                }
                display();
            } else {
                std::cout<<command<<" is not a interpretable command!"<<std::endl;
            }
        } else {
            if (command=="restart") {
                restart();
                display();
            }
        }
}

void levelController::sequencedinterpreter(std::ifstream &ifs) {
    std::string command;
    while (ifs>>command) {
        decider(command); 
    }
}

void levelController::commandinterpreter() {
    //getLevel()->display_text();
    display();
    while (!std::cin.eof()) {
        if (!allOver()) {
            std::cout<<"Enter a command: "<<std::endl;
            std::string command;
            std::cin>>command;
            decider(command);
        } else {
            std::cout<<"Gameover! Please enter restart to play it again:"<<std::endl;
            std::string command;
            std::cin>>command;
            decider(command);
        }
        
    }
}

void levelController::restart() {
    for (int i=0; i<maxPlayer; i++) {
        delete myGameboards.at(i);
        delete myLevels.at(i);
    }
    myGameboards.clear();
    myLevels.clear();
    levelNums.clear();
    isOver.clear();
    for (int i=0; i<maxPlayer; i++) {
        gameBoard *theBoard=new gameBoard{nullptr, nullptr, this, i};
        myGameboards.emplace_back(theBoard);
        myLevels.emplace_back(newLevel(startLevel, theBoard, i));
        levelNums.emplace_back(startLevel);
        isOver.emplace_back(0);
        myLevels.at(i)->Blockgen();
        myLevels.at(i)->Blockgen();
    }
    controllingLevel=0;
}

void levelController::forceChange(char T) {
    myLevels.at(toggleLevel(controllingLevel))->forceChange(T);
}

Level *levelController::getLevel() {
    return myLevels.at(controllingLevel);
}

Level *levelController::getLevel(int level) {
    return myLevels.at(level);
}

Level *levelController::newLevel(int n, gameBoard *theBoard, int player) {
    Level *levelType=nullptr;
    if (n==0) {
        levelType=new Level0{theBoard, 0, 0, 1, player, randSeed, player==0?p1_seq:p2_seq};
    } else if (n==1) {
        levelType=new Level1{theBoard, 0, 0, 1, player, randSeed};
    } else if (n==2) {
        levelType=new Level2{theBoard, 0, 0, 1, player, randSeed};
    } else if (n==3) {
        levelType=new Level3{theBoard, 0, 0, 1, player, randSeed};
    } else if (n==4) {
        levelType=new Level4{theBoard, 0, 0, 1, player, randSeed};
    } else {
        throw std::out_of_range{"level must between 0 and 4!"};
    }
    return levelType;
}

Level *levelController::typeLevel(int n, bool blind_state, bool heavy_state, bool random_state) {
    Level *levelType=nullptr;
    if (n==0) {
        levelType=new Level0{myGameboards.at(controllingLevel), blind_state, heavy_state, random_state, controllingLevel, randSeed, controllingLevel==0?p1_seq:p2_seq};
    } else if (n==1) {
        levelType=new Level1{myGameboards.at(controllingLevel), blind_state, heavy_state, random_state, controllingLevel, randSeed};
    } else if (n==2) {
        levelType=new Level2{myGameboards.at(controllingLevel), blind_state, heavy_state, random_state, controllingLevel, randSeed};
    } else if (n==3) {
        levelType=new Level3{myGameboards.at(controllingLevel), blind_state, heavy_state, random_state, controllingLevel, randSeed};
    } else if (n==4) {
        levelType=new Level4{myGameboards.at(controllingLevel), blind_state, heavy_state, random_state, controllingLevel, randSeed};
    } else {
        throw std::out_of_range{"level must between 0 and 4!"};
    }
    return levelType;
}

void levelController::changeLevel(bool inc) {
    bool changed=false;
    if (levelNums.at(controllingLevel)-1>=0&&!inc){
        levelNums.at(controllingLevel)--;
        changed=true;
    } else if (levelNums.at(controllingLevel)+1<=4&&inc) {
        levelNums.at(controllingLevel)++;
        changed=true;
    }
    std::cout<<changed<<std::endl;
    if (changed) {
        Level *newLevel=typeLevel(levelNums.at(controllingLevel), getLevel()->getBlind(), getLevel()->getHeavy(), 1);
        //myGameboard->setLevel(newLevel);
        delete getLevel();
        myLevels.at(controllingLevel)=newLevel;
    }
}

levelController::~levelController() {
    for (int i=0; i<maxPlayer; i++) {
        delete myGameboards.at(i);
        delete myLevels.at(i);
    }
    delete myWindow;
}
