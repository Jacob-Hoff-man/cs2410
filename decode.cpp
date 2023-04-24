#include "decode.h"

Decode::Decode(
    deque<Instruction> & fInstructionQueue,
    unordered_map<int, pair<int, int>> & btb,
    deque<Instruction> & dInstructionQueue,
    unordered_map<string, string> & mappingTable,
    deque<string> & freeList,
    deque<unordered_map<string, string>> & mappingTableHistory,
    deque<deque<string>> & freeListHistory,
    unordered_map<string, int> & branchLabelsTable,
    BranchPredictor & dbp,
    const int nf,
    const int ni
) :
    fInstructionQueue(fInstructionQueue),
    btb(btb),
    dInstructionQueue(dInstructionQueue),
    mappingTable(mappingTable),
    freeList(freeList),
    mappingTableHistory(mappingTableHistory),
    freeListHistory(freeListHistory),
    branchLabelsTable(branchLabelsTable),
    dbp(dbp),
    nf(nf),
    ni(ni)
{
    this->stageType = StageType::DECODE;
    printStageType();
    cout << "\nNF in Decode stage = " << nf << "\n"
        << "\nNI in Decode stage = " << ni << "\n";
    cout << "\n";
};

string Decode::performRegisterRenaming(string inpRegName, bool isDestinationReg) {
    string newRegName = inpRegName;
    bool inpContainedOffset = containsOffset(newRegName);
    // if there exists an offset, remove from string
    if (inpContainedOffset) newRegName = trimOffset(newRegName);
    // handle zero register
    if (newRegName == ZERO_REGISTER_NAME) return ZERO_REGISTER_NAME;
    // no available free registers, return unique failure code
    if (freeList.empty()) return FREE_LIST_EMPTY_CODE;
    if (mappingTable.count(newRegName)) {
        // mappingTable contains key for inpRegName
        if (!isDestinationReg) return mappingTable[newRegName];
    }
    // get free reg for new reg, insert into mappingTable
    string freeReg = freeList.front();
    freeList.pop_front();
    mappingTable[newRegName] = freeReg;
    return freeReg;
}

bool Decode::dispatch() {
    cout << "\nd_dispatch called (nf=" << nf << " ni=" << ni << ")=\n";
    // throughput syncronized with fetch stage i.e. ni param
    for (int i = 0; i < nf; i++) {
        if (fInstructionQueue.empty() || dInstructionQueue.size() >= ni) {
            // either no instructions are available from fetch stage,
            // or dInstructionQueue is full. stall!
            if (dInstructionQueue.size() >= ni) return false;
            else return true; // don't count empty fetch queue as a stall
        } else {
            Instruction iInstr = this->fInstructionQueue.front();
            // check if original reg name contains an offset 
            int immediateOffset = 0;
            bool offsetExists = containsOffset(iInstr.rs);
            if (offsetExists) immediateOffset = getOffset(iInstr.rs);
            string regName;
            switch(iInstr.opcode) {
                // if ever regName == free list empty code,
                // there are no available physical regs. stall!
                case InstructionType::FLD:
                    // rs
                    regName = performRegisterRenaming(iInstr.rs, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rs = regName;
                    // rd
                    regName = performRegisterRenaming(iInstr.rd, true);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rd = regName;

                    break;
                case InstructionType::FSD: 
                    // rt
                    regName = performRegisterRenaming(iInstr.rt, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rt = regName;
                    // rs
                    regName = performRegisterRenaming(iInstr.rs, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rs = regName;

                    break;
                case InstructionType::ADD: // rd, rs, rt
                    // rt
                    regName = performRegisterRenaming(iInstr.rt, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rt = regName;
                    // rs
                    regName = performRegisterRenaming(iInstr.rs, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rs = regName;
                    // rd
                    regName = performRegisterRenaming(iInstr.rd, true);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rd = regName;

                    break;
                case InstructionType::ADDI: // rs, rd
                    // rs
                    regName = performRegisterRenaming(iInstr.rs, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rs = regName;
                    // rd
                    regName = performRegisterRenaming(iInstr.rd, true);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rd = regName;

                    break;
                case InstructionType::SLT: // rd, rs, rt
                    // rt
                    regName = performRegisterRenaming(iInstr.rt, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rt = regName;
                    // rs
                    regName = performRegisterRenaming(iInstr.rs, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rs = regName;
                    // rd
                    regName = performRegisterRenaming(iInstr.rd, true);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rd = regName;

                    break;
                case InstructionType::FADD: // rd, rs, rt
                    // rt
                    regName = performRegisterRenaming(iInstr.rt, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rt = regName;
                    // rs
                    regName = performRegisterRenaming(iInstr.rs, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rs = regName;
                    // rd
                    regName = performRegisterRenaming(iInstr.rd, true);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rd = regName;

                    break;
                case InstructionType::FSUB: // rd, rs, rt
                    // rt
                    regName = performRegisterRenaming(iInstr.rt, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rt = regName;
                    // rs
                    regName = performRegisterRenaming(iInstr.rs, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rs = regName;
                    // rd
                    regName = performRegisterRenaming(iInstr.rd, true);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rd = regName;

                    break;
                case InstructionType::FMUL: // rd, rs, rt
                    // rt
                    regName = performRegisterRenaming(iInstr.rt, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rt = regName;
                    // rs
                    regName = performRegisterRenaming(iInstr.rs, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rs = regName;
                    // rd
                    regName = performRegisterRenaming(iInstr.rd, true);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rd = regName;

                    break;
                case InstructionType::FDIV: // rd, rs, rt
                    // rt
                    regName = performRegisterRenaming(iInstr.rt, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rt = regName;
                    // rs
                    regName = performRegisterRenaming(iInstr.rs, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rs = regName;
                    // rd
                    regName = performRegisterRenaming(iInstr.rd, true);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rd = regName;

                    break;
                case InstructionType::BNE:
                    // rs
                    regName = performRegisterRenaming(iInstr.rs, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rs = regName;
                    // rd
                    regName = performRegisterRenaming(iInstr.rd, false);
                    if (regName == FREE_LIST_EMPTY_CODE) return false;
                    iInstr.rd = regName;

                    break;
                default:
                    // instruction type not recognized
                    return false;
            }
            // if original reg name contained offset, add into immediate
            if (offsetExists) iInstr.immediate += immediateOffset; 
            // add current instruction with renamed reg to dInstructionQueue
            fInstructionQueue.pop_front();
            dInstructionQueue.push_back(iInstr);
            // btb insertion
            if (iInstr.opcode == InstructionType::BNE) {
                // store a historic copy of the current mapping table and free list
                mappingTableHistory.push_back(mappingTable);
                freeListHistory.push_back(freeList);
                if (BTB_ENTRIES_COUNT)
                if (btb.count(iInstr.address) == 0) {
                    // current instruction is not contained in btb
                    btb[iInstr.address] = {
                        branchLabelsTable[iInstr.rt],
                        dbp.getBranchPrediction()
                    };
                }
            }
        }
    }
    return true;
}