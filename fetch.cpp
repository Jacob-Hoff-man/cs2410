#include "fetch.h"

Fetch::Fetch(
    deque<Instruction> & instructions,
    deque<Instruction> & instructionQueue,
    int & nf,
    int & ni,
    int & programCounter,
    unordered_map<int, pair<int, int>> & btb
) :
    instructions(instructions),
    instructionQueue(instructionQueue),
    nf(nf),
    ni(ni),
    programCounter(programCounter),
    btb(btb)
{
    this->stageType = StageType::FETCH;
    this->instructions = instructions;
    this->nf = nf;
    this->ni = ni;
    this->programCounter = programCounter;
    this->instructionQueue = instructionQueue;
    this->btb = btb;
    printStageType();
    cout << "NF in Fetch stage = " << nf;
    cout << "NI in Fetch stage = " << ni;
};

void Fetch::dispatch() {
    cout << "f_dispatch called=";
    for (int i = 0; i < nf; i++) {
        if (instructions.empty() || instructionQueue.size() >= ni) {
            // either no instructions available, or instructionQueue is full
            // stall
            return;
        } else {
            Instruction iInstr = this->instructions.front();
            this->instructions.pop_front();
            instructionQueue.push_back(iInstr);
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

