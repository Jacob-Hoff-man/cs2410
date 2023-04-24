#include "fetch.h"

Fetch::Fetch(
    deque<Instruction> & instructions,
    deque<Instruction> & fInstructionQueue,
    int & programCounter,
    unordered_map<int, pair<int, BranchPredictionType>> & btb,
    const int nf
) :
    instructions(instructions),
    fInstructionQueue(fInstructionQueue),
    programCounter(programCounter),
    btb(btb),
    nf(nf)
{
    this->stageType = StageType::FETCH;
    printStageType();
    cout << "\nNF in Fetch stage = " << nf;
    cout << "\n";
};

bool Fetch::dispatch() {
    cout << "\nf_dispatch called (nf=" << nf << ")=\n";
    for (int i = 0; i < nf; i++) {
        if (instructions.empty() || fInstructionQueue.size() >= nf || programCounter > instructions.size()-1) {
            // either no instructions available, or fInstructionQueue is full, or programCounter points beyond max addr
            // stall!
            return true; // don't count as a stall
        } else {
            Instruction iInstr = this->instructions[programCounter];
            fInstructionQueue.push_back(iInstr);
            if ((iInstr.opcode == InstructionType::BNE) && (btb.count(iInstr.address))) {
                if (btb.count(iInstr.address)) {
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
    return true;
}

