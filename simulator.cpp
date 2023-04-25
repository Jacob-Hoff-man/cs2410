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

void Simulator::initSimulatorPhysicalRegs() {
    // add $0 reg
    physicalRegs[ZERO_REGISTER_NAME] = 0;
    // add remaining physical regs
    for (int i = 1; i < REGISTER_COUNT; i++) {
        string physicalName = "p" + to_string(i);
        // init physical reg and status
        // initialize all to zero
        physicalRegs[physicalName] = 0;
        // init free list
        freeList.push_back(physicalName);
    }
    // init mappingTable
    mappingTable = unordered_map<string, string>();
    // init mappingTable history
    mappingTableHistory= deque<unordered_map<string, string>>();
    // init freeList history
    freeListHistory = deque<deque<string>>();
}

void Simulator::printSimulatorInstructions() {
    cout << "\nINSTRUCTIONS CACHE (size=" << instructions.size() << ")\n";
    printInstructions(instructions);
}

void Simulator::printSimulatorFetchInstructionQueue() {
    cout << "\nFETCH INSTRUCTION QUEUE (size= " << fInstructionQueue.size() << ")\n";
    printInstructions(fInstructionQueue);
}

void Simulator::printSimulatorDecodeInstructionQueue() {
    cout << "\nDECODE INSTRUCTION QUEUE (size= " << dInstructionQueue.size() << ")\n";
    printInstructions(dInstructionQueue);
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
        default: throw invalid_argument(
            "simulator_tokenizeMemory: the provided string was not recognized as an InstructionType. Check to ensure that input parameters are not null or unequal"
        );
    }
    // add branch label to table if exists
    if (!branchLabel.empty()) branchLabelsTable[branchLabel] = address;
    // build current Instruction struct
    Instruction currentInstr;
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

void Simulator::cyclePipeline() {
    // TODO: set up Stage::debug() and use here for each stage
    cout << "\n";
    printSimulatorCurrentCycleCount();
    // run stages backwards
    if (!e->dispatch()) eStallCount++;
    if (!i->dispatch()) iStallCount++;
    if (!d->dispatch()) dStallCount++;
    if (!f->dispatch()) fStallCount++;
    tickCycleCount();
    if (debugMode) {
        cout << "\nPOST CYCLE RESULTS\n";
        // debug per cycle
        // printSimulatorMemories();
        printSimulatorDecodeInstructionQueue();
        // printSimulatorBranchLabelsTable();
        // printSimulatorBtbMap();
        // printSimulatorPhysicalRegs();
        // printSimulatorMappingTable();
        // printSimulatorFreeList();
        // printSimulatorMappingTableHistory();
        // printSimulatorFreeListHistory();
        printSimulatorReservationStations();
        printSimulatorROB();
        // printSimulatorStallCounts();
    }
}

void Simulator::cyclePipeline(int cycles) {
    for (int i = 0; i < cycles; i++) {
        cyclePipeline();
        if (debugMode) {
            // debug per cycle
            // printSimulatorMemories();
            // printSimulatorDecodeInstructionQueue();
            // printSimulatorBranchLabelsTable();
            // printSimulatorBtbMap();
            // printSimulatorPhysicalRegs();
            // printSimulatorMappingTable();
            // printSimulatorFreeList();
            // printSimulatorMappingTableHistory();
            // printSimulatorFreeListHistory();
            // printSimulatorReservationStations();
            // printSimulatorROB();
            // printSimulatorStallCounts();
        }
    }
}

void Simulator::execute() {
    cout << "\nstart\n";
    do cyclePipeline();
    while (fInstructionQueue.size() > 0 || dInstructionQueue.size() > 0 || rob.size() > 0);
    cout << "\ndone\n\nPOST EXECUTE RESULTS:\n";
    // debug post execution
    printSimulatorMemories();
    // printSimulatorDecodeInstructionQueue();
    // printSimulatorBranchLabelsTable();
    // printSimulatorBtbMap();
    printSimulatorPhysicalRegs();
    printSimulatorMappingTable();
    // printSimulatorFreeList();
    // printSimulatorMappingTableHistory();
    // printSimulatorFreeListHistory();
    // printSimulatorReservationStations();
    // printSimulatorROB();
    printSimulatorStallCounts();
    cout << "\n";
}

Simulator::Simulator(
    string inpFileName,
    int nf,
    int ni,
    int nw,
    int nb,
    int nr,
    bool debugMode
) :
    inpFileName(inpFileName),
    nf(nf),
    ni(ni),
    nw(nw),
    nb(nb),
    nr(nr),
    debugMode(debugMode)
{
    // init
    readInputFile(inpFileName.c_str());
    initSimulatorPhysicalRegs();
    this->dbp = new BranchPredictor(
        debugMode
    );
    // init pipeline stages
    this->f = new Fetch(
        instructions,
        fInstructionQueue,
        programCounter,
        btb,
        nf,
        debugMode
    );
    this->d = new Decode(
        fInstructionQueue,
        btb,
        dInstructionQueue,
        mappingTable,
        freeList,
        mappingTableHistory,
        freeListHistory,
        branchLabelsTable,
        dbp,
        nf,
        ni,
        debugMode
    );
    this->i = new Issue(
        dInstructionQueue,
        rsUnitInt,
        rsUnitLoad,
        rsUnitStore,
        rsUnitFpAdd,
        rsUnitFpMult,
        rsUnitFpDiv,
        rsUnitBu,
        rob,
        nw,
        nr,
        debugMode
    );
    this->e = new Execute(
        rsUnitInt,
        rsUnitLoad,
        rsUnitStore,
        rsUnitFpAdd,
        rsUnitFpMult,
        rsUnitFpDiv,
        rsUnitBu,
        rob,
        cdb,
        physicalRegs,
        memories,
        freeList,
        dbp,
        programCounter,
        btb,
        mappingTable,
        freeListHistory,
        mappingTableHistory,
        fInstructionQueue,
        dInstructionQueue,
        nw,
        nr,
        nb,
        debugMode
    );
}

Simulator::~Simulator() {}