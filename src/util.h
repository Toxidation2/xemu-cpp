/*
Project: xemu-cpp
Author: Starman0620
Date: November 5th, 2020
File: util.h
*/

#pragma once
#include <string>

int decodeUpperNybble(char byte);
int decodeLowerNybble(char byte);
int parseInputChar(char in);
void parseOutputChar(int out, std::string *charBuffer);