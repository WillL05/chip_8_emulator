#include <iostream>
#include "chip8.hpp"
#include <fstream>
#include "cstring"
using namespace std;
#include <format>

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

void Chip8::cycle(){
    uint16_t opcode;
    uint16_t first_byte = memory[pc];
    opcode = first_byte << 8;
    opcode = opcode | memory[pc+1];
    pc+=2; 
    switch(opcode & 0xF000){
        case 0x0000: {
            switch (opcode & 0x0FFF){
                case 0x00E0:{
                    break;
                }
                case 0x00EE:{
                    
                    break;
                }
                
            }
        }

        case 0x1000:{
            uint16_t last_three_digits = opcode & 0x0FFF;
            break;
        }

        case 0x2000:{
            uint16_t last_three_digits = opcode & 0x0FFF;
            break;
        }

        case 0x3000:{
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint16_t last_two_digits = opcode & 0x0FF;
            break;
        }

        case 0x4000:{
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint16_t last_two_digits = opcode & 0x0FF;
            break;
        }

        case 0x5000:{
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint8_t third_digit = (opcode & 0x00F0) >> 4;
            break;
        }

        case 0x6000:{
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint16_t last_two_digits = opcode & 0x0FF;
            break;
        }

        case 0x7000:{
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint16_t last_two_digits = opcode & 0x0FF;
            break;
        }

        case 0x8000:{
            switch(opcode & 0x000F){
                case 0x0000:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    uint8_t third_digit = (opcode & 0x00F0) >> 4;
                    V[second_digit] = V[third_digit];
                    break;
                }
                case 0x0001:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    uint8_t third_digit = (opcode & 0x00F0) >> 4;
                    V[second_digit] = V[second_digit] | V[third_digit];
                    break;
                }
                case 0x0002:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    uint8_t third_digit = (opcode & 0x00F0) >> 4;
                    V[second_digit] = V[second_digit] & V[third_digit];
                    break;
                }
                case 0x0003:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    uint8_t third_digit = (opcode & 0x00F0) >> 4;
                    V[second_digit] = V[second_digit] ^ V[third_digit];
                    break;
                }
                case 0x0004:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    uint8_t third_digit = (opcode & 0x00F0) >> 4;
                    uint16_t buf = V[second_digit] + V[third_digit];
                    if(buf > 255){
                        V[0x0F] = 1;
                    }
                    else{
                        V[0x0F] = 0;
                    }
                    V[second_digit] = buf;
                    break;
                }
                case 0x0005:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    uint8_t third_digit = (opcode & 0x00F0) >> 4;
                    if( V[second_digit] > V[third_digit]){
                        V[0x0F] = 1;
                    }
                    else{
                        V[0x0F] = 0;
                    }
                    V[second_digit] = V[second_digit] - V[third_digit];
                    break;
                }
                case 0x0006:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    uint8_t third_digit = (opcode & 0x00F0) >> 4;
                    uint8_t overflow_check = V[second_digit] & 0x01;
                    V[0x0F] = overflow_check;
                    V[second_digit] = second_digit >> 1;
                    break;
                }
                case 0x0007:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    uint8_t third_digit = (opcode & 0x00F0) >> 4;
                    if( V[third_digit] > V[second_digit]){
                        V[0x0F] = 1;
                    }
                    else{
                        V[0x0F] = 0;
                    }
                    V[second_digit] = V[third_digit] - V[second_digit];
                    break;
                    
                }
                case 0x000E:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    uint8_t third_digit = (opcode & 0x00F0) >> 4;
                    uint8_t overflow_check = V[second_digit] & 0x80;
                    V[0x0F] = overflow_check;
                    V[second_digit] = second_digit << 1;
                    break;
                }
                default:
                    std::cerr << "Unknown instruction"<< std::endl;
                    break;
                
            }
            break;
        }

        case 0x9000:{
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint8_t third_digit = (opcode & 0x00F0) >> 4;
            break;
        }
            
        case 0xA000:{
            uint16_t last_three_digits = opcode & 0x0FFF;
            break;
        }

        case 0xB000:{
            uint16_t last_three_digits = opcode & 0x0FFF;
            break;
        }
            
        case 0xC000:{
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint16_t last_two_digits = opcode & 0x0FF;
            break;
        }

        case 0xD000:{
            
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint8_t third_digit = (opcode & 0x00F0) >> 4;
            uint8_t last_digit = (opcode & 0x000F);
            break;
        }

        case 0xE000:{
            switch(opcode & 0x00FF){
                case 0x009E:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    break;
                }
                case 0x00A1:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    break;
                }
                default:
                    std::cerr << "Unknown instruction"<< std::endl;
                    break;
            }

            break;
        }

        case 0xF000:{
            switch (opcode & 0x00FF){
                case 0x0007:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    break;
                }
                case 0x000A:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    break;

                }
                case 0x0015:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    break;
                }
                case 0x0018:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    break;
                }
                case 0x001E:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    break;
                }
                case 0x0029:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    break;
                }
                case 0x0033:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    break;
                }
                case 0x0055:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    break;
                }
                case 0x0065:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    break;
                }
                default:
                    std::cerr << "Unknown instruction"<< std::endl;
                    break;
            }
            break;
        }
        
        default:
            std::cerr << "Unknown instruction"<< std::endl;
            break;
    }
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

