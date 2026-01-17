#if defined(_WIN32) || defined(_WIN64)
    #define K_NT
#elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    #define K_UNIX
#endif

#if __cplusplus < 201703L
#error "C++17 is the standard version. Update to C++17 or higher."
#endif

#if defined(K_NT)
#error "Windows is currently not supported"
#endif

#ifdef K_NT
#error "Windows is currently not supported"
#endif

#define VERSION "Neodymium VM v0.1.0"

#include <stdint.h>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <cstring>
#include <fstream>
#if defined(K_UNIX)
    #include <sys/stat.h>
//#elif defined(K_NT)
    // Include windows code here
#endif

#define REGISTERS 8
// This is an approximation (floored to 5.54 because there is no other ln from 0-254 like it, at most they are 5.53)
#define BYTE_LN 5.54 
#define STACK_ADDRESS 0xcf00
#define NON_SIGNAL_PREFIX 128

typedef uint8_t byte;
typedef byte* bytes;

enum struct Errors : byte{ // May add others just in case
    SIGABRT             =   6,
    SIGSEGV             =   11,
    // Non-signal errors (1-128)
    OS_UNSUPPORTED      =   NON_SIGNAL_PREFIX + 1,
    NO_FILE_ARG         =   NON_SIGNAL_PREFIX + 2,
    FILE_NOT_FOUND      =   NON_SIGNAL_PREFIX + 3,
    FILE_TOO_BIG        =   NON_SIGNAL_PREFIX + 4,
    ERROR_OPENING_FILE  =   NON_SIGNAL_PREFIX + 5,
};

const std::unordered_map<Errors, const char*> errors_dict = {
    {Errors::SIGABRT, "Abnormal termination."},                 {Errors::SIGSEGV, "Segmentation fault."},
    {Errors::OS_UNSUPPORTED, "Your OS isn't supported."},       {Errors::NO_FILE_ARG, "No file argument provided."},
    {Errors::FILE_NOT_FOUND, "File not found."},                {Errors::FILE_TOO_BIG, "File too big."},
    {Errors::ERROR_OPENING_FILE, "Error opening file."},        
};

void raise(Errors code) {
    fprintf(stderr, errors_dict.at(code)); printf("\n");

    byte scode = static_cast<byte>(code);

    if (scode <= 128)
    {
        exit(128 + scode);
    }
    
    exit(scode - 128);
}

uint16_t bytes_to_uint16(byte x, byte y) {
    return ((uint16_t)x << 8) + (uint16_t)y;
}

bytes uint16_to_bytes(uint16_t x) {
    bytes result = new byte[sizeof(x)];
    std::memcpy(result, &x, sizeof(x));

    return result;
}

struct RAM {
    bytes memory;
    uint16_t pc;

    RAM () : pc(0) {
        memory = new byte[0x10000](); // 0x0000 - 0xffff
    };

    ~RAM () {
        delete[] memory;
    };

    byte current() {
        return memory[pc];
    };

    byte next() {
        byte result = memory[pc];
        pc++;
        return result;
    }

    byte get_from_address(uint16_t addr) {
        return memory[addr];
    }

    uint16_t next_16bit_immediate() {
        uint16_t result = bytes_to_uint16(next(), next());
        return result;
    }

    int write(uint16_t address, byte data) {
        memory[address] = data;
        if (memory[address] != data) return 1; // means error, but it's almost impossible this occurrs, i may deprecate this line
        return 0;
    };
};

struct Stack {
    private:
    bytes stack;
    byte sp;
    public:

    Stack (byte* stack_start) : sp(0) {
        stack = stack_start;
    }

    byte peek() {
        return stack[255-sp];
    }

    byte pop() {
        byte temp = stack[255-sp];
        sp--;
        return temp;
    }

    uint16_t pop_16bit() {
        byte x = pop();
        byte y = pop();
        return bytes_to_uint16(y, x);
    }

    void push(byte data) {
        stack[255-sp] = data;
        sp++;   
    }

    void push_16bit(uint16_t data) {
        byte* bdata = uint16_to_bytes(data);
        push(bdata[0]);
        push(bdata[1]);
    }
};

