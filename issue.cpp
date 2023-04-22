#include "issue.h"

Issue::Issue(
    deque<Instruction> & dInstructionQueue,
    vector<RSStatus> & rsUnitInt,
    vector<RSStatus> & rsUnitLoad,
    vector<RSStatus> & rsUnitStore,
    vector<RSStatus> & rsUnitFpAdd,
    vector<RSStatus> & rsUnitFpMult,
    vector<RSStatus> & rsUnitFpDiv,
    vector<RSStatus> & rsUnitBu,
    deque<ROBStatus> & rob,
    const int nw,
    const int nr
) :
    dInstructionQueue(dInstructionQueue),
    rsUnitInt(rsUnitInt),
    rsUnitLoad(rsUnitLoad),
    rsUnitStore(rsUnitStore),
    rsUnitFpAdd(rsUnitFpAdd),
    rsUnitFpMult(rsUnitFpMult),
    rsUnitFpDiv(rsUnitFpDiv),
    rsUnitBu(rsUnitBu),
    rob(rob),
    nw(nw),
    nr(nr)
{
    this->stageType = StageType::ISSUE;
    printStageType();
    cout << "\nNW in Issue stage = " << nw;
    cout << "\nNR in Issue stage = " << nr;
    cout << "\n";
};

bool Issue::insertInstructionInReservationStation(
    vector<RSStatus> & inpReservationStation,
    int inpReservationStationCount,
    Instruction inpInstruction
) {
    if (inpReservationStation.size() >= inpReservationStationCount) {
        // reservation station is full,
        // stall "callback"
        return false;
    }
    // create new rs entry, set to busy
    RSStatus rsEntry;
    rsEntry.instruction = inpInstruction;
    rsEntry.busy = true;
    inpReservationStation.push_back(rsEntry);
    return true;
}

