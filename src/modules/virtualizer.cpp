#include "virtualizer.h"
#include "arch.h"
#include <thread>
#include <chrono>

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
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Makes the CPU usage go from 100% to around 3% (according with neo8 testing)
    }
}