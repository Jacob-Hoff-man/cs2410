#ifndef H_EXECUTE
#define H_EXECUTE

#include "stage.h"

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
        const int nw;
        const int nr;

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
        const int nw,
        const int nr
    );
    ~Execute();
};

#endif