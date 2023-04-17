#include <deque>

#ifndef H_FETCH
#define H_FETCH

#include "stage.h"
#include "common.h"

class Fetch: public Stage {
    private:
    public:
        Instruction instr;
        deque<Instruction> * instructions;
        bool dispatch();
    Fetch(deque<Instruction> * instructions);
    ~Fetch();
};

#endif