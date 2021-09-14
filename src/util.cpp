/*
Project: xemu-cpp
Author: Starman0620
Date: November 5th, 2020
File: util.h

Description:
This file holds some misc functions that both x04 and x08 use
just so they don't have to be implemented in each emulator
*/

#include <string>

#include "util.h"

int decodeLowerNybble(char byte) {
    return (byte & 0x0F);
}

int decodeUpperNybble(char byte) {
    return ((byte & 0xF0) >> 4);
}

// I'm not really sure if x08 will use this or not but it's here just in case,
// It'll be moved to the x04 file if x08 doesn't use it though
int parseInputChar(char in) {
    int retVal = 0; // Return value
    switch(in) {
        case ' ': retVal = 0; break;
        case 'l': retVal = 1; break;
        case 'o': retVal = 2; break;
        case 'r': retVal = 3; break;
        case 'h': retVal = 4; break;
        case 'd': retVal = 5; break;
        case 'i': retVal = 6; break;
        case 'n': retVal = 7; break;
        case 'm': retVal = 8; break;
        case 'g': retVal = 9; break;
        case 'a': retVal = 10; break;
        case 'b': retVal = 11; break;
        case 'c': retVal = 12; break;
        case 'f': retVal = 13; break;
        case 'e': retVal = 14; break;
        case 't': retVal = 15; break;
        default:  retVal = -15; break; // -15 == Skip
    }
    return retVal;
}

void parseOutputChar(int out, std::string *charBuffer) {
    switch(out) {
        case 0: charBuffer->append(" "); break;
        case 1: charBuffer->append("L"); break;
        case 2: charBuffer->append("O"); break;
        case 3: charBuffer->append("R"); break;
        case 4: charBuffer->append("H"); break;
        case 5: charBuffer->append("D"); break;
        case 6: charBuffer->append("I"); break;
        case 7: charBuffer->append("N"); break;
        case 8: charBuffer->append("M"); break;
        case 9: charBuffer->append("G"); break;
        case 10: charBuffer->append("A"); break;
        case 11: charBuffer->append("B"); break;
        case 12: charBuffer->append("C"); break;
        case 13: charBuffer->append("F"); break;
        case 14: charBuffer->append("E"); break;
        case 15: charBuffer->append("T"); break;
    }
}