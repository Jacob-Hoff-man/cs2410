#include "common.h"

bool operator==(Instruction const &lhs, Instruction const &rhs) {
    return lhs.address == rhs.address and
        lhs.opcode == rhs.opcode and
        lhs.rd == rhs.rd and
        lhs.rs == rhs.rs and
        lhs.rt == rhs.rt and
        lhs.immediate == rhs.immediate;
}
 
bool operator!=(Instruction const &lhs, Instruction const &rhs) {
    return lhs.address != rhs.address or
        lhs.opcode != rhs.opcode or
        lhs.rd != rhs.rd or
        lhs.rs != rhs.rs or
        lhs.rt != rhs.rt or
        lhs.immediate != rhs.immediate;
}

string toUpper(string inpString) {
    string temp = inpString;
    transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
    return temp;
}

InstructionType getInstructionTypeFromString(const string inpString) {
    if (inpString == "FLD") return InstructionType::FLD;
    else if (inpString == "FSD") return InstructionType::FSD;
    else if (inpString == "ADD") return InstructionType::ADD;
    else if (inpString =="ADDI") return InstructionType::ADDI;
    else if (inpString =="SLT") return InstructionType::SLT;
    else if (inpString =="FADD") return InstructionType::FADD;
    else if (inpString =="FSUB") return InstructionType::FSUB;
    else if (inpString =="FMUL") return InstructionType::FMUL;
    else if (inpString =="FDIV") return InstructionType::FDIV;
    else if (inpString =="BNE") return InstructionType::BNE;
    else throw invalid_argument(
        "common_getInstructionTypeFromString: the provided string was not recognized as an InstructionType. Check to ensure that input parameters are not null or unequal"
    );
};

string getStringFromInstructionType(const InstructionType inpInstrType) {
    switch (inpInstrType) {
        case InstructionType::FLD: return "FLD";
        case InstructionType::FSD: return "FSD";
        case InstructionType::ADD: return "ADD";
        case InstructionType::ADDI: return "ADDI";
        case InstructionType::SLT: return "SLT";
        case InstructionType::FADD: return "FADD";
        case InstructionType::FSUB: return "FSUB";
        case InstructionType::FMUL: return "FMUL";
        case InstructionType::FDIV: return "FDIV";
        case InstructionType::BNE: return "BNE";
        default: throw invalid_argument(
            "common_getStringFromInstructionType: the provided InstructionType was not recognized. Check to ensure that input parameters are not null or invalid"
        );
    }
};

void printInstruction(Instruction inpInstr) {
    cout << "\n     Instruction= {" <<
        "\n         address=" << inpInstr.address <<
        "\n         opcode=" << getStringFromInstructionType(inpInstr.opcode) << " (" << inpInstr.opcode << ")" <<
        "\n         rd=" << inpInstr.rd <<
        "\n         rs=" << inpInstr.rs <<
        "\n         rt=" << inpInstr.rt <<
        "\n         immediate=" << inpInstr.immediate <<
        "\n     }\n";
}

void printInstructions(deque<Instruction> inpInstrs) {
    for(int i = 0; i < inpInstrs.size(); i++) {
        printInstruction(inpInstrs[i]);
    }
}

bool containsOffset(const string inpString) {
    int pos1 = inpString.find("(");
    int pos2 = inpString.find(")");
    return (pos1 != -1 && pos2 != -1);
}

int getOffset(const string inpString) {
    int offset = 0;
    int pos1 = inpString.find("(");
    if (pos1 != -1) {
        string offsetStr = inpString.substr(0, pos1);
        offset = stoi(offsetStr);
        return offset;
    }
    return offset;
}

string trimOffset(const string inpString) {
    string newStr = inpString;
    int pos1 = inpString.find("(");
    int pos2 = inpString.find(")");
    if (pos1 != -1 && pos2 != -1) {
        newStr = inpString.substr(pos1 + 1, pos2-pos1-1);
        return newStr;
    }
    return newStr;
}

string appendOffset(const string inpString, const int offset) {
    return to_string(offset) + "(" + inpString + ")";
}

void printROBStatus(ROBStatus inpEntry) {
    cout << "   REORDER BUFFER ENTRY =\n" <<
        "   entry name=" << inpEntry.entryName << "\n" <<
        "   busy=" << inpEntry.busy << "\n" <<
        "   status=" << inpEntry.status << "\n" <<
        "   destination=" << inpEntry.instruction.rd << "\n" <<
        "   value=" << inpEntry.value << "\n" <<
        "   countLatency=" << inpEntry.countLatency;
    printInstruction(inpEntry.instruction);

}

void printRSStatus(RSStatus inpRs) {
    cout << "   RESERVATION STATION ENTRY =\n" <<
        "   busy=" << inpRs.busy << "\n" <<
        "   vj=" << inpRs.vj << "\n" <<
        "   vk=" << inpRs.vk << "\n" <<
        "   qj=" << inpRs.qj << "\n" <<
        "   qk=" << inpRs.qk << "\n" <<
        "   destination=" << inpRs.qk << "\n" <<
        "   a=" << inpRs.a << "\n";
    printInstruction(inpRs.instruction);
}

string getRSNameFromInstructionType(InstructionType inpInstrType) {
    switch (inpInstrType) {
        case InstructionType::FLD: return "LOAD";
        case InstructionType::FSD: return "STORE";
        case InstructionType::ADD: return "INT";
        case InstructionType::ADDI: return "INT";
        case InstructionType::SLT: return "INT";
        case InstructionType::FADD: return "FPADD";
        case InstructionType::FSUB: return "FPADD";
        case InstructionType::FMUL: return "FPMULT";
        case InstructionType::FDIV: return "FPDIV";
        case InstructionType::BNE: return "BU";
        default: throw invalid_argument(
            "common_getRSNameFromInstructionType: the provided InstructionType was not recognized. Check to ensure that input parameters are not null or invalid"
        );
    }
}