struct CPU {
    private:
    bytes registers; // Will always be defined-size in CPU init (aka registers = new byte[SIZE_HERE]()). Why? It's less mem usage against the old switch case i had.
    byte* get_register_by_addr(byte addr) {
        if (addr == 0xff) return new byte(); // Returns a pointer to a new byte which can be modified, but no modification with update it, also, it's always zero
        if(addr >= REGISTERS) raise(Errors::SIGSEGV);
        return &registers[addr];
    };

    byte* get_next_as_register() {
        return get_register_by_addr(ram.next());
    }

    void update_flags_with_number(int64_t num) {
        if (num > 255) {
            overflow = true;
            zero = false;
            underflow = false;
            return;
        }

        if (num == 0) {
            zero = true;
            overflow = false;
            underflow = false;
            return;
        }

        if (num < 0){
            underflow = true;
            overflow = false;
            zero = false;
            return;
        }

        underflow = false;
        overflow = false;
        zero = false;
        return;
    }
    
    bool zero;
    bool underflow;
    bool overflow;

    public:
    RAM ram;
    Stack stack;


    CPU () : ram(RAM()), stack(Stack(&(ram.memory[STACK_ADDRESS]))), zero(false), underflow(false), overflow(false) {
        registers = new byte[REGISTERS]();
    };

