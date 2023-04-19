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

bool Fetch::dispatch() {
    cout << "\nf_dispatch called=\n";
    for (int i = 0; i < nf; i++) {
        if (instructions.empty() || fInstructionQueue.size() >= nf || programCounter > instructions.size()-1) {
            // either no instructions available, or fInstructionQueue is full, or programCounter points beyond max addr
            // stall
            return false;
        } else {
            Instruction iInstr = this->instructions[programCounter];
            fInstructionQueue.push_back(iInstr);
            if ((iInstr.opcode == InstructionType::BNE) && (btb.count(iInstr.address))) {
                if (btb.find(iInstr.address) != btb.end()) {
                    // address exists in btb
                    programCounter = btb[iInstr.address].first;
                    cout << "ADDRESS EXISTS IN BTB" << "\n";
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

