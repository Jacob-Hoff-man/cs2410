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
bool operator==(Instruction const &lhs, Instruction const &rhs);
bool operator!=(Instruction const &lhs, Instruction const &rhs);
string toUpper(string inpString);
InstructionType getInstructionTypeFromString(const string inpString);
string getStringFromInstructionType(const InstructionType inpInstrType);
void printInstruction(Instruction inpInstr);
void printInstructions(deque<Instruction> inpInstrs);
bool containsOffset(const string inpString);
int getOffset(const string inpString);
string trimOffset(const string inpString);
string appendOffset(const string inpString, const int offset);
void printROBStatus(ROBStatus inpEntry);
void printRSStatus(RSStatus inpRs);
string getRSNameFromInstructionType(InstructionType inpInstrType);

#endif
