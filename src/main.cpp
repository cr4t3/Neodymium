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

#define VERSION "Neodymium VM v0.2.2 (build 5)"

#include <iostream>
#include <fstream>
#include <cstring>
#if defined(K_UNIX)
    #include <sys/stat.h>
//#elif defined(K_NT)
    // Include windows code here
#endif

#include "modules/cpu.h"
#include "modules/errors.h"
#include "modules/screen.h"

int main(int argc, const char* argv[]) {
    
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
    
    if (strcmp(file_name, "--version") == 0 || strcmp(file_name, "-v") == 0){
        printf("%s\n", VERSION);
        exit(0);
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
    
    CPU cpu = CPU();
    for (int i = 0; i < buffer.st_size; i++) {
        byte b;
        in.read((char*)&b, sizeof(b));
        
        cpu.ram.write(i, b);
    }
    
    cpu.run();
}