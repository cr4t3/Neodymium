#pragma once
#include <sys/stat.h>
#include <fstream>

struct BaseArch 
{
    const char* version;

    virtual int tick();

    virtual void input_file(std::ifstream* in, off_t size);
};