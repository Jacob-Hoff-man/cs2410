#include "decode.h"

Decode::Decode(
    deque<Instruction> & fInstructionQueue,
    unordered_map<int, pair<int, int>> & btb,
    deque<Instruction> & dInstructionQueue,
    int & nf,
    int & ni
) :
    fInstructionQueue(fInstructionQueue),
    btb(btb),
    dInstructionQueue(dInstructionQueue),
    nf(nf),
    ni(ni)
{
    this->stageType = StageType::DECODE;
    this->fInstructionQueue = fInstructionQueue;
    this->btb = btb;
    this->dInstructionQueue = dInstructionQueue;
    this->nf = nf;
    this->ni = ni;
    printStageType();
    cout << "nf in Decode stage = " << nf << "\n"
        << "ni in Decode stage = " << ni << "\n";
};

void Decode::dispatch() {
    cout << "\nd_dispatch called=\n";
    
}