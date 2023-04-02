#include <iostream>
#include <string>
#include <deque>
#include <unordered_map>
#include <vector>
#include "fetch.h"

using namespace std;

#ifndef H_SIM
#define H_SIM

#define REGISTER_COUNT 32 // # of physical registers

enum InstructionType {
    FLD,
    FSD,
    ADD,
    ADDI,
    SLT,
    FADD,
    FSUB,
    FMUL,
    FDIV,
    BNE
};

struct Instruction {
    int address;
    InstructionType opcode;
    string rd;
    string rs;
    string rt;
    int immediate;
};

class Simulator {
    private:
        bool debugMode = false;
        string inpFileName = "";
        int nf, ni, nw, nb, nr;
        unordered_map<int, double> memories;
        deque<Instruction> instructions;
        unordered_map<string, int> branchLabelsTable;
        int address = 0;
        int cycleCount = 0;
        int programCounter = 0;
        unordered_map<int, pair<int, int>> btb;
        // unordered_map<string, string> mappingTable;
        // deque<string> freeList;
        // int physicalRegs[REGISTER_COUNT];

    public:
        // stages
        Fetch * fetch;
        void tokenizeMemory(char * inpStr);
        void tokenizeInstruction(char * inpStr);
        bool readInputFile(const char * inpFile);
        int getMemoriesSize() {
            return memories.size();
        }
        int getInstructionsSize() {
            return instructions.size();
        }
        void printMemories() {
            for(const auto& elem : memories) {
                cout << elem.first << " " << elem.second << "\n";
            }
        }
        void printInstruction(Instruction inpInstr);
        void printInstructions();
        void printBranchLabelsTable() {
            for(const auto& elem : branchLabelsTable) {
                cout << elem.first << " " << elem.second << "\n";
            }
        }
        void printCurrentAddress() {
            cout << "\nAddress = " << address << "\n";
        }
        void printCurrentCycleCount() {
            cout << "\ncycleCount = " << cycleCount << "\n";
        }
        void tickCycleCount() {
            cycleCount++;
        }
        Simulator(string inpFileName, int nf, int ni, int nw, int nb, int nr);
        Simulator(string inpFileName, int nf, int ni, int nw, int nb, int nr, bool debugMode);
        ~Simulator(); 
};

#endif