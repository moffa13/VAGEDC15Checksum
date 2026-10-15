#include "BitArithmetics.h"

namespace MemoryOperations {

    uint16_t lo(uint32_t v) { return (uint16_t)v; }
    uint16_t hi(uint32_t v) { return (uint16_t)(v >> 16); }
    uint32_t pack(uint16_t low, uint16_t high) { return ((uint32_t)high << 16) | low; }

    uint16_t ror(uint16_t x, uint16_t n)
    {
        n &= 15;
        if (!n) return x;

        return (x >> n) | ((x & ((1u << n) - 1)) << (16 - n));
    }

    uint16_t rol(uint16_t x, uint16_t n)
    {
        n &= 15;
        if (!n) return x;

        return (x << n) | (x >> (16 - n));
    }

    uint16_t rolCarry(uint16_t value, uint16_t count, uint16_t* carry)
    {
        count &= 15;
        if (!count) {
            *carry = 0;
            return value;
        }

        *carry = (value >> (16 - count)) & 1;
        return (value << count) | (value >> (16 - count));
    }

}
