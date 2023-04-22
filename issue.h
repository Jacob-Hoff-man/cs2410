
#ifndef H_ISSUE
#define H_ISSUE

#include "stage.h"

#define RS_COUNT_INT 4
#define RS_COUNT_LOAD 2
#define RS_COUNT_STORE 2
#define RS_COUNT_FPADD 3
#define RS_COUNT_FPMULT 3
#define RS_COUNT_FPDIV 2
#define RS_COUNT_BU 2

class Issue: public Stage {
    private:
        deque<Instruction> & dInstructionQueue;
        int & nw;
        int & nr;
        // reservation stations
        vector<RSStatus> & rsUnitInt;
        vector<RSStatus> & rsUnitLoad;
        vector<RSStatus> & rsUnitStore;
        vector<RSStatus> & rsUnitFpAdd;
        vector<RSStatus> & rsUnitFpMult;
        vector<RSStatus> & rsUnitFpDiv;
        vector<RSStatus> & rsUnitBu;
        deque<ROBStatus> & rob;

        bool insertInstructionInReservationStation(
            vector<RSStatus> & inpReservationStation,
            int inpReservationStationCount,
            Instruction inpInstruction
        );
        RSStatus & getReservationStationForInstruction(
            Instruction inpInstruction
        );
        int getReservationStationIndex(
            vector<RSStatus> inpRsUnit,
            RSStatus inpReseravtionStation
        );
        ROBStatus & getROBStatusEntryForInstruction(
            Instruction inpInstruction
        );
        int getROBStatusEntryIndex(
            deque<ROBStatus> inpRob,
            ROBStatus inpEntry
        );
    public:
        bool dispatch();
    Issue(
        deque<Instruction> & dInstructionQueue,
        vector<RSStatus> & rsUnitInt,
        vector<RSStatus> & rsUnitLoad,
        vector<RSStatus> & rsUnitStore,
        vector<RSStatus> & rsUnitFpAdd,
        vector<RSStatus> & rsUnitFpMult,
        vector<RSStatus> & rsUnitFpDiv,
        vector<RSStatus> & rsUnitBu,
        deque<ROBStatus> & rob,
        int & nw,
        int & nr
    );
    ~Issue();
};

#endif