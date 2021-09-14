/*
Project: xemu-cpp
Author: Starman0620
Date: November 4th, 2020
File: main.cpp

Description:
This file is more or less the 'backbone' of the emulator,
it handles basic setup tasks like parsing command line arguments, selecting an emulator
and reading an actual ROM file.
*/

#include <fstream>
#include <iostream>
#include <filesystem>
#include <termios.h>

#include "x04.h"

int main(int argc, char *argv[])
{
    // Argument parsing
    if(argc <= 1) {
        std::cerr << "Error: no ROM file. Please provide a ROM file" << std::endl;
        return 1;
    }

    // Check if the ROM file exists
    if(!std::filesystem::exists(argv[1])) {
        std::cerr << "Error: invalid or no ROM file." << std::endl;
        return 1;
    }

    // Set the terminal to single char mode (yes; this input pretty much all came from stackoverflow lol)
    struct termios t;
    struct termios t_saved;
    tcgetattr(fileno(stdin), &t);
    t_saved = t;
    t.c_lflag &= (~ICANON & ~ECHO);
    t.c_cc[VTIME] = 0;
    t.c_cc[VMIN] = 1;
    if(tcsetattr(fileno(stdin), TCSANOW, &t) < 0) {
        std::cerr << "Error: failed to set terminal to single character mode." << std::endl;
        return 1;
    }

    std::ifstream romStream(argv[1]);

    // Determine the length of the ROM
    romStream.seekg(0, std::ios::end);
    size_t romLen = romStream.tellg();
    romStream.seekg(0, std::ios::beg);

    // ROM size checking
    if(romLen > 16)
        std::cerr << "Warning: ROM file is too large! Only the first 16 bytes will be loaded." << std::endl;

    // Load the ROM
    unsigned char* ROM = new unsigned char[16];
    romStream.read((char*)ROM, romLen);
    romStream.close();

    // x04 is in a class here for the sake of future proofing; so the x08 emulation can be added in easily
    x04 cpu;
    cpu.execute(ROM);

    // Cleanup; This shouldn't be necessary since the host OS should free up the memory automatically, but still- good practice I guess?
    cpu.cleanup();
    delete ROM;

    // Restore terminal mode.
    if (tcsetattr(fileno(stdin), TCSANOW, &t_saved) < 0) {
        std::cerr << "Error: unable to restore terminal mode" << std::endl;;
        return -1;
    }
}