#include <iostream>
#include <string>
#include <deque>
#include <unordered_map>
#include <vector>
#include "common.h"
#include "fetch.h"
#include "decode.h"

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
        deque<Instruction> instructions;
        deque<Instruction> fInstructionQueue;
        unordered_map<int, pair<int, int>> btb;
        deque<Instruction> dInstructionQueue;
        unordered_map<string, string> mappingTable;
        deque<string> freeList;
        unordered_map<string, double> physicalRegs;
        deque<unordered_map<string, string>> mappingTableHistory;
        deque<deque<string>> freeListHistory;
        void initSimulatorPhysicalRegs();
    public:
        // stages
        Fetch * f;
        Decode * d;
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
            cout << "MEMORIES CACHE\n";
            for(const auto& elem : memories) {
                cout << elem.first << " " << elem.second << "\n";
            }
        }
        void printSimulatorInstructions();
        void printSimulatorBranchLabelsTable() {
            cout << "BRANCH LABELS TABLE\n";
            for (const auto& elem : branchLabelsTable) {
                cout << elem.first << " " << elem.second << "\n";
            }
        }
        void printCurrentAddress() {
            cout << "\nAddress = " << address << "\n";
        }
        void printSimulatorCurrentCycleCount() {
            cout << "\ncycleCount = " << cycleCount << "\n";
        }
        void printSimulatorFetchInstructionQueue();
        void printSimulatorDecodeInstructionQueue();
        void printSimulatorBtbMap() {
            cout << "BTB\n";
            for (auto & kv : btb) {
                cout << "\naddress = " << kv.first << 
                "\n   branch translation = {" << kv.second.first << ", " << kv.second.second <<
                "}\n";
            }
        }
        void printSimulatorMappingTable() {
            cout << "MAPPING TABLE\n";
            for (const auto& elem : mappingTable) {
                cout << elem.first << " " << elem.second << "\n";
            }
        }
        void printSimulatorFreeList(){ 
            cout << "FREE LIST\n";
            for (const auto& elem : freeList) {
                cout << "\n" << elem << "\n";
            }
        }
        void printSimulatorMappingTableHistory() {
            cout << "MAPPING TABLE HISTORY\n";
            int i = 0;
            for (const auto& mt : mappingTableHistory) {
                cout << "MAPPING TABLE index " << i++ << "\n";
                for (const auto& elem : mt) {
                    cout << elem.first << " " << elem.second << "\n";
                }
            }
        }
        void printSimulatorFreeListHistory() {
            cout << "FREE LIST HISTORY\n";
            int i = 0;
            for (const auto& fl : freeListHistory) {
                cout << "FREE LIST index " << i++ << "\n";
                for (const auto& elem : fl) {
                    cout << "\n" << elem << "\n";
                }
            }
        }
        void tickCycleCount() {
            cycleCount++;
        }
        void cyclePipeline();
        void execute();
        void execute(int inpCycleCount);
        Simulator(string inpFileName, int nf, int ni, int nw, int nb, int nr, bool debugMode);
        ~Simulator(); 
};

#endif