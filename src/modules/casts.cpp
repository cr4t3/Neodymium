#include <cstring>
#include "casts.h"

uint16_t bytes_to_uint16(byte x, byte y) {
    byte buf[] = {x, y};
    uint16_t result;
    std::memcpy(&result, buf, sizeof(buf));

    return result;
}

bytes uint16_to_bytes(uint16_t x) {
    bytes result = new byte[sizeof(x)];
    std::memcpy(result, &x, sizeof(x));

    return result;
}
