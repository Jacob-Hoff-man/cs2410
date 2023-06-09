#ifndef H_DECODE
#define H_DECODE

#include "stage.h"
#include "branchPredictor.h"

#define FREE_LIST_EMPTY_CODE " "
#define BTB_ENTRIES_COUNT 16

class Decode: public Stage {
    private:
        string performRegisterRenaming(string inpRegName, bool isDestinationReg);
        deque<Instruction> & fInstructionQueue;
        unordered_map<int, pair<int, int>> & btb;
        deque<Instruction> & dInstructionQueue;
        unordered_map<string, string> & mappingTable;
        deque<string> & freeList;
        deque<unordered_map<string, string>> & mappingTableHistory;
        deque<deque<string>> & freeListHistory;
        unordered_map<string, int> & branchLabelsTable;
        BranchPredictor * dbp;
        const int nf;
        const int ni;
        const bool debugMode;
    public:
        bool dispatch();
    Decode(
        deque<Instruction> & fInstructionQueue,
        unordered_map<int, pair<int, int>> & btb,
        deque<Instruction> & dInstructionQueue,
        unordered_map<string, string> & mappingTable,
        deque<string> & freeList,
        deque<unordered_map<string, string>> & mappingTableHistory,
        deque<deque<string>> & freeListHistory,
        unordered_map<string, int> & branchLabelsTable,
        BranchPredictor * dbp,
        const int nf,
        const int ni,
        const bool debugMode
    );
    ~Decode();
};

#endif