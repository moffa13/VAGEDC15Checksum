#pragma once

#include <cstdint>

namespace MemoryOperations {

    uint16_t lo(uint32_t v);
    uint16_t hi(uint32_t v);
    uint32_t pack(uint16_t low, uint16_t high); 
    uint16_t ror(uint16_t value, uint16_t count);
    uint16_t rol(uint16_t value, uint16_t count);
    uint16_t rolCarry(uint16_t value, uint16_t count, uint16_t* carry);

}