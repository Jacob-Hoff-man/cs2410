#ifndef H_SIM
#define H_SIM

#include <fstream>
// import modules
#include "branchPredictor.h"
// import stages
#include "common.h"
#include "fetch.h"
#include "decode.h"
#include "issue.h"
#include "execute.h"

class Simulator {
    private:
        // simulation
        string inpFileName = "";
        const int nf, ni, nw, nb, nr;
        bool debugMode = false;
        // system memory cache
        unordered_map<int, double> memories;
        unordered_map<string, int> branchLabelsTable;
        int address = 0;
        int cycleCount = 1;
        int programCounter = 0;
        int fStallCount = 0;
        int dStallCount = 0;
        int iStallCount = 0;
        int eStallCount = 0;        
        // system instruction cache
        deque<Instruction> instructions;
        // fetch instruction queue (before decode stage)
        deque<Instruction> fInstructionQueue;
        // branch prediction
        unordered_map<int, pair<int, int>> btb;
        BranchPredictor * dbp;
        // decode instruction queue (before issue stage)
        deque<Instruction> dInstructionQueue;
        // register renaming
        unordered_map<string, string> mappingTable;
        deque<string> freeList;
        unordered_map<string, double> physicalRegs;
        deque<unordered_map<string, string>> mappingTableHistory;
        deque<deque<string>> freeListHistory;
        // reservation stations
        vector<RSStatus> rsUnitInt;
        vector<RSStatus> rsUnitLoad;
        vector<RSStatus> rsUnitStore;
        vector<RSStatus> rsUnitFpAdd;
        vector<RSStatus> rsUnitFpMult;
        vector<RSStatus> rsUnitFpDiv;
        vector<RSStatus> rsUnitBu;
        deque<ROBStatus> rob;
        unordered_map<string, double> cdb;
        void initSimulatorPhysicalRegs();
    public:
        // stages
        Fetch * f;
        Decode * d;
        Issue * i;
        Execute * e;
        void tokenizeMemory(char * inpStr);
        void tokenizeInstruction(char * inpStr);
        bool readInputFile(const char * inpFile);
        int getMemoriesSize() {
            return memories.size();
        }
        int getInstructionsSize() {
            return instructions.size();
        }
        void tickCycleCount(bool isPositive) {
            if (isPositive) cycleCount++;
            else cycleCount--;
        }

