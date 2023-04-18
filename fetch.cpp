#include "fetch.h"

Fetch::Fetch(
    deque<Instruction> & instructions,
    deque<Instruction> & fInstructionQueue,
    int & programCounter,
    unordered_map<int, pair<int, int>> & btb,
    int & nf
) :
    instructions(instructions),
    fInstructionQueue(fInstructionQueue),
    programCounter(programCounter),
    btb(btb),
    nf(nf)
{
    this->stageType = StageType::FETCH;
    this->instructions = instructions;
    this->programCounter = programCounter;
    this->fInstructionQueue = fInstructionQueue;
    this->btb = btb;
    this->nf = nf;
    printStageType();
    cout << "NF in Fetch stage = " << nf;
};

void Fetch::dispatch() {
    cout << "\nf_dispatch called=\n";
    for (int i = 0; i < nf; i++) {
        if (instructions.empty() || fInstructionQueue.size() >= nf) {
            // either no instructions available, or fInstructionQueue is full
            // stall
            return;
        } else {
            Instruction iInstr = this->instructions.front();
            this->instructions.pop_front();
            fInstructionQueue.push_back(iInstr);
            if ((iInstr.opcode == InstructionType::BNE) && (btb.count(iInstr.address))) {
                if (btb.find(iInstr.address) != btb.end()) {
                    // address exists in btb
                    programCounter = btb[iInstr.address].first;
                } else {
                    programCounter++;
                }
            } else {
                programCounter++;
            }
        }
    }
    return;
}

