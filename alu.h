#ifndef H_ALU
#define H_ALU

#include "module.h"

class Alu: public Module {
    private:
        unordered_map<string, double> & cdb;
        unordered_map<string, double> & physicalRegs;
        unordered_map<int, double> & memories;
        const bool debugMode;
    public:
        double performOperation(
            ROBStatus inpEntry,
            RSStatus inpRs
        );
    Alu(
        unordered_map<string, double> & cdb,
        unordered_map<string, double> & physicalRegs,
        unordered_map<int, double> & memories,
        const bool debugMode
    );
    ~Alu() {};
};

#endif