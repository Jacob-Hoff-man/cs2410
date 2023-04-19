#include <string>
#include <iostream>
using namespace std;

#ifndef H_STAGE
#define H_STAGE

enum StageType {
    FETCH,
    DECODE,
    EXECUTE,
    MEMORY,
    WRITE_BACK,
    COMMIT
};

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