    int tick() { // Gotta make it DRY, cause a lot of repetition in it. (like the register and immediates)
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
                byte* register_y = get_next_as_register();
                byte* register_z = get_next_as_register();

                *register_x = ram.get_from_address(
                    bytes_to_uint16(*register_y, *register_z)
                );
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
                byte* register_x = get_next_as_register();
                byte* register_y = get_next_as_register();
                
                ram.pc = ((uint16_t)*register_x<<8) + (uint16_t)*register_y;
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
                byte* register_x = get_next_as_register();
                byte* register_y = get_next_as_register();
                
                if(zero) ram.pc = ((uint16_t)*register_x<<8) + (uint16_t)*register_y;
                return -1;
            }
            case 0x26: { // JNZ [#0]
                uint16_t immediate = ram.next_16bit_immediate();

                if(!zero) ram.pc = immediate;
                return -1;
            }
            case 0x27: { // JNZ [$x,$y]
                byte* register_x = get_next_as_register();
                byte* register_y = get_next_as_register();
                
                if(!zero) ram.pc = ((uint16_t)*register_x<<8) + (uint16_t)*register_y;
                return -1;
            }
            case 0x28: { // JU [#0]
                uint16_t immediate = ram.next_16bit_immediate();

                if(underflow) ram.pc = immediate;
                return -1;
            }
            case 0x29: { // JU [$x,$y]
                byte* register_x = get_next_as_register();
                byte* register_y = get_next_as_register();
                
                if(underflow) ram.pc = ((uint16_t)*register_x<<8) + (uint16_t)*register_y;
                return -1;
            }
            case 0x2a: { // JNU [#0]
                uint16_t immediate = ram.next_16bit_immediate();

                if(!underflow) ram.pc = immediate;
                return -1;
            }
            case 0x2b: { // JNU [$x,$y]
                byte* register_x = get_next_as_register();
                byte* register_y = get_next_as_register();
                
                if(!underflow) ram.pc = ((uint16_t)*register_x<<8) + (uint16_t)*register_y;
                return -1;
            }
            case 0x2c: { // JO [#0]
                uint16_t immediate = ram.next_16bit_immediate();

                if(overflow) ram.pc = immediate;
                return -1;
            }
            case 0x2d: { // JO [$x,$y]
                byte* register_x = get_next_as_register();
                byte* register_y = get_next_as_register();
                
                if(overflow) ram.pc = ((uint16_t)*register_x<<8) + (uint16_t)*register_y;
                return -1;
            }
            case 0x2e: { // JNO [#0]
                uint16_t immediate = ram.next_16bit_immediate();

                if(!overflow) ram.pc = immediate;
                return -1;
            }
            case 0x2f: { // JNO [$x,$y]
                byte* register_x = get_next_as_register();
                byte* register_y = get_next_as_register();
                
                if(!overflow) ram.pc = ((uint16_t)*register_x<<8) + (uint16_t)*register_y;
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
            case 0x4a: { // PWR $x, #0
                byte* register_x = get_next_as_register();
                byte immediate = ram.next();

                /* C++ returns trash values when a exponent or base is 0 so
                *  it would be better to skip it, it also takes out the possibility of
                *  a = 0, which would make ln(a) = -1 (due to C++ indicating an error)
                */
                if (immediate == 0 || *register_x == 0) {
                    raise(Errors::SIGABRT);
                }

                double pow_size = (double)immediate * log((double)*register_x); // a**b > 255 = b*ln(a) > ln(255)

                overflow    = (pow_size > BYTE_LN);
                underflow   = false;
                zero        = false;

                double result = pow((double)*register_x, (double)immediate); // this gonna overflow heavily, it's unstopable.

                *register_x = (byte)result;
                return -1;
            }
            case 0x4b: { // PWR $x, $y
                byte* register_x = get_next_as_register();
                byte* register_y = get_next_as_register();

                /* C++ returns trash values when a exponent or base is 0 so
                *  it would be better to skip it, it also takes out the possibility of
                *  a = 0, which would make ln(a) = -1 (due to C++ indicating an error)
                */
                if (*register_y == 0 || *register_x == 0) {
                    raise(Errors::SIGABRT);
                }

                double pow_size = (double)*register_y * log((double)*register_x); // a**b > 255 = b*ln(a) > ln(255)

                overflow    = (pow_size > BYTE_LN);
                underflow   = false;
                zero        = false;

                double result = pow((double)*register_x, (double)*register_y); // this gonna overflow heavily, it's unstopable.

                *register_x = (byte)result;
                return -1;
            }
            case 0x4c: { // SQRT $x
                // Thanks Quake III
                
                byte* register_x = get_next_as_register();

                long i;
                float x2, y;

                x2 = (float)*register_x * 0.5F;
                y = (float)*register_x;
                i = * (long*)&y;
                i = 0x5f3759df - (i >> 1);
                y = * (float*)&i;
                y = y * (1.5F - (x2 * y * y));

                *register_x = (byte)round(1/y);

                overflow    = false;
                underflow   = false;
                zero        = (*register_x == 0);
                return -1;
            }
            case 0x4d: { // FSQRT $x
                byte* register_x = get_next_as_register();
                
                int64_t result = round(sqrt((double)*register_x));

                overflow    = false;
                underflow   = false;
                zero        = (result == 0);

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
                    *get_next_as_register(),
                    *get_next_as_register()
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
        return 0; // this wont run never (Cause raise calls exit()), but it makes g++ happy
    }

    int run() {
        while (true) {
            int res = tick();
            if (res != -1) {
                return res;
            }
        }
    }
};

int main(int argc, const char* argv[]) {
    CPU cpu = CPU();

    #if defined(K_NT)
        printf("Windows is currently not supported.");
        raise(Errors::OS_UNSUPPORTED);
    #elif !defined(K_UNIX)
        printf("Your OS is currently not supported");
        raise(Errors::OS_UNSUPPORTED);
    #endif

    if (argc != 2) {
        raise(Errors::NO_FILE_ARG);
    }
    const char* file_name = argv[1];

    if (strcmp(file_name, "--version") == 0 || strcmp(file_name, "-v")){
        printf("%s", VERSION);
    }

    struct stat buffer;
    if (stat(file_name, &buffer) != 0){
        raise(Errors::FILE_NOT_FOUND);
    }

    if (buffer.st_size > 0x10000) {
        raise(Errors::FILE_TOO_BIG);
    }

    std::ifstream in;
    in.open(file_name, std::ios::in | std::ios::binary);

    if (!in.is_open()) {
        raise(Errors::ERROR_OPENING_FILE);
    }

    for (int i = 0; i < buffer.st_size; i++) {
        byte b;
        in.read((char*)&b, sizeof(b));

        cpu.ram.write(i, b);
    }
    
    cpu.run();
}