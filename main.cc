#include "blocks.h"
#include "gameBoard.h"
#include "levels.h"
#include "levelController.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

int main(int argc, char *argv[]) {
    int index=1;
    bool isGraphical=true;
    int seed=time(0);
    int startLevel=0;
    std::string p1_seq="biquadris_sequence1.txt";
    std::string p2_seq="biquadris_sequence2.txt";
    while (index<argc) {
        std::string arg=argv[index];
        if (arg=="-text") {
            isGraphical=false;
        } else if (arg=="-seed") {
            index++;
            if (index>=argc) {
                std::cout<<"syntax error, must be -seed n"<<std::endl;
                return 0;
            }
            std::string n_arg=argv[index];
            std::istringstream iss{n_arg};
            if (iss>>seed) {
            } else {
                std::cout<<"syntax error, must be -seed n"<<std::endl;
                return 0;
            }
        } else if (arg=="-scriptfile1") {
            index++;
            if (index>=argc) {
                std::cout<<"syntax error, must be -scriptfile1 xxx"<<std::endl;
                return 0;
            }
            std::string n_arg=argv[index];
            p1_seq=n_arg;
        } else if (arg=="-scriptfile2") {
            index++;
                if (index>=argc) {
                    std::cout<<"syntax error, must be -scriptfile2 xxx"<<std::endl;
                    return 0;
                }
                std::string n_arg=argv[index];
                p2_seq=n_arg;
        } else if (arg=="-startlevel") {
            index++;
            if (index>=argc) {
                std::cout<<"syntax error, must be -startlevel n"<<std::endl;
                return 0;
            }
            std::string n_arg=argv[index];
            std::istringstream iss{n_arg};
            if (iss>>startLevel) {
            } else {
                std::cout<<"syntax error, must be -startlevel n"<<std::endl;
                return 0;
            }
        } else {
            std::cout<<"unrecognizable argument """<<arg<<""""<<std::endl;
        }
        index++;
    }
    levelController myLevelController{time(0), isGraphical, p1_seq, p2_seq, startLevel};
    myLevelController.commandinterpreter();
    
}
