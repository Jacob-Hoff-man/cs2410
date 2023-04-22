#include "execute.h"

Execute::Execute(
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
) :
    rsUnitInt(rsUnitInt),
    rsUnitLoad(rsUnitLoad),
    rsUnitStore(rsUnitStore),
    rsUnitFpAdd(rsUnitFpAdd),
    rsUnitFpMult(rsUnitFpMult),
    rsUnitFpDiv(rsUnitFpDiv),
    rsUnitBu(rsUnitBu),
    rob(rob),
    nw(nw),
    nr(nr)
{
    this->stageType = StageType::EXECUTE;
    printStageType();
    cout << "\nNW in Execute stage = " << nw;
    cout << "\nNR in Execute stage = " << nr;
    cout << "\n";
};

bool Execute::dispatch() {
    cout << "\ne_dispatch called (nw=" << nw << " nr=" << nr << ")=\n";
    return true;
}