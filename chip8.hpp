#pragma once
#include <cstdint>

class Chip8 {
public:
    // Constructor (runs automatically when the emulator starts)
    Chip8();

    bool loadROM(const char* filename);

    void cycle();

private:
    // The CHIP-8 has exactly 4,096 bytes of memory
    uint8_t memory[4096];

    // 16 general purpose 8-bit registers (V0 to VF)
    uint8_t V[16];

    // The Program Counter tracks where we are in memory
    uint16_t pc;

    uint16_t stack[16];
    uint8_t sp;
    uint8_t sound_timer;
    uint8_t delay_timer;
    uint16_t I;
};

