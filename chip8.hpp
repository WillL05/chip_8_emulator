#pragma once
#include <cstdint>

class Chip8 {
public:
    // Constructor (runs automatically when the emulator starts)
    Chip8();

    bool loadROM(const char* filename);

    uint16_t cycle();

private:
    // The CHIP-8 has exactly 4,096 bytes of memory
    uint8_t memory[4096];

    // 16 general purpose 8-bit registers (V0 to VF)
    uint8_t V[16];

    // The Program Counter tracks where we are in memory
    uint16_t pc;
};

