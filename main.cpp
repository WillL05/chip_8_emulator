#include <iostream>
#include "chip8.hpp"
#include <fstream>
#include "cstring"
using namespace std;
#include <format>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

Chip8::Chip8() {
    memset(memory, 0, sizeof(memory));
    memset(display, 0, sizeof(display));
    memset(V, 0, sizeof(V));
    I = 0;
    sp = 0;
    delay_timer = 0;
    sound_timer = 0;
    pc = 0x200; 
    std::cout << "CHIP-8 CPU Initialized. Program Counter set to 0x200." << endl;
    for(int i = 0; i<80;i++){
        memory[0x050 + i] = fontset[i];
    }
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
                    memset(display, 0, sizeof(display));
                    break;
                }
                case 0x00EE:{
                    sp--;
                    pc = stack[sp];
                    break;
                }
                
            }
            break;
        }

        case 0x1000:{
            uint16_t last_three_digits = opcode & 0x0FFF;
            pc = last_three_digits;
            break;
        }

        case 0x2000:{
            uint16_t last_three_digits = opcode & 0x0FFF;
            stack[sp] = pc;
            sp++;
            pc = last_three_digits;
            break;
        }

        case 0x3000:{
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint16_t last_two_digits = opcode & 0x0FF;
            if(V[second_digit] == last_two_digits){
                pc += 2;
            }
            break;
        }

        case 0x4000:{
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint16_t last_two_digits = opcode & 0x0FF;
            if(V[second_digit] != last_two_digits){
                pc+=2;
            }
            break;
        }

        case 0x5000:{
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint8_t third_digit = (opcode & 0x00F0) >> 4;
            if(V[second_digit] == V[third_digit]){
                pc +=2;
            }
            break;
        }

        case 0x6000:{
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint16_t last_two_digits = opcode & 0x0FF;
            V[second_digit] = last_two_digits;
            break;
        }

        case 0x7000:{
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint16_t last_two_digits = opcode & 0x0FF;
            V[second_digit] += last_two_digits;
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
                    if( V[second_digit] >= V[third_digit]){
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
                    V[second_digit] = V[second_digit] >> 1;
                    break;
                }
                case 0x0007:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    uint8_t third_digit = (opcode & 0x00F0) >> 4;
                    if( V[third_digit] >= V[second_digit]){
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
                    uint8_t overflow_check = (V[second_digit] & 0x80) >> 7;
                    V[0x0F] = overflow_check;
                    V[second_digit] = V[second_digit] << 1;
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
            if(V[second_digit] != V[third_digit]){
                pc += 2;
            }
            break;
        }
            
        case 0xA000:{
            uint16_t last_three_digits = opcode & 0x0FFF;
            I = last_three_digits;
            break;
        }

        case 0xB000:{
            uint16_t last_three_digits = opcode & 0x0FFF;
            pc = last_three_digits + V[0];
            break;
        }
            
        case 0xC000:{
            uint8_t second_digit = (opcode & 0x0F00) >> 8;
            uint16_t last_two_digits = opcode & 0x0FF;
            V[second_digit] = (rand() % 256) & last_two_digits;
            break;
        }

        case 0xD000:{
            
            uint8_t second_digit = (opcode & 0x0F00) >> 8; //X
            uint8_t third_digit = (opcode & 0x00F0) >> 4; // Y
            uint8_t last_digit = (opcode & 0x000F); // N

            int Xcoords = V[second_digit] % 64;
            int Ycoords = V[third_digit] % 32;
            uint8_t buf;
            V[0x0F] = 0;
            for(int i = 0; i < last_digit; i++){
                buf = memory[I + i];
                for(int x = 0; x < 8; x++){
                    uint8_t current_bit = (buf & (0x80 >> x));
                    if (current_bit != 0){
                        int index = (((Ycoords+i) %32) * 64) + ((Xcoords+x)%64);

                        if(display[index] == 1){
                            V[0x0F] = 1;
                        }
                        display[index] ^= 1;
                    }
                }

            }
            break;
        }
        case 0xE000:{
            switch(opcode & 0x00FF){
                case 0x009E:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    if (keypad[V[second_digit]] == 1){
                        pc += 2;
                    }
                    break;
                }
                case 0x00A1:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    if (keypad[V[second_digit]] == 0){
                        pc += 2;
                    }
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
                    V[second_digit] = delay_timer;
                    break;
                }
                case 0x000A:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    bool key_pressed = false;
                    for(int i =0; i < 16; i++){
                        if(keypad[i] == 1){
                            V[second_digit] = i;
                            key_pressed = true;
                            break;
                        }
                    }
                    if(!key_pressed){
                        pc -= 2;
                    }
                    break;

                }
                case 0x0015:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    delay_timer = V[second_digit];
                    break;
                }
                case 0x0018:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    sound_timer = V[second_digit];
                    break;
                }
                case 0x001E:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    I += V[second_digit];
                    break;
                }
                case 0x0029:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    uint8_t character = V[second_digit];
                    I = 0x050 + (character *5); 
                    break;
                }
                case 0x0033:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    int tens = (V[second_digit] /10) % 10;
                    int ones = V[second_digit] % 10;
                    int hundreds = (V[second_digit] /100);
                    for(int i = 0; i <3;i++){
                        if (i == 0){
                            memory[I+i] = hundreds;
                        }
                        else if(i == 1){
                            memory[I+i] = tens;
                        }
                        else{
                            memory[I+i] = ones;
                        }
                        
                    }
                    break;
                }
                case 0x0055:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;

                    for(int i = 0; i<= second_digit;i++){
                        memory[I+i] = V[i];
                    }
                    break;
                }
                case 0x0065:{
                    uint8_t second_digit = (opcode & 0x0F00) >> 8;
                    for(int i = 0; i<= second_digit;i++){
                        V[i] = memory[I+i];
                    }
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

void Chip8::tickTimers(){
    if(delay_timer > 0){
        delay_timer--;
    } 
    if(sound_timer > 0){
        sound_timer--;
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ROM_Filename>" << std::endl;
        return 1; // Exit with an error code
    }
    Chip8 myEmulator;
    myEmulator.loadROM(argv[1]);

    const int scale = 15;
    sf::RenderWindow window(sf::VideoMode({64*scale,32*scale}),"CHIP-8 Emulator");
    auto last_timer_time = chrono::high_resolution_clock::now();
    while (window.isOpen()){
        uint8_t* keypad = myEmulator.access_keypad();
        sf::Event event;
        while(window.pollEvent(event)){
    
            if(event.type == sf::Event::Closed){
                window.close();
            }
            else if(event.type == sf::Event::KeyPressed ){
                switch(event.key.code){
                    case sf::Keyboard::Num1: 
                        keypad[0x1] = 1;
                        break;
                    case sf::Keyboard::Num2: 
                        keypad[0x2] = 1;
                        break;
                    case sf::Keyboard::Num3: 
                        keypad[0x3] = 1;
                        break;
                    case sf::Keyboard::Num4: 
                        keypad[0xC] = 1;
                        break;
                    
                    case sf::Keyboard::Q: 
                        keypad[0x4] = 1;
                        break;
                    case sf::Keyboard::W: 
                        keypad[0x5] = 1;
                        break;
                    case sf::Keyboard::E: 
                        keypad[0x6] = 1;
                        break;
                    case sf::Keyboard::R: 
                        keypad[0xD] = 1;
                        break;

                    case sf::Keyboard::A: 
                        keypad[0x7] = 1;
                        break;
                    case sf::Keyboard::S: 
                        keypad[0x8] = 1;
                        break;
                    case sf::Keyboard::D:
                        keypad[0x9] = 1;
                        break;
                    case sf::Keyboard::F: 
                        keypad[0xE] = 1;
                        break;

                    case sf::Keyboard::Z: 
                        keypad[0xA] = 1;
                        break;
                    case sf::Keyboard::X: 
                        keypad[0x0] = 1;
                        break;
                    case sf::Keyboard::C: 
                        keypad[0xB] = 1;
                        break;
                    case sf::Keyboard::V: 
                        keypad[0xF] = 1;
                        break;
                }
            }
            else if(event.type == sf::Event::KeyReleased){

                switch(event.key.code){
                    case sf::Keyboard::Num1: 
                        keypad[0x1] = 0;
                        break;
                    case sf::Keyboard::Num2: 
                        keypad[0x2] = 0;
                        break;
                    case sf::Keyboard::Num3: 
                        keypad[0x3] = 0;
                        break;
                    case sf::Keyboard::Num4: 
                        keypad[0xC] = 0;
                        break;
                    
                    case sf::Keyboard::Q: 
                        keypad[0x4] = 0;
                        break;
                    case sf::Keyboard::W: 
                        keypad[0x5] = 0;
                        break;
                    case sf::Keyboard::E: 
                        keypad[0x6] = 0;
                        break;
                    case sf::Keyboard::R: 
                        keypad[0xD] = 0;
                        break;

                    case sf::Keyboard::A: 
                        keypad[0x7] = 0;
                        break;
                    case sf::Keyboard::S: 
                        keypad[0x8] = 0;
                        break;
                    case sf::Keyboard::D:
                        keypad[0x9] = 0;
                        break;
                    case sf::Keyboard::F: 
                        keypad[0xE] = 0;
                        break;

                    case sf::Keyboard::Z: 
                        keypad[0xA] = 0;
                        break;
                    case sf::Keyboard::X: 
                        keypad[0x0] = 0;
                        break;
                    case sf::Keyboard::C: 
                        keypad[0xB] = 0;
                        break;
                    case sf::Keyboard::V: 
                        keypad[0xF] = 0;
                        break;
                }

            }
        }
            myEmulator.cycle();
        auto curr_timer_time = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = curr_timer_time - last_timer_time;
        auto comparison = chrono::microseconds(16667);
        if(elapsed >= comparison ){
            myEmulator.tickTimers();
            last_timer_time = curr_timer_time;
            window.clear(sf::Color::Black);
            sf::RectangleShape Rectangle;
            Rectangle.setSize(sf::Vector2f(scale,scale));
            Rectangle.setFillColor(sf::Color::White);
            uint8_t* display = myEmulator.access_display();
            int X;
            int Y;
            for(int i = 0;i<=2047;i++){
                if(display[i] == 1){
                    X = (i % 64) * scale;
                    Y = (i / 64) * scale;
                    Rectangle.setPosition(X,Y);
                    window.draw(Rectangle);
                }
            }
            window.display();
        }
        std::this_thread::sleep_for (std::chrono::microseconds(2000));
    }
    return 0;
}

