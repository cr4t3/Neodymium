#pragma once
#include "../../src/modules/arch.h"
#include "../../globals/errors.h"
#include "../../globals/def.h"
#include "casts.h"
#include "screen.h"
#include "ram.h"
#include "stack.h"
#include <fstream>


struct Arch : BaseArch{
    private:
    byte* get_register_by_address(byte addr);
    byte* get_next_as_register();
    void update_flags_with_number(int64_t num);

    bool zero;          // Indicates if last result is equal to zero
    bool underflow;     // Indicates if last result is under 0 and had to wrap around to 255
    bool overflow;      // Indicate if last result is over 255 and had to wrap around to 0

    byte ALWAYS_ZERO;
    bytes registers;

    RAM ram;
    Stack stack;
    Screen screen;

    public:

    Arch();
    const char* version;
    int tick() override;
    void input_file(std::ifstream* in, off_t size) override;
};