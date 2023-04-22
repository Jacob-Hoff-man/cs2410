#ifndef H_FETCH
#define H_FETCH

#include "stage.h"

class Fetch: public Stage {
    private:
        deque<Instruction> & instructions;
        deque<Instruction> & fInstructionQueue;
        int & programCounter;
        unordered_map<int, pair<int, BranchPredictionType>> & btb;
        const int nf;
    public:
        bool dispatch();
    Fetch(
        deque<Instruction> & instructions,
        deque<Instruction> & fInstructionQueue,
        int & programCounter,
        unordered_map<int, pair<int, BranchPredictionType>> & btb,
        const int nf
    );
    ~Fetch();
};

#endif