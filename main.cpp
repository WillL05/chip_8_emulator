#include <iostream>
#include "chip8.hpp"
#include <fstream>
#include "cstring"
using namespace std;

Chip8::Chip8() {
    pc = 0x200; 
    std::cout << "CHIP-8 CPU Initialized. Program Counter set to 0x200." << endl;
}

bool Chip8::loadROM(const char* filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate );
    if (!file){
        cerr << "Error opening file";
        return false;
    }
    
    auto end_pos = file.tellg();
    cout << "Address of pointer: " << end_pos << endl;
    file.seekg(0, ios::beg);
    file.read((char*)&memory[pc],end_pos);
    
    // // Loop through the first 16 bytes of the game
    // for (int i = 0; i < 16; i++) {
    //     // We add 0x200 to 'i' because that is where the game starts!
    //     // The + prefix forces C++ to print the actual number instead of trying to read it as a character
    //     std::cout << std::hex << +(memory[0x200 + i]) << " ";
    // }
    // std::cout << std::endl;
    return true;
}   

uint16_t Chip8::cycle(){
    uint16_t opcode;
    uint16_t first_byte = memory[pc];
    opcode = first_byte << 8;
    opcode = opcode | memory[pc+1];
    pc+=2; 
    return opcode;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ROM_Filename>" << std::endl;
        return 1; // Exit with an error code
    }
    Chip8 myEmulator;
    myEmulator.loadROM(argv[1]);
    myEmulator.cycle();
    return 0;
}