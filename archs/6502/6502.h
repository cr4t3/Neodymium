#pragma once
#include <fstream>
#include "../../src/modules/arch.h"
#include "../../globals/def.h"
#include "ram.h"

struct Flags 
{
    bool c; // Carry flag
    bool z; // Zero flag
    bool i; // Interrupt disable
    bool d; // Decimal mode
    bool b; // Break command
    bool o; // Overflow flag
    bool n; // Negative flag

    Flags();
};

// For more information, read http://6502.org/users/obelisk/6502/index.html
struct Arch : BaseArch
{
    Arch();
    const char* version;
    RAM ram;
    //bool input_file_need;

    uint16_t pc; // Program Counter (0x0-0xffff / 0-65535)
    byte sp; // Stack pointer (0x0-0xff / 0-255) with designed offset of 0x100
    byte accumulator; // Used for all arithmetic and logical operations (except inc and dec)
    byte x; // Normal register. Just one special function: can be used to get a stack pointer copy or change it's value
    byte y; // Normal register. No special functions.

    Flags flags;

    int tick() override;

    void input_file(std::ifstream* in, off_t size) override;
};