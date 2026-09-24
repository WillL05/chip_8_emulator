# CHIP-8 Emulator

A functional, cycle-accurate CHIP-8 virtual machine written from scratch in C++. This emulator accurately reproduces the 1970s CHIP-8 architecture, featuring a custom CPU instruction switchboard, dual-clock system, and hardware rendering powered by SFML.

## Features
* **Complete Instruction Set:** Fully implements all 35 original CHIP-8 opcodes, including BCD binary-coded decimal formatting and sprite XOR rendering.
* **Dual-Clock Architecture:** 
  * CPU cycles throttled to historical speeds (~500Hz).
  * Independent Delay and Sound hardware timers locked precisely to 60Hz.
* **Hardware Integration:** 
  * 64x32 monochrome display array upscaled for modern monitors.
  * 16-key hexadecimal keypad mapped to a standard physical keyboard.
* **Modern C++:** Utilizes standard libraries (`<chrono>`, `<thread>`) and robust object-oriented design for memory and hardware encapsulation.

## Dependencies
To compile and run this project, you need:
* A C++ compiler (e.g., GCC/g++)
* **SFML (Simple and Fast Multimedia Library):** Specifically the `graphics`, `window`, and `system` modules.

## Build Instructions
Clone the repository and compile the source files directly using `g++`. Ensure you link the required SFML libraries.

**Linux / macOS:**
```bash
g++ main.cpp chip8.cpp -o chip8 -lsfml-graphics -lsfml-window -lsfml-system
```
**Usage:**
The emulator runs via the command line and requires the filepath to a valid `.ch8` as an argument.

```bash
./chip8 path/to/rom/Tetris.ch8
```
