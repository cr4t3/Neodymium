#include "stack.h"
#include "casts.h"

Stack::Stack (byte* stack_start) : sp(0) {
    stack = stack_start;
}

byte Stack::peek() {
    return stack[255-sp];
}

byte Stack::pop() {
    byte temp = stack[255-sp];
    sp--;
    return temp;
}

uint16_t Stack::pop_16bit() {
    byte x = pop();
    byte y = pop();
    return bytes_to_uint16(y, x);
}

void Stack::push(byte data) {
    stack[255-sp] = data;
    sp++;   
}

void Stack::push_16bit(uint16_t data) {
    byte* bdata = uint16_to_bytes(data);
    push(bdata[0]);
    push(bdata[1]);
}