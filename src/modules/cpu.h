#pragma once
#include "def.h"
#include "ram.h"
#include "stack.h"

struct CPU
{
    private:
    byte ALWAYS_ZERO;
    bytes registers;
    byte* get_register_by_address(byte addr);
    byte* get_next_as_register();
    void update_flags_with_number(int64_t num);

    bool zero; // Indicates if last value is equal to zero
    bool underflow; // Indicates if last value is under 0 and had to wrap around to 255
    bool overflow; // Indicate if last value is over 255 and had to wrap around to 0


    public:
    RAM ram;
    Stack stack;

    CPU();
    
    int tick();
    int run();
};
