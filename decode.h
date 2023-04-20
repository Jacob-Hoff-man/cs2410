#include <deque>
#include <unordered_map>

#ifndef H_DECODE
#define H_DECODE

#include "stage.h"
#include "common.h"

#define FREE_LIST_EMPTY_CODE " "
#define ZERO_REGISTER_NAME "$0"

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
        int & nf;
        int & ni;
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
        int & nf,
        int & ni
    );
    ~Decode();
};

#endif