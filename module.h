#ifndef H_MODULE
#define H_MODULE

#include "common.h"

class Module {
    private:
    public:
        ModuleType moduleType;
        void printModuleType() {
            cout << "printing ModuleType = " << moduleType << "\n";
        }
};

#endif