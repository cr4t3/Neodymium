#include "neo8.h"
#include <iostream>
#include <cmath>

#define STACK_ADDRESS   0xcf00
#define SCREEN_ADDRESS  0xa000
#define REGISTERS       8

typedef std::ifstream ifstream;

Arch::Arch() : version("Neo8 v0.1.0"), ram(RAM()), stack(Stack(&ram.memory[STACK_ADDRESS])), screen(Screen(&ram.memory[SCREEN_ADDRESS])) {
    screen.tick();
};

void Arch::input_file(std::ifstream* in, off_t size) 
{
    if (size > 0x10000) {
        raise(Errors::FILE_TOO_BIG);
    }

    for (int i = 0; i < size; i++) {
        byte b;
        in->read((char*)&b, sizeof(b));
        
        ram.write(i, b);
    }
}

int Arch::tick()
{
    byte opcode = ram.next();

    switch (opcode) {
        case 0x00: {return -1;} // NOP
        case 0x01: { // MOV $x, $y
            byte* register_x = get_next_as_register();
            byte* register_y = get_next_as_register();
            
            *register_x = *register_y;
            return -1;
        }
        case 0x02: { // MOV $x, #0
            byte* register_x = get_next_as_register();
            byte immediate = ram.next();
            
            *register_x = immediate;
            return -1;
        }
        case 0x03: { // MOV $x, [#0]
            byte* register_x = get_next_as_register();
            uint16_t immediate = ram.next_16bit_immediate();
            
            *register_x = ram.get_from_address(immediate);
            return -1;
        }
        case 0x04: { // MOV $x, [$y, $z]
            byte* register_x = get_next_as_register();
            uint16_t addr = bytes_to_uint16(
                *(get_next_as_register()),
                *(get_next_as_register())
            );
            
            *register_x = ram.get_from_address(addr);
            return -1;
        }
        case 0x10: { // NOT $x
            byte* register_x = get_next_as_register();
            
            *register_x = ~(*register_x);
            return -1;
        }
        case 0x11: { // AND $x, $y
            byte* register_x = get_next_as_register();
            byte* register_y = get_next_as_register();
            
            *register_x = (*register_x) & (*register_y);
            return -1;
        }
        case 0x12: { // AND $x, #2
            byte* register_x = get_next_as_register();
            byte immediate = ram.next();
            
            *register_x = (*register_x) & immediate;
            return -1;
        }
        case 0x20: { // JMP [#0]
            uint16_t immediate = ram.next_16bit_immediate();
            
            ram.pc = immediate;
            return -1;
        }
        case 0x21: { // JMP [$x,$y]
            uint16_t addr = bytes_to_uint16(
                *(get_next_as_register()),
                *(get_next_as_register())
            );
            
            ram.pc = addr;
            return -1;
        }
        case 0x22: { // CMP $x, #0
            byte* register_x = get_next_as_register();
            byte immediate = ram.next();
            
            int64_t result = (int64_t)*register_x + (int64_t)immediate;
            update_flags_with_number(result);
            
            return -1;
        }
        case 0x23: { // CMP $x, $y
            byte* register_x = get_next_as_register();
            byte* register_y = get_next_as_register();
            
            int64_t result = (int64_t)*register_x + (int64_t)*register_y;
            update_flags_with_number(result);
            
            return -1;
        }
        case 0x24: { // JZ [#0]
            uint16_t immediate = ram.next_16bit_immediate();
            
            if(zero) ram.pc = immediate;
            return -1;
        }
        case 0x25: { // JZ [$x,$y]
            uint16_t addr = bytes_to_uint16(
                *(get_next_as_register()),
                *(get_next_as_register())
            );
            
            if(zero) ram.pc = addr;
            return -1;
        }
        case 0x26: { // JNZ [#0]
            uint16_t immediate = ram.next_16bit_immediate();
            
            if(!zero) ram.pc = immediate;
            return -1;
        }
        case 0x27: { // JNZ [$x,$y]
            uint16_t addr = bytes_to_uint16(
                *(get_next_as_register()),
                *(get_next_as_register())
            );
            
            if(!zero) ram.pc = addr;
            return -1;
        }
        case 0x28: { // JU [#0]
            uint16_t immediate = ram.next_16bit_immediate();
            
            if(underflow) ram.pc = immediate;
            return -1;
        }
        case 0x29: { // JU [$x,$y]
            uint16_t addr = bytes_to_uint16(
                *(get_next_as_register()),
                *(get_next_as_register())
            );
            
            if(underflow) ram.pc = addr;
            return -1;
        }
        case 0x2a: { // JNU [#0]
            uint16_t immediate = ram.next_16bit_immediate();
            
            if(!underflow) ram.pc = immediate;
            return -1;
        }
        case 0x2b: { // JNU [$x,$y]
            uint16_t addr = bytes_to_uint16(
                *(get_next_as_register()),
                *(get_next_as_register())
            );
            
            if(!underflow) ram.pc = addr;
            return -1;
        }
        case 0x2c: { // JO [#0]
            uint16_t immediate = ram.next_16bit_immediate();
            
            if(overflow) ram.pc = immediate;
            return -1;
        }
        case 0x2d: { // JO [$x,$y]
            uint16_t addr = bytes_to_uint16(
                *(get_next_as_register()),
                *(get_next_as_register())
            );
            
            if(overflow) ram.pc = addr;
            return -1;
        }
        case 0x2e: { // JNO [#0]
            uint16_t immediate = ram.next_16bit_immediate();
            
            if(!overflow) ram.pc = immediate;
            return -1;
        }
        case 0x2f: { // JNO [$x,$y]
            uint16_t addr = bytes_to_uint16(
                *(get_next_as_register()),
                *(get_next_as_register())
            );
            
            if(!overflow) ram.pc = addr;
            return -1;
        }
        case 0x30: { // PUSH $x
            byte* register_x = get_next_as_register();
            
            stack.push(*register_x);
            return -1;
        }
        case 0x31: { // PUSH #0
            byte immediate = ram.next();
            
            stack.push(immediate);
            return -1;
        }
        case 0x32: { // POP $x
            byte* register_x = get_next_as_register();
            
            *register_x = stack.pop();
            return -1;
        }
        case 0x40: { // ADD $x, #0
            byte* register_x = get_next_as_register();
            byte immediate = ram.next();
            
            int64_t result = (int64_t)*register_x + (int64_t)immediate;
            update_flags_with_number(result);
            
            *register_x = (byte)result;
            
            return -1;
        }
        case 0x41: { // ADD $x, $y
            byte* register_x = get_next_as_register();
            byte* register_y = get_next_as_register();
            
            int64_t result = (int64_t)*register_x + (int64_t)*register_y;
            update_flags_with_number(result);
            
            *register_x = (byte)result;
            
            return -1;
        }
        case 0x42: { // INC $x
            byte* register_x = get_next_as_register();
            
            (*register_x)++;
            if (*register_x == 0) {
                update_flags_with_number(256); // if it's zero, then it wraped around, so it was 256 if it wasnt a byte
            }
            
            return -1;
        }
        case 0x43: { // SUB $x, #0
            byte* register_x = get_next_as_register();
            byte immediate = ram.next();
            
            int64_t result = (int64_t)*register_x + (int64_t)immediate;
            update_flags_with_number(result);
            
            *register_x = (byte)result;
            return -1;
        }
        case 0x44: { // SUB $x, $y
            byte* register_x = get_next_as_register();
            byte* register_y = get_next_as_register();
            
            int64_t result = (int64_t)*register_x + (int64_t)*register_y;
            update_flags_with_number(result);
            
            *register_x = (byte)result;
            return -1;
        }
        case 0x45: { // DEC $x
            byte* register_x = get_next_as_register();
            
            (*register_x)--;
            if (*register_x == 255) {
                update_flags_with_number(-1); // if it's 255, then it wraped around, so it was -1 if it wasnt a byte
            }
            
            return -1;
        }
        case 0x46: { // MUL $x, #0
            byte* register_x = get_next_as_register();
            byte immediate = ram.next();
            
            int64_t result = (int64_t)*register_x * (int64_t)immediate;
            update_flags_with_number(result);
            
            *register_x = (byte)result;
            return -1;
        }
        case 0x47: { // MUL $x, $y
            byte* register_x = get_next_as_register();
            byte* register_y = get_next_as_register();
            
            int64_t result = (int64_t)*register_x * (int64_t)*register_y;
            update_flags_with_number(result);
            
            *register_x = (byte)result;
            return -1;
        }
        case 0x48: { // DIV $x, #0
            byte* register_x = get_next_as_register();
            byte immediate = ram.next();
            
            int64_t result = (int64_t)round((double)*register_x / (double)immediate);
            update_flags_with_number(result);
            
            *register_x = (byte)result;
            return -1;
        }
        case 0x49: { // DIV $x, $y
            byte* register_x = get_next_as_register();
            byte* register_y = get_next_as_register();
            
            int64_t result = (int64_t)round((double)*register_x / (double)*register_y);
            update_flags_with_number(result);
            
            *register_x = (byte)result;
            return -1;
        }
        case 0x4a: { // MOD $x, $y
            byte* register_x = get_next_as_register();
            byte* register_y = get_next_as_register();

            uint64_t result = (uint64_t)*register_x % (uint64_t)*register_y;
            update_flags_with_number(result);

            *register_x = (byte)result;
            return -1;
        }
        case 0x4b: { // MOD $x, #0
            byte* register_x = get_next_as_register();
            byte immediate = ram.next();

            uint64_t result = (uint64_t)*register_x % (uint64_t)immediate;
            update_flags_with_number(result);

            *register_x = (byte)result;
            return -1;
        }
        case 0x50: { // CALL [#0]
            uint16_t immediate = ram.next_16bit_immediate();
            
            stack.push_16bit(ram.pc);
            
            ram.pc = immediate;
            
            return -1;
        }
        case 0x51: { // CALL [$x,$y]
            uint16_t addr = bytes_to_uint16(
                *(get_next_as_register()),
                *(get_next_as_register())
            );
            
            stack.push_16bit(ram.pc);
            
            ram.pc = addr;
            
            return -1;
        }
        case 0x52: { // RET
            uint16_t addr = stack.pop_16bit();
            
            ram.pc = addr;
            
            return -1;
        }
        case 0x60: { // STORE [$x,$y], $z
            uint16_t addr = bytes_to_uint16(
                *(get_next_as_register()),
                *(get_next_as_register())
            );
            byte* register_z = get_next_as_register();

            ram.write(addr, *register_z);
            return -1;
        }
        case 0x61: { // STORE [#0], $x
            uint16_t addr = ram.next_16bit_immediate();
            byte* register_x = get_next_as_register();

            ram.write(addr, *register_x);
            return -1;
        }
        case 0x62: { // STORE [$x,$y], #0
            uint16_t addr = bytes_to_uint16(
                *(get_next_as_register()),
                *(get_next_as_register())
            );
            byte immediate = ram.next();

            ram.write(addr, immediate);
            return -1;
        }
        case 0x63: { // STORE [#0], #1
            uint16_t addr = ram.next_16bit_immediate();
            byte immediate = ram.next();

            ram.write(addr, immediate);
            return -1;
        }
        case 0x70: { // FLUSH
            screen.tick();

            return -1;
        }
        case 0xfd: { // HALT $x
            byte* register_x = get_next_as_register();
            
            return *register_x;
        }
        case 0xfe: { // HALT #0
            byte error_code = ram.next();
            return error_code;
        }    
        case 0xff: { // HALT
            return 0;
        }
    }

    raise(Errors::SIGABRT);
    return 0; // Makes the compiler happy, but won't execute because raise() exits the code
}

byte* Arch::get_register_by_address(byte addr)
{
    if (addr == 0xff) return &ALWAYS_ZERO; // Returns a pointer to a new byte which can be modified, but no modification with update it, also, it's always zero
    if(addr >= REGISTERS) raise(Errors::SIGSEGV);
    return &registers[addr];
};

byte* Arch::get_next_as_register() 
{
    return get_register_by_address(ram.next());
}

void Arch::update_flags_with_number(int64_t num)
{
    overflow    = num > 255;
    zero        = num == 0;
    underflow   = num < 0; 
}