        void cyclePipeline();
        void cyclePipeline(int cycles);
        void execute();
        void execute(int inpCycleCount);
        Simulator(
            string inpFileName,
            const int nf,
            const int ni,
            const int nw,
            const int nb,
            const int nr,
            const bool debugMode
        );
        ~Simulator();
        // debug
        void printSimulatorMemories() {
            cout << "\n\nMEMORIES CACHE (size=" << this->memories.size() << ")\n\n";
            for(const auto& elem : memories) {
                cout << "   " << elem.first << " " << elem.second << "\n";
            }
        }
        void printSimulatorInstructions();
        void printSimulatorBranchLabelsTable() {
            cout << "\n\nBRANCH LABELS TABLE (size=" << this->branchLabelsTable.size() << ")\n";
            for (const auto& elem : branchLabelsTable) {
                cout << elem.first << " " << elem.second << "\n";
            }
        }
        void printCurrentAddress() {
            cout << "\n\nAddress = " << address << "\n";
        }
        void printSimulatorCurrentCycleCount() {
            cout << "\n\ncycleCount = " << cycleCount << "\n";
        }
        void printSimulatorFetchInstructionQueue();
        void printSimulatorDecodeInstructionQueue();
        void printSimulatorBtbMap() {
            cout << "\n\nBTB (size=" << this->btb.size() << ")\n";
            for (auto & kv : btb) {
                cout << "\naddress=" << kv.first << 
                "\n   branch translation={" << kv.second.first << ", " << kv.second.second <<
                "}\n";
            }
        }
        void printSimulatorPhysicalRegs() {
            cout << "\n\nPHYSICAL REGS (size=" << this->physicalRegs.size() << ")\n";
            for (auto & kv : physicalRegs) {
                cout << "\n     name=" << kv.first <<
                "\n     val=" << kv.second <<
                "\n";
            }
        }
        void printSimulatorMappingTable() {
            cout << "\n\nMAPPING TABLE (size=" << this->mappingTable.size() << ")\n\n";
            for (const auto& elem : mappingTable) {
                cout << "   " << elem.first << " " << elem.second << "\n";
            }
        }
        void printSimulatorFreeList(){ 
            cout << "\n\nFREE LIST (size=" << this->freeList.size() << ")\n";
            for (const auto& elem : freeList) {
                cout << "\n" << elem << "\n";
            }
        }
        void printSimulatorMappingTableHistory() {
            cout << "\n\nMAPPING TABLE HISTORY (size=" << this->mappingTableHistory.size() << ")\n";
            int i = 0;
            for (const auto& mt : mappingTableHistory) {
                cout << "MAPPING TABLE index " << i++ << "\n";
                for (const auto& elem : mt) {
                    cout << elem.first << " " << elem.second << "\n";
                }
            }
        }
        void printSimulatorFreeListHistory() {
            cout << "\n\nFREE LIST HISTORY (size=" << this->freeListHistory.size() << ")\n";
            int i = 0;
            for (const auto& fl : freeListHistory) {
                cout << "FREE LIST index " << i++ << "\n";
                for (const auto& elem : fl) {
                    cout << "\n" << elem << "\n";
                }
            }
        }
        void printSimulatorROB() {
            cout << "\n\nROB (size=" << this->rob.size() << ")\n";
            for (auto & entry : this->rob) {
                printROBStatus(entry);
            }
        }
        void printSimulatorReservationStationUnit(vector<RSStatus> inpReservationStationUnit) {
            for (auto & rs : inpReservationStationUnit)
                printRSStatus(rs);
        }
        void printSimulatorReservationStations() {
            cout << "\n\nRESERVATION STATION UNIT=INT (size=" << this->rsUnitInt.size() << ")\n";
            printSimulatorReservationStationUnit(this->rsUnitInt);
            cout << "\n\nRESERVATION STATION UNIT=LOAD (size=" << this->rsUnitLoad.size() << ")\n";
            printSimulatorReservationStationUnit(this->rsUnitLoad);
            cout << "\n\nRESERVATION STATION UNIT=STORE (size=" << this->rsUnitStore.size() << ")\n";
            printSimulatorReservationStationUnit(this->rsUnitStore);
            cout << "\n\nRESERVATION STATION UNIT=FPADD (size=" << this->rsUnitFpAdd.size() << ")\n";
            printSimulatorReservationStationUnit(this->rsUnitFpAdd);
            cout << "\n\nRESERVATION STATION UNIT=FPMULT (size=" << this->rsUnitFpMult.size() << ")\n";
            printSimulatorReservationStationUnit(this->rsUnitFpMult);
            cout << "\n\nRESERVATION STATION UNIT=FPDIV (size=" << this->rsUnitFpDiv.size() << ")\n";
            printSimulatorReservationStationUnit(this->rsUnitFpDiv);
            cout << "\n\nRESERVATION STATION UNIT=BU (size=" << this->rsUnitBu.size() << ")\n";
            printSimulatorReservationStationUnit(this->rsUnitBu);
        }
        void printSimulatorStallCounts() {
            cout << "\n\nFETCH STAGE STALLS= " << fStallCount << "\n" <<
                "\nDECODE STAGE STALLS= " << dStallCount << "\n" <<
                "\nISSUE STAGE STALLS= " << iStallCount << "\n" <<
                "\nEXECUTE STAGE STALLS= " << eStallCount << "\n";
        }
};

#endif