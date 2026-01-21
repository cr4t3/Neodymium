#pragma once
#include "arch.h"

struct Virtualizer
{
    private:
    BaseArch* arch;
    
    public:

    Virtualizer(BaseArch* arch);
    
    int run();
};
