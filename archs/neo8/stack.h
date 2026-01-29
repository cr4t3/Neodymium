#pragma once
#include "../../globals/def.h"

struct Stack {
    private:
    bytes stack;
    byte sp;

    public:
    Stack(byte* stack_start);
    byte peek();
    byte pop();
    uint16_t pop_16bit();
    void push(byte data);
    void push_16bit(uint16_t data);
};