RSStatus Issue::getReservationStationForInstruction(
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

int Issue::getReservationStationIndex(
    vector<RSStatus> inpRsUnit,
    RSStatus inpReservationStation
) {
    for (int i = 0; i < inpRsUnit.size(); i++) {
        if (inpRsUnit.at(i).instruction == inpReservationStation.instruction)
            return i;
    }
    throw invalid_argument(
        "reservation station does not exist in the unit. Check to ensure that input parameters are not null"
    );
}

ROBStatus & Issue::getROBStatusEntryForInstruction(
    Instruction inpInstruction
) {
    for(auto & entry : this->rob) {
        if (entry.instruction == inpInstruction) return entry;
    }
    throw invalid_argument(
        "rob status entry does not exist that contains the instruction. Check to ensure that input parameters are not null"
    );
}

int Issue::getROBStatusEntryIndex(
    deque<ROBStatus> inpRob,
    ROBStatus inpEntry
) {
    for (int i = 0; i < inpRob.size(); i++) {
    if (inpRob.at(i).instruction == inpEntry.instruction)
        return i;
    }
    throw invalid_argument(
        "rob status entry does not exist in the buffer. Check to ensure that input parameters are not null"
    );
}

vector<RSStatus> Issue::getReservationStationUnitFromInstructionType(InstructionType inpInstrType) {
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

string Issue::generateROBStatusEntryName(Instruction inpInstr) {
    int unitSize = getReservationStationUnitFromInstructionType(inpInstr.opcode).size();
    return getRSNameFromInstructionType(inpInstr.opcode) + to_string(unitSize);
}


bool Issue::dispatch() {
    cout << "\ni_dispatch called (nw=" << nw << " nr=" << nr << ")=\n";
    
    for (int i = 0; i < nw; i++) {
        if (dInstructionQueue.empty() || rob.size() >= nr) {
            // no instructions are available from decode stage,
            // or if reorder buffer is full
            // stall
            if (dInstructionQueue.empty()) cout << "stall i because dInstrQueue empty\n";
            cout << rob.size() << " and " << this->nr;
            if (rob.size() == this->nr) cout << "stall i because rob size == nr \n";
            return false;
        } else {
            Instruction iInstr = this->dInstructionQueue.front();
            // attempt to insert current instruction into corresponding reservation station
            bool success = false;
            switch(iInstr.opcode) {
                case InstructionType::FLD:
                    success = insertInstructionInReservationStation(
                        rsUnitLoad,
                        RS_COUNT_LOAD,
                        iInstr
                    );
                    break;
                case InstructionType::FSD:
                    success = insertInstructionInReservationStation(
                        rsUnitStore,
                        RS_COUNT_STORE,
                        iInstr
                    );
                    break;
                case InstructionType::ADD:
                    success = insertInstructionInReservationStation(
                        rsUnitInt,
                        RS_COUNT_INT,
                        iInstr
                    );
                    break;
                case InstructionType::ADDI:
                    success = insertInstructionInReservationStation(
                        rsUnitInt,
                        RS_COUNT_INT,
                        iInstr
                    );
                    break;
                case InstructionType::SLT:
                    success = insertInstructionInReservationStation(
                        rsUnitInt,
                        RS_COUNT_INT,
                        iInstr
                    );
                    break;
                case InstructionType::FADD:
                    success = insertInstructionInReservationStation(
                        rsUnitFpAdd,
                        RS_COUNT_FPADD,
                        iInstr
                    );
                    break;
                case InstructionType::FSUB:
                    success = insertInstructionInReservationStation(
                        rsUnitFpAdd,
                        RS_COUNT_FPADD,
                        iInstr
                    );
                    break;
                case InstructionType::FMUL:
                    success = insertInstructionInReservationStation(
                        rsUnitFpMult,
                        RS_COUNT_FPMULT,
                        iInstr
                    );
                    break;
                case InstructionType::FDIV:
                    success = insertInstructionInReservationStation(
                        rsUnitFpDiv,
                        RS_COUNT_FPDIV,
                        iInstr
                    );
                    break;
                case InstructionType::BNE:
                    success = insertInstructionInReservationStation(
                        rsUnitBu,
                        RS_COUNT_BU,
                        iInstr
                    );
                    break;
                default:
                    throw invalid_argument(
                        "i_dispatch: the provided InstructionType was not recognized. Check to ensure that input parameters are not null"
                    );
            }
            if (!success) {
                // reservation stations full,
                // stall
                cout << "stall i because rs insert was unsuccessful (" << getRSNameFromInstructionType(iInstr.opcode) << " full)\n";
                return false;
            }
            // rs contains new entry for current instruction, remove from dispatch instr queue
            this->dInstructionQueue.pop_front();
            // insert entry into ROB
            ROBStatus robEntry;
            robEntry.instruction = iInstr;
            robEntry.status = InstructionStatusType::ISSUED;
            robEntry.busy = true;
            robEntry.entryName = generateROBStatusEntryName(iInstr);
            this->rob.push_back(robEntry);
            // update rs entry
            RSStatus iInstrRs = getReservationStationForInstruction(iInstr);
            iInstrRs.destination = robEntry.entryName;
            // Vj, Qj
            if (!iInstr.rs.empty()) {
                // update Vj
                iInstrRs.vj = iInstr.rs;
                // update Qj
                for (auto & entry : rob) {
                    if (
                        entry.instruction.rd == iInstr.rs &&
                        entry.status != InstructionStatusType::WRITING_RESULT
                    ) {
                        iInstrRs.qj = entry.entryName;
                        break;
                    }
                }

            }
            // Vk, Qk
            if (!iInstr.rt.empty()) {
                // update vk
                iInstrRs.vk = iInstr.rt;
                // update qk
                for (auto & entry : rob) {
                    if (
                        entry.instruction.rd == iInstr.rt &&
                        entry.status != InstructionStatusType::WRITING_RESULT
                    ) {
                        iInstrRs.qk = entry.entryName;
                        break;
                    }
                }
            }
            // Immediate
            if (
                iInstr.opcode == InstructionType::ADDI ||
                iInstr.opcode == InstructionType::FLD ||
                iInstr.opcode == InstructionType::FSD
            ) {
                iInstrRs.a = iInstr.immediate;
            }
        }
    }
    return true;
}