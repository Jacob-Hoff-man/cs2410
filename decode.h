#include <deque>
#include <unordered_map>

#ifndef H_DECODE
#define H_DECODE

#include "stage.h"
#include "common.h"

class Decode: public Stage {
    private:
    public:
        deque<Instruction> & fInstructionQueue;
        unordered_map<int, pair<int, int>> & btb;
        deque<Instruction> & dInstructionQueue;
        int & nf;
        int & ni;
        void dispatch();
    Decode(
        deque<Instruction> & fInstructionQueue,
        unordered_map<int, pair<int, int>> & btb,
        deque<Instruction> & dInstructionQueue,
        int & nf,
        int & ni
    );
    ~Decode();
};

#endif