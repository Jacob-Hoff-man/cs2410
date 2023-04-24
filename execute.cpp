#include "execute.h"

Execute::Execute(
    vector<RSStatus> & rsUnitInt,
    vector<RSStatus> & rsUnitLoad,
    vector<RSStatus> & rsUnitStore,
    vector<RSStatus> & rsUnitFpAdd,
    vector<RSStatus> & rsUnitFpMult,
    vector<RSStatus> & rsUnitFpDiv,
    vector<RSStatus> & rsUnitBu,
    deque<ROBStatus> & rob,
    unordered_map<string, double> & cdb,
    unordered_map<string, double> & physicalRegs,
    unordered_map<int, double> & memories,
    deque<string> & freeList,
    BranchPredictor & dbp,
    int & programCounter,
    unordered_map<int, pair<int, int>> & btb,
    unordered_map<string, string> & mappingTable,
    deque<deque<string>> & freeListHistory,
    deque<unordered_map<string, string>> & mappingTableHistory,
    deque<Instruction> & fInstructionQueue,
    deque<Instruction> & dInstructionQueue,
    const int nw,
    const int nr,
    const int nb
) :
    rsUnitInt(rsUnitInt),
    rsUnitLoad(rsUnitLoad),
    rsUnitStore(rsUnitStore),
    rsUnitFpAdd(rsUnitFpAdd),
    rsUnitFpMult(rsUnitFpMult),
    rsUnitFpDiv(rsUnitFpDiv),
    rsUnitBu(rsUnitBu),
    rob(rob),
    cdb(cdb),
    physicalRegs(physicalRegs),
    memories(memories),
    freeList(freeList),
    dbp(dbp),
    programCounter(programCounter),
    btb(btb),
    mappingTable(mappingTable),
    freeListHistory(freeListHistory),
    mappingTableHistory(mappingTableHistory),
    fInstructionQueue(fInstructionQueue),
    dInstructionQueue(dInstructionQueue),
    nw(nw),
    nr(nr),
    nb(nb)
{
    this->stageType = StageType::EXECUTE;
    printStageType();
    cout << "\nNW in Execute stage = " << nw <<
        "\nNR in Execute stage = " << nr <<
        "\nNB in Execute stage = " << nb <<
        "\n";
};

RSStatus Execute::getReservationStationFromInstruction(
    Instruction inpInstruction
) {
    InstructionType instrType = inpInstruction.opcode;
    switch(instrType) {
        case InstructionType::FLD:
            for(auto rs : rsUnitLoad) {
                if (rs.instruction == inpInstruction) return rs;
            }
        case InstructionType::FSD:
            for(auto rs : rsUnitStore) {
                if (rs.instruction == inpInstruction) return rs;
            }
        case InstructionType::ADD:
            for(auto rs : rsUnitInt) {
                if (rs.instruction == inpInstruction) return rs;
            }
        case InstructionType::ADDI:
            for(auto rs : this->rsUnitInt) {
                if (rs.instruction == inpInstruction) return rs;
            }
        case InstructionType::SLT:
            for(auto rs : this->rsUnitInt) {
                if (rs.instruction == inpInstruction) return rs;
            }
        case InstructionType::FADD:
            for(auto rs : this->rsUnitFpAdd) {
                if (rs.instruction == inpInstruction) return rs;
            }
        case InstructionType::FSUB:
            for(auto rs : this->rsUnitFpAdd) {
                if (rs.instruction == inpInstruction) return rs;
            }
        case InstructionType::FMUL:
            for(auto rs : this->rsUnitFpMult) {
                if (rs.instruction == inpInstruction) return rs;
            }
        case InstructionType::FDIV:
            for(auto rs : this->rsUnitFpDiv) {
                if (rs.instruction == inpInstruction) return rs;
            }
        case InstructionType::BNE:
            for(auto rs : this->rsUnitBu) {
                if (rs.instruction == inpInstruction) return rs;
            }
        default:
            throw invalid_argument(
                "the provided InstructionType was not recognized. Check to ensure that input parameters are not null"
            );
    }
}

int Execute::getReservationStationIndex(
    vector<RSStatus> inpRsUnit,
    RSStatus inpReservationStation
) {
    for (int i = 0; i < inpRsUnit.size(); i++) {
        if (inpRsUnit.at(i).instruction == inpReservationStation.instruction)
            return i;
    }
    return -1;
}

