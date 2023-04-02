#include "fetch.h"

Fetch::Fetch() {
    stageType = StageType::FETCH;
    printStageType();
};

bool Fetch::dispatch() {
    return false;
}

