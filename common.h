#include <string>

using namespace std;

#ifndef H_COMMON
#define H_COMMON

enum InstructionType {
    FLD,
    FSD,
    ADD,
    ADDI,
    SLT,
    FADD,
    FSUB,
    FMUL,
    FDIV,
    BNE
};

struct Instruction {
    int address;
    InstructionType opcode;
    string rd;
    string rs;
    string rt;
    int immediate;
};

static string toUpper(string inpString) {
    string temp = inpString;
    transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
    return temp;
}

static InstructionType getInstructionTypeFromString(const string inpString) {
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
};

static string getStringFromInstructionType(const InstructionType inpInstrType) {
    switch (inpInstrType) {
        case InstructionType::FLD: return "FLD";
        case InstructionType::FSD: return "FLD";
        case InstructionType::ADD: return "ADD";
        case InstructionType::ADDI: return "ADDI";
        case InstructionType::SLT: return "SLT";
        case InstructionType::FADD: return "FADD";
        case InstructionType::FSUB: return "FSUB";
        case InstructionType::FMUL: return "FMUL";
        case InstructionType::FDIV: return "FDIV";
        case InstructionType::BNE: return "BNE";
        default: return "";
    }
};

static void printInstruction(Instruction inpInstr) {
    cout << "\nInstruction {" <<
        "\n     address=" << inpInstr.address <<
        "\n     opcode=" << getStringFromInstructionType(inpInstr.opcode) << " (" << inpInstr.opcode << ")" <<
        "\n     rd=" << inpInstr.rd <<
        "\n     rs=" << inpInstr.rs <<
        "\n     rt=" << inpInstr.rt <<
        "\n     immediate=" << inpInstr.immediate <<
        "\n}\n";
}

static void printInstructions(deque<Instruction> inpInstrs) {
    for(int i = 0; i < inpInstrs.size(); i++) {
        printInstruction(inpInstrs[i]);
    }
}

#endif
