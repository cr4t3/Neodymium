#include "6502.h"

Flags::Flags() 
:   c(false), 
    z(false),
    i(false),
    d(false),
    b(false),
    o(false),
    n(false)
{}

Arch::Arch() 
:   flags(Flags()),
    pc(0),
    sp(0),
    accumulator(0),
    x(0),
    y(0),
    version("6502 (v1.0.0-alpha) by Crate"),
    ram(RAM())
{
}

int Arch::tick() 
{
    
    return -1;
}

void Arch::input_file(std::ifstream* in, off_t size)
{
    in->close();
};
