#include <cstring>
#include "casts.h"

uint16_t bytes_to_uint16(byte x, byte y) {
    return ((uint16_t)x << 8) + (uint16_t)y;
}

bytes uint16_to_bytes(uint16_t x) {
    bytes result = new byte[sizeof(x)];
    std::memcpy(result, &x, sizeof(x));

    return result;
}
