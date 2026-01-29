#include "ram.h"

#define RAM_SIZE 0x10000

RAM::RAM() {
    memory = new byte[RAM_SIZE];
}

RAM::~RAM() {
    delete[] memory;
}