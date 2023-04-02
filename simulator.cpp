#include <fstream>
#include <cstring>
#include <algorithm>
#include <ctype.h>
#include "simulator.h"

const char COMMENT_LINE_PREFIX = '%';
const char * DELIM_1 = ", ";
const char * DELIM_2 = "(";
const char MAX_INSTRUCTION_LENGTH = 50;

bool isCommentLine(const char inpLine []) {
    return inpLine[0] == COMMENT_LINE_PREFIX;
}

bool isMemoryLine(const char inpLine []) {
    return isdigit(inpLine[0]);
}

bool isInstructionLine(const char inpLine []) {
    return isalpha(inpLine[0]);
}

bool isBranchLabel(const string inpString) {
    return inpString[inpString.size() - 1] == ':';
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
};

string getStringFromInstructionType(const InstructionType inpInstrType) {
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

void Simulator::printInstruction(Instruction inpInstr) {
    cout << "\nInstruction {" <<
        "\n     address=" << inpInstr.address <<
        "\n     opcode=" << getStringFromInstructionType(inpInstr.opcode) << " (" << inpInstr.opcode << ")" <<
        "\n     rd=" << inpInstr.rd <<
        "\n     rs=" << inpInstr.rs <<
        "\n     rt=" << inpInstr.rt <<
        "\n     immediate=" << inpInstr.immediate <<
        "\n}\n";
}

void Simulator::printInstructions() {
    for(int i = 0; i < instructions.size(); i++) {
        printInstruction(instructions[i]);
    }
}

void Simulator::tokenizeMemory(char * inpStr) {
    vector<char *> words;
    int location, value;
    // slice line into tokens
    char * word = strtok(inpStr, DELIM_1);
    while (word) {
        words.push_back(word);
        word = strtok(NULL, DELIM_1);
    }
    location = atoi(words[0]);
    value = atoi(words[1]);
    // insert into memories
    memories[location] = value;
    if (debugMode) {
        cout << "\nMEMORY TOKENIZED\n";
        cout << "MEM LOCATION=" << location <<
            "\nMEM VALUE=" << value << 
            "\n";
    }
}

void Simulator::tokenizeInstruction(char * inpStr) {
    vector<char *> words;
    string branchLabel, op, rd, rs, rt;
    InstructionType opcode;
    int immediate = 0;
    // slice line into tokens
    char * word = strtok(inpStr, DELIM_1);
    // check if first word is branch label
    bool branchLabelExists = isBranchLabel(word);
    if (branchLabelExists) {
        string temp = word;
        branchLabel = temp.substr(0, temp.size() - 1);
    }
    int index = 0;
    while (word) {
        if (!(index == 0 && branchLabelExists)) words.push_back(word);
        word = strtok(NULL, DELIM_1);
        index++;
    }
    op = words[0];
    if (debugMode) cout << "\n OP= " << op << "\n";
    switch(getInstructionTypeFromString(toUpper(op))) {
        case InstructionType::FLD: {
            opcode = InstructionType::FLD;
            rd = words[1];
            rs = words[2];
            if (rs[rs.size() -1] == '\r') {
                rs.erase(rs.size() - 1);
            }
            for (int i = 0; i < rs.size(); ++i) {
                if (rt[i] == '(') {
                    immediate = stoi(rs.substr(0, i));
                    rs = rs.substr(i + 1, rs.size() - i - 2);
                    break;
                }
            }
            break;
        }
        case InstructionType::FSD: {
            opcode = InstructionType::FSD;
            rs = words[1];
            rt = words[2];
            if (rt[rt.size() - 1] == '\r') {
                rt.erase(rt.size() - 1);
            }
            for (int i = 0; i < rt.size(); ++i) {
                if (rt[i] == '(') {
                    immediate = stoi(rt.substr(0, i));
                    rt = rt.substr(i + 1, rt.size() - i - 2);
                    break;
                }
            }
            break;
        }
        case InstructionType::ADD: {
            opcode = InstructionType::ADD;
            rd = words[1];
            rs = words[2];
            rt = words[3];
            if (rt[rt.size() - 1] == '\r') {
                rt.erase(rt.size() - 1);
            }
            break;
        }
        case InstructionType::ADDI: {
            opcode = InstructionType::ADDI;
            rd = words[1];
            rs = words[2];
            immediate = stoi(words[3]);
            break;
        }
        case InstructionType::SLT: {
            opcode = InstructionType::SLT;
            rd = words[1];
            rs = words[2];
            rt = words[3];
            if (rt[rt.size() - 1] == '\r') {
                rt.erase(rt.size() - 1);
            }
            break;
        }
        case InstructionType::FADD: {
            opcode = InstructionType::FADD;
            rd = words[1];
            rs = words[2];
            rt = words[3];
            if (rt[rt.size() - 1] == '\r') {
                rt.erase(rt.size() - 1);
            }
            break;
        }
        case InstructionType::FSUB: {
            opcode = InstructionType::FSUB;
            rd = words[1];
            rs = words[2];
            rt = words[3];
            if (rt[rt.size() - 1] == '\r') {
                rt.erase(rt.size() - 1);
            }
            break;
        }
        case InstructionType::FMUL: {
            opcode = InstructionType::FMUL;
            rd = words[1];
            rs = words[2];
            rt = words[3];
            if (rt[rt.size() - 1] == '\r') {
                rt.erase(rt.size() - 1);
            }
            break;
        }
        case InstructionType::FDIV: {
            opcode = InstructionType::FDIV;
            rd = words[1];
            rs = words[2];
            rt = words[3];
            if (rt[rt.size() - 1] == '\r') {
                rt.erase(rt.size() - 1);
            }
            break;
        }
        case InstructionType::BNE: {
            opcode = InstructionType::BNE;
            rd = words[1];
            rs = words[2];
            rt = words[3];
            if (rt[rt.size() - 1] == '\r') {
                rt.erase(rt.size() - 1);
            }
            break;
        }
        default: {
            // TODO: add error
            cout << "word not recognized as a valid InstructionType: " << op << "\n";
        }
    }
    // add branch label to table if exists
    if (!branchLabel.empty()) branchLabelsTable[branchLabel] = address;
    // build current Instruction struct
    struct Instruction currentInstr;
    currentInstr.address = address++;
    currentInstr.opcode = opcode;
    currentInstr.rd = rd;
    currentInstr.rs = rs;
    currentInstr.rt = rt;
    currentInstr.immediate = immediate;
    // insert into instructions
    instructions.push_back(currentInstr);
    if (debugMode) {
        cout << "\nINSTRUCTION TOKENIZED\n";
        printInstruction(currentInstr);
    }
}

bool Simulator::readInputFile(const char * inpFile) {
    inpFileName = inpFile;
    ifstream file(inpFile);
    char currentLine[MAX_INSTRUCTION_LENGTH];
    if (file.is_open()) {
        while(file.getline(currentLine, MAX_INSTRUCTION_LENGTH)) {
            if (isCommentLine(currentLine)) {
                if (debugMode) cout << "COMMENT LINE= " << currentLine << "\n";
                continue;
            } else if (isMemoryLine(currentLine)) {
                if (debugMode) cout << "MEM LINE= " << currentLine << "\n";
                tokenizeMemory(currentLine);
            } else if (isInstructionLine(currentLine)) {
                if (debugMode) cout << "INSTR LINE= " << currentLine << "\n";
                tokenizeInstruction(currentLine);
            }
        }
        file.close();
        return true;
    } else {
        cout << "READING " << inpFile << " INPUT FILE WAS UNSUCCESSFUL\n";
        return false;
    }
    return true;
}

Simulator::Simulator(string inpFileName, int nf, int ni, int nw, int nb, int nr) {
    this->nf = nf;
    this->ni = ni;
    this->nw = nw;
    this->nb = nb;
    this->nr = nr;
    readInputFile(inpFileName.c_str());
    this->fetch = new Fetch();
}

Simulator::Simulator(string inpFileName, int nf, int ni, int nw, int nb, int nr, bool debugMode) {
    this->debugMode = debugMode;
    this->nf = nf;
    this->ni = ni;
    this->nw = nw;
    this->nb = nb;
    this->nr = nr;
    readInputFile(inpFileName.c_str());
    this->fetch = new Fetch();
}

Simulator::~Simulator() {}