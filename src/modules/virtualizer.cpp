#include "virtualizer.h"
#include "arch.h"

Virtualizer::Virtualizer(BaseArch* arch)
: arch(arch)
{
}

int Virtualizer::run() {
    while (true) 
    {
        int res = arch->tick();
        if (res != -1) 
        {
            return res;
        }
    }
}