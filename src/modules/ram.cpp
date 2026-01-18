#include "ram.h"
#include "casts.h"


RAM::RAM () 
    : pc(0) 
{
    memory = new byte[0x10000](); // 0x0000 - 0xffff
};

RAM::~RAM () 
{
    delete[] memory;
};

byte RAM::current() 
{
    return memory[pc];
};

byte RAM::next() 
{
    byte result = memory[pc];
    pc++;
    return result;
}

byte RAM::get_from_address(uint16_t addr) 
{
    return memory[addr];
}

uint16_t RAM::next_16bit_immediate() 
{
    uint16_t result = bytes_to_uint16(next(), next());
    return result;
}

int RAM::write(uint16_t address, byte data) 
{
    memory[address] = data;
    if (memory[address] != data) return 1; // means error, but it's almost impossible this occurrs, i may deprecate this line
    return 0;
};