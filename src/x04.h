/*
Project: xemu-cpp
Author: Starman0620
Date: November 4th, 2020
File: x04.h
*/

#pragma once 

class x04
{
public:
    void execute(unsigned char* ROM);
    void cleanup();
private:
    void doInstruction(unsigned char byte);
};  