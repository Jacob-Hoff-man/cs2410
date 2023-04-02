#ifndef H_FETCH
#define H_FETCH

#include "stage.h"

class Fetch: public Stage {
    private:
    public:
        bool dispatch();
    Fetch();
    ~Fetch();
};

#endif