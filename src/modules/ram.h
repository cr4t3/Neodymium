#pragma once
#include "def.h"

struct RAM {
    bytes memory;
    uint16_t pc;
    
    RAM();
    ~RAM();
    byte current();
    byte next();
    byte get_from_address(uint16_t addr);
    uint16_t next_16bit_immediate();
    int write(uint16_t address, byte data);
};