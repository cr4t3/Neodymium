# Basic Architecture information

## How to implement an architecture?
To implement an architecture, the only requirements are:
* Having files with the same name as the folder but extensions .cpp, .h and .cmake
* The .cpp and .h should have an overwritten struct named "Arch". You can use this template code for the header file:
```cpp
#pragma once
#include <fstream>

struct Arch : BaseArch
{
    Arch();
    const char* version;
    int tick() override;
    void input_file(std::ifstream* in, off_t size) override;
};
```