#ifndef H_DECODE
#define H_DECODE

#include "stage.h"

#define FREE_LIST_EMPTY_CODE " "
#define BTB_ENTRIES_COUNT 16

class Decode: public Stage {
    private:
        string performRegisterRenaming(string inpRegName, bool isDestinationReg);
        deque<Instruction> & fInstructionQueue;
        unordered_map<int, pair<int, BranchPredictionType>> & btb;
        deque<Instruction> & dInstructionQueue;
        unordered_map<string, string> & mappingTable;
        deque<string> & freeList;
        deque<unordered_map<string, string>> & mappingTableHistory;
        deque<deque<string>> & freeListHistory;
        unordered_map<string, int> & branchLabelsTable;
        const int nf;
        const int ni;
    public:
        bool dispatch();
    Decode(
        deque<Instruction> & fInstructionQueue,
        unordered_map<int, pair<int, BranchPredictionType>> & btb,
        deque<Instruction> & dInstructionQueue,
        unordered_map<string, string> & mappingTable,
        deque<string> & freeList,
        deque<unordered_map<string, string>> & mappingTableHistory,
        deque<deque<string>> & freeListHistory,
        unordered_map<string, int> & branchLabelsTable,
        const int nf,
        const int ni
    );
    ~Decode();
};

#endif