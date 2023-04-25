#ifndef H_EXECUTE
#define H_EXECUTE

#include "stage.h"
#include "branchPredictor.h"
#include "alu.h"

#define INT_LATENCY 1
#define LOAD_LATENCY 1
#define STORE_LATENCY 1
#define FPADD_LATENCY 3
#define FPMULT_LATENCY 4
#define FPDIV_LATENCY 8
#define BU_LATENCY 1

#define FPADD_IS_PIPELINED true
#define FPMULT_IS_PIPELINED true
#define FPDIV_IS_PIPELINED false

class Execute: public Stage {
    private:
        // reservation stations
        vector<RSStatus> & rsUnitInt;
        vector<RSStatus> & rsUnitLoad;
        vector<RSStatus> & rsUnitStore;
        vector<RSStatus> & rsUnitFpAdd;
        vector<RSStatus> & rsUnitFpMult;
        vector<RSStatus> & rsUnitFpDiv;
        vector<RSStatus> & rsUnitBu;
        deque<ROBStatus> & rob;
        unordered_map<string, double> & cdb;
        unordered_map<string, double> & physicalRegs;
        unordered_map<int, double> & memories;
        deque<string> & freeList;
        // branch prediction and flush
        BranchPredictor * dbp;
        int & programCounter;
        unordered_map<int, pair<int, int>> & btb;
        unordered_map<string, string> & mappingTable;
        deque<deque<string>> & freeListHistory;
        deque<unordered_map<string, string>> & mappingTableHistory;
        deque<Instruction> & fInstructionQueue;
        deque<Instruction> & dInstructionQueue;
        const int nw;
        const int nr;
        const int nb;
        const bool debugMode;
        // alu module
        Alu * alu;
        int getLatencyFromRSName(string inpName){
            if (inpName == "LOAD") return LOAD_LATENCY;
            else if (inpName == "STORE") return STORE_LATENCY;
            else if (inpName == "INT") return INT_LATENCY;
            else if (inpName == "FPADD") return FPADD_LATENCY;
            else if (inpName == "FPMULT") return FPMULT_LATENCY;
            else if (inpName == "FPDIV") return FPDIV_LATENCY;
            else if (inpName == "BU") return BU_LATENCY;
            else throw invalid_argument(
                "e_getLatencyFromRSName: the provided InstructionType was not recognized. Check to ensure that input parameters are not null or invalid"
            );
        }
        RSStatus getReservationStationFromInstruction(Instruction inpInstruction);
        int getReservationStationIndex(vector<RSStatus> inpRsUnit, RSStatus inpReservationStation);
        vector<RSStatus> & getReservationStationUnitFromInstructionType(InstructionType inpInstrType);
        void updateReservationStation(ROBStatus inpEntry, RSStatus inpRs);
        void updateAllReservationStationsUsingEntry(ROBStatus inpEntry, vector<RSStatus> & inpRsUnit);
        void removeReservationStation(ROBStatus inpEntry, RSStatus inpRs);
        // double alu(ROBStatus inpEntry, RSStatus inpRs);
    public:
        bool dispatch();
    Execute(
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
        BranchPredictor * dbp,
        int & programCounter,
        unordered_map<int, pair<int, int>> & btb,
        unordered_map<string, string> & mappingTable,
        deque<deque<string>> & freeListHistory,
        deque<unordered_map<string, string>> & mappingTableHistory,
        deque<Instruction> & fInstructionQueue,
        deque<Instruction> & dInstructionQueue,
        const int nw,
        const int nr,
        const int nb,
        const bool debugMode
    );
    ~Execute();
};

#endif