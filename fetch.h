#include <deque>
#include <unordered_map>

#ifndef H_FETCH
#define H_FETCH

#include "stage.h"
#include "common.h"

class Fetch: public Stage {
    private:
    public:
        Instruction instr;
        deque<Instruction> & instructions;
        deque<Instruction> & instructionQueue;
        int & nf;
        int & ni;
        int & programCounter;
        unordered_map<int, pair<int, int>> & btb;
        void dispatch();
    Fetch(
        deque<Instruction> & instructions,
        deque<Instruction> & instructionQueue,
        int & nf,
        int & ni,
        int & programCounter,
        unordered_map<int, pair<int, int>> & btb
    );
    ~Fetch();
};

#endif