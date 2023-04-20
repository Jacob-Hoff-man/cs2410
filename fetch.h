#include <deque>
#include <unordered_map>

#ifndef H_FETCH
#define H_FETCH

#include "stage.h"
#include "common.h"

class Fetch: public Stage {
    private:
        deque<Instruction> & instructions;
        deque<Instruction> & fInstructionQueue;
        int & programCounter;
        unordered_map<int, pair<int, int>> & btb;
        int & nf;
    public:
        bool dispatch();
    Fetch(
        deque<Instruction> & instructions,
        deque<Instruction> & fInstructionQueue,
        int & programCounter,
        unordered_map<int, pair<int, int>> & btb,
        int & nf
    );
    ~Fetch();
};

#endif