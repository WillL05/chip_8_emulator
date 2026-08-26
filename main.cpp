#include <iostream>
#include "chip8.hpp"
#include <fstream>
#include "cstring"
using namespace std;

Chip8::Chip8() {
    pc = 0x200; 
    std::cout << "CHIP-8 CPU Initialized. Program Counter set to 0x200." << endl;
}

bool Chip8::loadROM(const char* filename){
    std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate );
    if (!file){
        cerr << "Error opening file";
        return false;
    }
    string s;
    file >> s;
    auto end_pos = file.tellg();
    cout << "Address of pointer: " << end_pos << endl;
    file.seekg(0, ios::beg);
    
    size_t str_length;
    file.read(reinterpret_cast<char *>(&str_length),sizeof(str_length));

    char *buffer = new char[str_length + 1];
    file.read(buffer,str_length);
    buffer[str_length] = '\0';
    Chip8.memory[pc];
}   


int main() {
    
    Chip8 myEmulator;
    
    return 0;
}