vector<RSStatus> & Execute::getReservationStationUnitFromInstructionType(InstructionType inpInstrType) {
    switch (inpInstrType) {
        case InstructionType::FLD: return rsUnitLoad;
        case InstructionType::FSD: return rsUnitStore;
        case InstructionType::ADD: return rsUnitInt;
        case InstructionType::ADDI: return rsUnitInt;
        case InstructionType::SLT: return rsUnitInt;
        case InstructionType::FADD: return rsUnitFpAdd;
        case InstructionType::FSUB: return rsUnitFpAdd;
        case InstructionType::FMUL: return rsUnitFpMult;
        case InstructionType::FDIV: return rsUnitFpDiv;
        case InstructionType::BNE: return rsUnitBu;
    }
}

void Execute::updateReservationStation(ROBStatus inpEntry, RSStatus inpRs){
    // replace rs in corresponding unit
    vector<RSStatus> & rsu = getReservationStationUnitFromInstructionType(inpEntry.instruction.opcode);
    int index = getReservationStationIndex(
        rsu,
        inpRs
    );
    if (index != -1) rsu[index] = inpRs;
}

// updates Vj, Vk for each rs in unit only (along with any changes on the base inpEntry ROBStatus obj)
void Execute::updateAllReservationStationsUsingEntry(ROBStatus inpEntry, vector<RSStatus> & inpRsUnit) {
    for (auto & loopRs : inpRsUnit) {
        bool vjIsUpdated = false, vkIsUpdated = false;
        // Vj
        if (loopRs.qj == inpEntry.entryName) {
            loopRs.vj = inpEntry.instruction.rd;
            loopRs.qj = "";
            vjIsUpdated = true;
        }
        if (loopRs.qk == inpEntry.entryName) {
            loopRs.vk = inpEntry.instruction.rd;
            loopRs.qk = "";
            vkIsUpdated = true;
        }
        if (vjIsUpdated || vkIsUpdated) {
            // replace rs in corresponding unit
            updateReservationStation(inpEntry, loopRs);
        }
    }
}

void Execute::removeReservationStation(ROBStatus inpEntry, RSStatus inpRs){
    // replace rs in corresponding unit
    vector<RSStatus> & rsu = getReservationStationUnitFromInstructionType(inpEntry.instruction.opcode);
    int index = getReservationStationIndex(
        rsu,
        inpRs
    );
    if (index != -1) rsu.erase(rsu.begin() + index);
}

// perform operation based on InstructionType
double Execute::alu(ROBStatus inpEntry, RSStatus inpRs) {
    double newVj;
    if (cdb.count(inpRs.vj)) newVj = cdb[inpRs.vj];
    else newVj = physicalRegs[inpRs.vj];
    double newVk;
    if (cdb.count(inpRs.vk)) newVk = cdb[inpRs.vk];
    else if (
        inpEntry.instruction.opcode != InstructionType::ADDI &&
        inpEntry.instruction.opcode != InstructionType::FLD
    ) {
        if (inpEntry.instruction.rt == "loop") {
            cout << "ALU SEES instruction.rt AS 'LOOP' \n";
            cout << inpEntry.instruction.rt << "\n";
        }
        // TODO: FIX USING BRANCH TABLE
        if (inpEntry.instruction.rt == ZERO_REGISTER_NAME) newVk = 0;
        else if (inpEntry.instruction.opcode == InstructionType::BNE) {
                if (inpEntry.instruction.opcode == InstructionType::BNE) {
                     cout << "\nBNE NEW VK (rd = " << inpEntry.instruction.rd << ") count = " << physicalRegs.count(inpEntry.instruction.rd) << " val = " << physicalRegs[inpEntry.instruction.rd];
                }
            newVk = physicalRegs[inpEntry.instruction.rd];
        }
        else newVk = physicalRegs[inpEntry.instruction.rt];
    }

    cout << "PERFORMING ALU FOR=\n";
    printROBStatus(inpEntry);
    printRSStatus(inpRs);

    switch (inpEntry.instruction.opcode) {
        case InstructionType::FLD: {
            cout << "\nFLD alu hit (memories key = Vj + imm)= " <<
                newVj <<
                " + " <<
                inpEntry.instruction.immediate <<
                " = " << memories[newVj + inpEntry.instruction.immediate] <<
                "(key= " << newVj + inpEntry.instruction.immediate << ")" <<
                "\n"; 
            return memories[newVj + inpEntry.instruction.immediate];
        }
        case InstructionType::FSD: {
            cout << "\nFSD alu hit (val = Vk + imm)= " <<
                newVk <<
                " + " <<
                inpEntry.instruction.immediate <<
                " = " << newVk + inpEntry.instruction.immediate <<
                "\n"; 
            return newVk + inpEntry.instruction.immediate;
        }
        case InstructionType::ADD: {
            cout << "\nADD alu hit (val = Vj + Vk)= " <<
                newVj <<
                " + " <<
                newVk <<
                " = " << (int)(newVj + newVk) <<
                "\n"; 
            return (int)(newVj + newVk);
        }
        case InstructionType::ADDI: {
            cout << "\nADDI alu hit (val = Vj + imm)= " <<
                newVj <<
                " + " <<
                inpEntry.instruction.immediate <<
                " = " << newVj + inpEntry.instruction.immediate <<
                "\n"; 
            return newVj + inpEntry.instruction.immediate;
        }
        case InstructionType::SLT: {
            cout << "\nSLT alu hit (val = Vj < Vk)= " <<
                newVj <<
                " < " <<
                newVk <<
                " = " << (newVj < newVk) <<
                "\n"; 
            return newVj < newVk; // double val returned as 0(false) or 1(true)
        }
        case InstructionType::FADD: {
            cout << "\nFADD alu hit (val = Vj + Vk)= " <<
                newVj <<
                " + " <<
                newVk <<
                " = " << newVj + newVk <<
                "\n"; 
            return newVj + newVk;
        }
        case InstructionType::FSUB: {
            cout << "\nFSUB alu hit (val = Vj - Vk)= " <<
                newVj <<
                " - " <<
                newVk <<
                " = " << newVj - newVk <<
                "\n"; 
            return newVj - newVk;
        }
        case InstructionType::FMUL: {
            cout << "\nFMUL alu hit (val = Vj * Vk)= " <<
                newVj <<
                " * " <<
                newVk <<
                " = " << newVj * newVk <<
                "\n"; 
            return newVj * newVk;
        }
        case InstructionType::FDIV: {
            cout << "\nFDIV alu hit (val = Vj / Vk)= " <<
                newVj <<
                " / " <<
                newVk <<
                " = " << newVj / newVk <<
                "\n"; 
            return newVj / newVk;
        }
        case InstructionType::BNE: {
            cout << "\nBNE alu hit (val = Vj != Vk)= " <<
                newVj <<
                " != " <<
                newVk <<
                " = " << (newVj != newVk) <<
                "\n"; 
            return newVj != newVk; // double val returned as 0(false) or 1(true)
        }
        default: throw invalid_argument(
            "e_alu: the provided InstructionType was not recognized. Check to ensure that input parameters are not null or invalid"
        );

    }
    return 0;
}

