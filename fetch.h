#ifndef H_FETCH
#define H_FETCH

#include "stage.h"

class Fetch: public Stage {
    private:
        deque<Instruction> & instructions;
        deque<Instruction> & fInstructionQueue;
        int & programCounter;
        unordered_map<int, pair<int, int>> & btb;
        const int nf;
        const bool debugMode;
    public:
        bool dispatch();
    Fetch(
        deque<Instruction> & instructions,
        deque<Instruction> & fInstructionQueue,
        int & programCounter,
        unordered_map<int, pair<int, int>> & btb,
        const int nf,
        const bool debugMode
    );
    ~Fetch();
};

#endif