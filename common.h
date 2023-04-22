#ifndef H_COMMON
#define H_COMMON

#include <string>
#include <iostream>
#include <deque>
#include <unordered_map>
#include <vector>
#include <math.h>

using namespace std;

// enumerations
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

enum BranchPredictionType {
    WEAK_TAKEN,
    STRONG_TAKEN,
    WEAK_NOT_TAKEN,
    STRONG_NOT_TAKEN
};

enum StageType {
    FETCH,
    DECODE,
    ISSUE,
    EXECUTE,
    MEMORY,
    WRITE_BACK,
    COMMIT
};

enum InstructionStatusType {
    ISSUED,
    EXECUTING,
    WRITING_RESULT
};

// structs
struct Instruction {
    int address;
    InstructionType opcode;
    string rd;
    string rs;
    string rt;
    int immediate;
};

static bool operator==(Instruction const &lhs, Instruction const &rhs) {
    return lhs.address == rhs.address and
        lhs.opcode == rhs.opcode and
        lhs.rd == rhs.rd and
        lhs.rs == rhs.rs and
        lhs.rt == rhs.rt and
        lhs.immediate == rhs.immediate;
}
 
static bool operator!=(Instruction const &lhs, Instruction const &rhs) {
    return lhs.address != rhs.address or
        lhs.opcode != rhs.opcode or
        lhs.rd != rhs.rd or
        lhs.rs != rhs.rs or
        lhs.rt != rhs.rt or
        lhs.immediate != rhs.immediate;
}

struct ROBStatus {
    bool busy = false;
    Instruction instruction; // opcode = type, rd = dest
    InstructionStatusType status;
    double value = NAN;
    string entryName;
    int countLatency = 0;
};

struct RSStatus {
    bool busy = false;
    Instruction instruction;
    string vj = "";
    string vk = "";
    string qj = "";
    string qk = "";
    string destination;
    int a = -1;
};

// helpers
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
        case InstructionType::FSD: return "FSD";
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

static bool containsOffset(const string inpString) {
    int pos1 = inpString.find("(");
    int pos2 = inpString.find(")");
    return (pos1 != -1 && pos2 != -1);
}

static int getOffset(const string inpString) {
    int offset = 0;
    int pos1 = inpString.find("(");
    if (pos1 != -1) {
        string offsetStr = inpString.substr(0, pos1);
        offset = stoi(offsetStr);
        return offset;
    }
    return offset;
}

static string trimOffset(const string inpString) {
    string newStr = inpString;
    int pos1 = inpString.find("(");
    int pos2 = inpString.find(")");
    if (pos1 != -1 && pos2 != -1) {
        newStr = inpString.substr(pos1 + 1, pos2-pos1-1);
        return newStr;
    }
    return newStr;
}

static string appendOffset(const string inpString, const int offset) {
    return to_string(offset) + "(" + inpString + ")";
}

static void printROBStatus(ROBStatus inpEntry) {
    cout << "REORDER BUFFER ENTRY =\n" <<
        "entry name=" << inpEntry.entryName << "\n" <<
        "busy=" << inpEntry.busy << "\n" <<
        "status=" << inpEntry.status << "\n" <<
        "destination=" << inpEntry.instruction.rd << "\n" <<
        "value=" << inpEntry.value + 2 << "\n" <<
        "countLatency=" << inpEntry.countLatency;
    printInstruction(inpEntry.instruction);

}

static void printRSStatus(RSStatus inpRs) {
    cout << "RESERVATION STATION ENTRY =\n" <<
        "busy=" << inpRs.busy << "\n" <<
        "vj=" << inpRs.vj << "\n" <<
        "vk=" << inpRs.vk << "\n" <<
        "qj=" << inpRs.qj << "\n" <<
        "qk=" << inpRs.qk << "\n" <<
        "destination=" << inpRs.qk << "\n" <<
        "a=" << inpRs.a << "\n";
    printInstruction(inpRs.instruction);
}

static string getRSUnitNameFromInstructionType(InstructionType inpInstrType) {
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
        default: return "";
    }
}
#endif
