#ifndef H_BRANCH_PREDICTOR
#define H_BRANCH_PREDICTOR

#include "module.h"

class BranchPredictor: public Module {
    private:
        // initialize state to weakly taken
        BranchPredictionType currentState = BranchPredictionType::WEAK_TAKEN;
        const bool debugMode;
    public:
        int getBranchPrediction();
        void updateState(bool inpIsCorrect);
    BranchPredictor(
        const bool debugMode
    );
    ~BranchPredictor() {};
};

#endif