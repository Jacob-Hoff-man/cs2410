#ifndef H_STAGE
#define H_STAGE

#include "common.h"

#define ZERO_REGISTER_NAME "$0"

class Stage {
    private:
    public:
        StageType stageType;
        void printStageType() {
            cout << "printing StageType = " << stageType << "\n";
        }
        bool dispatch();
};

#endif