#include "fetch.h"

Fetch::Fetch(deque<Instruction> * instructions) {
    stageType = StageType::FETCH;
    this->instructions = instructions;
    printStageType();
};

bool Fetch::dispatch() {
    this->instr = this->instructions->front();
    this->instructions->pop_front();
    return true;
}

