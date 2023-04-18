#include <iostream>
#include <string>
#include <deque>
#include <unordered_map>
#include <vector>
#include "common.h"
#include "fetch.h"

#ifndef H_SIM
#define H_SIM

#define REGISTER_COUNT 32 // # of physical registers

class Simulator {
    private:
        bool debugMode = false;
        string inpFileName = "";
        int nf, ni, nw, nb, nr;
        unordered_map<int, double> memories;
        unordered_map<string, int> branchLabelsTable;
        int address = 0;
        int cycleCount = 0;
        int programCounter = 0;
        // unordered_map<string, string> mappingTable;
        // deque<string> freeList;
        // int physicalRegs[REGISTER_COUNT];

    public:
        deque<Instruction> instructions;
        deque<Instruction> instructionQueue;
        unordered_map<int, pair<int, int>> btb;
        // stages
        Fetch * f;
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
        void printSimulatorInstructions();
        void printSimulatorBranchLabelsTable() {
            for(const auto& elem : branchLabelsTable) {
                cout << elem.first << " " << elem.second << "\n";
            }
        }
        void printCurrentAddress() {
            cout << "\nAddress = " << address << "\n";
        }
        void printSimulatorCurrentCycleCount() {
            cout << "\ncycleCount = " << cycleCount << "\n";
        }
        void printSimulatorInstructionQueue();
        void printSimulatorBtbMap() {
            for(auto & kv : btb) {
                cout << "\naddress = " << kv.first << 
                "\n   branch translation = {" << kv.second.first << ", " << kv.second.second <<
                "\n";
            }
        }
        void tickCycleCount() {
            cycleCount++;
        }
        void cyclePipeline();
        void execute(int inpCycleCount);
        Simulator(string inpFileName, int nf, int ni, int nw, int nb, int nr, bool debugMode);
        ~Simulator(); 
};

#endif