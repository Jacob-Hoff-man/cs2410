#ifndef H_STAGE
#define H_STAGE

#include "common.h"

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