#include "errors.h"
#include <unordered_map>
#include <iostream>

const std::unordered_map<Errors, const char*> errors_dict = {
    {Errors::SIGABRT, "Abnormal termination."},                 {Errors::SIGSEGV, "Segmentation fault."},
    {Errors::SIGKILL, "Signal killed."},
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