bool Execute::dispatch() {
    cout << "\ne_dispatch called (nw=" << nw << " nr=" << nr << " nb=" << nb << ")=\n";
    bool memoryIsBusy = false;
    int entriesWritingResultCount = 0;
    for (ROBStatus & entry : rob) {
        // update each ROB entry
        switch (entry.status) {
            case InstructionStatusType::COMMIT: {
                continue;
            }
            case InstructionStatusType::WRITING_RESULT: {
                entry.status = InstructionStatusType::COMMIT;
                break;
            }
            case InstructionStatusType::EXECUTING: {
                // done executing
                if (entry.countLatency == 0) {
                    if (
                        entry.instruction.opcode == InstructionType::FSD ||
                        entry.instruction.opcode == InstructionType::FLD
                    ) {
                        // set memory as busy
                        if (!memoryIsBusy) memoryIsBusy = true;
                        else continue;
                    }
                    // update status to writing result only if cdb available
                    if (entriesWritingResultCount != nb) {
                        entry.status = InstructionStatusType::WRITING_RESULT;
                        entriesWritingResultCount++;
                    } else continue;
                    // check value exists
                    if (!isnan(entry.value)) {
                        // validate branch prediction
                        if (entry.instruction.opcode == InstructionType::BNE) {
                            programCounter = entry.instruction.address + 1;
                            if (entry.value == 1) {
                                // branch will be taken
                                programCounter = btb[entry.instruction.address].first;
                                if (mappingTableHistory.size()) mappingTableHistory.pop_front();
                                if (freeListHistory.size()) freeListHistory.pop_front();
                            }
                            if (btb[entry.instruction.address].second != entry.value) {
                                // branch prediction was wrong, need to update branch predictor state and flush
                                cout << "\nINVALID BRANCH PREDICTION, FLUSHING PIPELINE\n";
                                // update branch predictor state
                                dbp.updateState(false);
                                // flush pipeline
                                btb[entry.instruction.address].second = entry.value;
                                dInstructionQueue.clear();
                                fInstructionQueue.clear();
                                if (freeListHistory.size()) {
                                    freeList = freeListHistory.front();
                                    freeListHistory.clear();
                                }
                                if (mappingTableHistory.size()) {
                                    mappingTable = mappingTableHistory.front();
                                    mappingTableHistory.clear();
                                }
                                // flush rob until entry is head, start from back and find current instr
                                for (int i = rob.size() - 1; i >= 0; i--) {
                                    if (rob[i].instruction.address != entry.instruction.address) {
                                        // clear corresponding rs and rob
                                        RSStatus rs = getReservationStationFromInstruction(rob[i].instruction);
                                        removeReservationStation(rob[i], rs);
                                        rob.pop_back();
                                        continue;
                                    } else {
                                        // found current entry in rob
                                        // stall!
                                        cout << "\nstall e because branch prediction was incorrect\n";
                                        return false;
                                    }
                                }
                            }
                        } else {
                            // insert into cdb
                            if (entry.instruction.opcode != InstructionType::FSD) cdb[entry.instruction.rd] = entry.value;
                            else {
                                // stall!
                                cout << "\nstall e instead of storing FSD instr onto CDB (rs name=" << getRSNameFromInstructionType(entry.instruction.opcode) << ")\n";
                                return false;
                            }

                        }
                    }
                    // for all rs units, set Vj, Vk for the reservation stations using the current entry
                    updateAllReservationStationsUsingEntry(entry, rsUnitInt);
                    updateAllReservationStationsUsingEntry(entry, rsUnitLoad);
                    updateAllReservationStationsUsingEntry(entry, rsUnitStore);
                    updateAllReservationStationsUsingEntry(entry, rsUnitFpAdd);
                    updateAllReservationStationsUsingEntry(entry, rsUnitFpMult);
                    updateAllReservationStationsUsingEntry(entry, rsUnitFpDiv);
                    updateAllReservationStationsUsingEntry(entry, rsUnitBu);
                } else {
                    // not done executing, decrement
                    entry.countLatency--;
                }
                break;
            }
            case InstructionStatusType::ISSUED: {
                RSStatus rs = getReservationStationFromInstruction(entry.instruction);
                // check if source operand reservation stations are empty
                if (rs.qj == "" && rs.qk == "") {
                    // operands are ready
                    entry.countLatency = getLatencyFromRSName(getRSNameFromInstructionType(entry.instruction.opcode));
                    entry.status = InstructionStatusType::EXECUTING;
                    entry.value = alu(entry, rs);
                    entry.countLatency--;
                } else {
                    // operands are not ready
                    bool qjIsUpdated = false, qkIsUpdated = false;
                    for (auto & loopEntry : rob) {
                        // Qj
                        if (
                            rs.qj != "" &&
                            entry.entryName == loopEntry.entryName &&
                            (loopEntry.status == InstructionStatusType::WRITING_RESULT || loopEntry.status == InstructionStatusType::COMMIT)
                        ) {
                            rs.qj = "";
                            qjIsUpdated = true;
                        }
                        // Qk
                        if (
                            rs.qk != "" &&
                            entry.entryName == loopEntry.entryName &&
                            (loopEntry.status == InstructionStatusType::WRITING_RESULT || loopEntry.status == InstructionStatusType::COMMIT)
                        ) {
                            rs.qk = "";
                            qkIsUpdated = true;
                        }
                        if (qjIsUpdated && qkIsUpdated) break;
                    }
                }
                // replace rs in corresponding unit
                updateReservationStation(entry, rs);
                break;
            }
            default: {
                // decrement latency 
                entry.countLatency--;
                break;
            }
        }
    }
    //
    if (rob.size() && rob.front().status == InstructionStatusType::COMMIT) {
        // handle storing value into register or memory
        ROBStatus & commitEntry = rob.front();
        rob.pop_front();
        // remove corresponding rs
        RSStatus commitRs = getReservationStationFromInstruction(commitEntry.instruction);
        removeReservationStation(commitEntry, commitRs);
        //
        switch (commitEntry.instruction.opcode) {
            case InstructionType::FSD: {
                double commitVal = NAN;
                // cdb
                if (cdb.count(commitEntry.instruction.rs)) {
                    commitVal = cdb[commitEntry.instruction.rs];
                    // remove cdb entry
                    cdb.erase(commitEntry.instruction.rs);
                    // update register free list
                    freeList.push_back(commitEntry.instruction.rs);
                } else if (physicalRegs.count(commitEntry.instruction.rs)) {
                    commitVal = physicalRegs[commitEntry.instruction.rs];
                }
                // memory
                memories[commitEntry.value] = commitVal; 
                break;
            }
            case InstructionType::BNE: break;
            default: {
                double commitVal = NAN;
                // cdb
                if (cdb.count(commitEntry.instruction.rd)) {
                    commitVal = cdb[commitEntry.instruction.rd];
                    // remove cdb entry
                    cdb.erase(commitEntry.instruction.rd);
                    // update register free list
                    freeList.push_back(commitEntry.instruction.rd);
                } else {
                    commitVal = commitEntry.value;
                }
                // register
                physicalRegs[commitEntry.instruction.rd] = commitVal;
                break;
            }
        }
    }
    return true;
}