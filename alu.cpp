#include "alu.h"

Alu::Alu(
    unordered_map<string, double> & cdb,
    unordered_map<string, double> & physicalRegs,
    unordered_map<int, double> & memories,
    const bool debugMode
) : 
    cdb(cdb),
    physicalRegs(physicalRegs),
    memories(memories),
    debugMode(debugMode)
{
    this->moduleType = ModuleType::ALU;
};

// perform operation based on InstructionType
double Alu::performOperation(ROBStatus inpEntry, RSStatus inpRs) {
double newVj;
    if (cdb.count(inpRs.vj)) newVj = cdb[inpRs.vj];
    else newVj = physicalRegs[inpRs.vj];
    double newVk;
    if (cdb.count(inpRs.vk)) newVk = cdb[inpRs.vk];
    else if (
        inpEntry.instruction.opcode != InstructionType::ADDI &&
        inpEntry.instruction.opcode != InstructionType::FLD
    ) {
        if (inpEntry.instruction.rt == ZERO_REGISTER_NAME) 
            newVk = 0;
        else if (inpEntry.instruction.opcode == InstructionType::BNE)
            newVk = physicalRegs[inpEntry.instruction.rd];
        else 
            newVk = physicalRegs[inpEntry.instruction.rt];
    }

    if (debugMode) {
        cout << "PERFORMING ALU FOR=\n";
        printROBStatus(inpEntry);
        printRSStatus(inpRs);
    }

    switch (inpEntry.instruction.opcode) {
        case InstructionType::FLD: {
            if (debugMode) cout << "\nFLD alu hit (memories key = Vj + imm)= " <<
                newVj <<
                " + " <<
                inpEntry.instruction.immediate <<
                " = " << memories[newVj + inpEntry.instruction.immediate] <<
                "(key= " << newVj + inpEntry.instruction.immediate << ")" <<
                "\n"; 
            return memories[newVj + inpEntry.instruction.immediate];
        }
        case InstructionType::FSD: {
            if (debugMode) cout << "\nFSD alu hit (val = Vk + imm)= " <<
                newVk <<
                " + " <<
                inpEntry.instruction.immediate <<
                " = " << newVk + inpEntry.instruction.immediate <<
                "\n"; 
            return newVk + inpEntry.instruction.immediate;
        }
        case InstructionType::ADD: {
            if (debugMode) cout << "\nADD alu hit (val = Vj + Vk)= " <<
                newVj <<
                " + " <<
                newVk <<
                " = " << (int)(newVj + newVk) <<
                "\n"; 
            return (int)(newVj + newVk);
        }
        case InstructionType::ADDI: {
            if (debugMode) cout << "\nADDI alu hit (val = Vj + imm)= " <<
                newVj <<
                " + " <<
                inpEntry.instruction.immediate <<
                " = " << newVj + inpEntry.instruction.immediate <<
                "\n"; 
            return newVj + inpEntry.instruction.immediate;
        }
        case InstructionType::SLT: {
            if (debugMode) cout << "\nSLT alu hit (val = Vj < Vk)= " <<
                newVj <<
                " < " <<
                newVk <<
                " = " << (newVj < newVk) <<
                "\n"; 
            return newVj < newVk; // double val returned as 0(false) or 1(true)
        }
        case InstructionType::FADD: {
            if (debugMode) cout << "\nFADD alu hit (val = Vj + Vk)= " <<
                newVj <<
                " + " <<
                newVk <<
                " = " << newVj + newVk <<
                "\n"; 
            return newVj + newVk;
        }
        case InstructionType::FSUB: {
            if (debugMode) cout << "\nFSUB alu hit (val = Vj - Vk)= " <<
                newVj <<
                " - " <<
                newVk <<
                " = " << newVj - newVk <<
                "\n"; 
            return newVj - newVk;
        }
        case InstructionType::FMUL: {
            if (debugMode) cout << "\nFMUL alu hit (val = Vj * Vk)= " <<
                newVj <<
                " * " <<
                newVk <<
                " = " << newVj * newVk <<
                "\n"; 
            return newVj * newVk;
        }
        case InstructionType::FDIV: {
            if (debugMode) cout << "\nFDIV alu hit (val = Vj / Vk)= " <<
                newVj <<
                " / " <<
                newVk <<
                " = " << newVj / newVk <<
                "\n"; 
            return newVj / newVk;
        }
        case InstructionType::BNE: {
            if (debugMode) cout << "\nBNE alu hit (val = Vj != Vk)= " <<
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