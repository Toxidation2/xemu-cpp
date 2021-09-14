/*
Project: xemu-cpp
Author: Starman0620
Date: November 4th, 2020
File: x04.cpp

Description:
This is the heart of x04 emulation. It handles absolutely everything specific to x04
*/

#include <iostream>

#include "x04.h"
#include "util.h"

// Data stuff
char* RAM = new char[14];   // 14 bytes of RAM; While x04 technically has 16 bytes, $E and $F are reserved for I/O.

// Registers
char A = 0;            // ALU A register
char B = 0;            // ALU B register
char O = 0;            // ALU output
char P = 0;            // Program register
unsigned short pc = 0; // Program counter

// Misc emulation variables
char charIn;
std::string charBuffer;
std::string outputString;
int inputIO = 0;
int outputIO = 0;
bool outputNew = false;
bool brk = false;
bool jmp = false;

void x04::execute(unsigned char* ROM) {
    std::streambuf *pbuf = std::cin.rdbuf();

    // Main loop
    while(!brk) {
        // Execute the current instruction
        doInstruction(ROM[pc]);

        // Print debug info
        printf("DEBUG: PC=%d, ROM[PC]=%Xh, A=%d, B=%d, O=%d, BRK=%s, P=%d", pc, ROM[pc], A, B, O, brk ? "true" : "false", P);
        printf(" OUT=%d, IN=%d, OUT_NEW=%s\n", outputIO, inputIO, outputNew ? "true" : "false");
        printf("DEBUG: RAM=");
        for(int i=0;i<14;i++) // A loop is used to print each byte individually since as far as I know, you can't print a char* as hex
            printf("%02X ", RAM[i]);
        printf("\n");
        printf("DEBUG: ROM=");
        for(int i=0;i<16;i++)
            printf("%02X ", ROM[i]);
        printf("\n");
        printf("DEBUG: outputString=%s\n", outputString.c_str());

        if(!jmp) pc++;
        pc=pc%16;

        // Receive input from the user
        std::cout << "DEBUG: Enter a character to add for input and/or press ENTER to step\nInput>";
        if (pbuf->sgetc() == EOF) brk = true;
        charIn = pbuf->sbumpc();
        std::cout << charIn << std::endl;

        // Input emulation
        charIn = tolower(charIn);
        int charVal = parseInputChar(charIn);
        if(charVal == -15) goto skipInput;
        inputIO = charVal;
        skipInput:

        // Output emulation
        if(outputNew) {
            parseOutputChar(outputIO, &charBuffer);
            outputNew = false;
        }
        outputString.clear();
        for(int i = 0; i < (int)charBuffer.length(); i++)
            outputString += charBuffer.c_str()[i];

        jmp = false;
    }
}

void x04::doInstruction(unsigned char byte) {
    int upperNybble = decodeUpperNybble(byte);
    int target = decodeLowerNybble(byte); // I declare target here just to make the compiler shut up with its warnings, this is technically slower though.
    switch(upperNybble) {
        case 0: brk = true; break;                // BRK
        case 1: O=(A+B)%16; break;                // ADD
        case 2: O=abs(A-B)%16; break;             // SUB
        case 3:                                   // LAM
        // Input/Output checks
        if(target == 15) A=inputIO;
        else if(target == 14) A=outputIO;
        else A=RAM[target];
        break;
        case 4: A=decodeLowerNybble(byte); break; // LDA
        case 5:                                   // ZJP
        if(A==0) { 
            pc = decodeLowerNybble(byte);
            jmp = true;
        }
        return;
        break;
        case 6: B=decodeLowerNybble(byte); break; // LDB
        case 7:                                   // LBM
        if(target == 15) A=inputIO;
        else if(target == 14) A=outputIO;
        else B=RAM[target];
        break;
        case 8: pc=decodeLowerNybble(byte);       // JMP 
        jmp = true;
        return;
        break;
        case 9:                                   // SOM
        if(target == 15) inputIO=O;
        else if(target==14) {
            outputIO=O;
            outputNew = true;
        }
        else RAM[target]=O;
        break;
        case 10: O=(A&B)%16; break;               // AND
        case 11: O=(A|B)%16; break;               // OR
        case 12: O=(A^B)%16; break;               // XOR
        case 13: O=abs(~(A&B))%16; break;         // NND
        case 14: O=abs(~(A|B))%16; break;         // NOR
        case 15: O=abs(~(A^B))%16; break;         // XNR
        default:
        printf("Error: invalid opcode: %X\n", upperNybble);
        brk = true;
        break;
    }
}

void x04::cleanup() {
    delete RAM;
}