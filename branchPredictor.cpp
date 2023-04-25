#include "branchPredictor.h"

int BranchPredictor::getBranchPrediction() {
    switch(this->currentState) {
        case BranchPredictionType::STRONG_NOT_TAKEN: return 0;
        case BranchPredictionType::WEAK_NOT_TAKEN: return 0;
        case BranchPredictionType::STRONG_TAKEN: return 1;
        case BranchPredictionType::WEAK_TAKEN: return 1;
    }
}

void BranchPredictor::updateState(bool inpIsCorrect) {
    if (inpIsCorrect) {
        // prediction was correct
        switch(this->currentState) {
            case BranchPredictionType::STRONG_NOT_TAKEN:
                break;
            case BranchPredictionType::WEAK_NOT_TAKEN:
                this->currentState = BranchPredictionType::STRONG_NOT_TAKEN;
                break;
            case BranchPredictionType::STRONG_TAKEN:
                break;
            case BranchPredictionType::WEAK_TAKEN: 
                this->currentState = BranchPredictionType::STRONG_TAKEN;
                break;
        }
    } else {
        // prediction was wrong
        switch(this->currentState) {
            case BranchPredictionType::STRONG_NOT_TAKEN:
                this->currentState = BranchPredictionType::WEAK_NOT_TAKEN;
                break;
            case BranchPredictionType::WEAK_NOT_TAKEN:
                this->currentState = BranchPredictionType::WEAK_TAKEN;
                break;
            case BranchPredictionType::STRONG_TAKEN:
                this->currentState = BranchPredictionType::WEAK_TAKEN;
                break;
            case BranchPredictionType::WEAK_TAKEN: 
                this->currentState = BranchPredictionType::WEAK_NOT_TAKEN;
                break;
        }
    }
    if (debugMode) cout << "\n UPDATED BRANCH PREDICTOR STATE= " << this->currentState << "\n";
}

BranchPredictor::BranchPredictor(
    const bool debugMode
) : 
    debugMode(debugMode)
{
    this->moduleType = ModuleType::BRANCH_PREDICTOR;
    if (debugMode) cout << "\nINITIAL BRANCH PREDICTOR STATE= " << this->currentState << "\n";
};

