#pragma once
#include <string>
#include <iostream>
#include <thread>
#include "colors.h"

void print_line(const std::string& line, int time) // this prints a line character by character so it looks good
{
    for (char c : line) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }
    std::cout << std::endl;
}

void log(const std::string& line, int type) // 0 info ; 1 good thing ; 2 error
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);

    switch (type)
    {
    case 0:
        print_line(std::string(WHITE) + "[" + BLUE + "..." + WHITE + "]" + BLUE + " " + line, 0);
        break;
    case 1:
        print_line(std::string(WHITE) + "[" + GREEN + "+" + WHITE + "]" + BLUE + " " + line, 0);
        break;
    case 2:
        print_line(std::string(WHITE) + "[" + RED + "-" + WHITE + "]" + BLUE + " " + line, 0);
        break;
    default:
        break;
    }

    // this is better than stacking if statements guys
}

template<typename T>
std::string toHexString(T value, bool prefix = false, bool uppercase = false) // this is because we can't do std::hex in custom
{                                                                             // print let me know if u have a better way
    std::stringstream stream;
    if (uppercase)
        stream << std::uppercase;

    if (prefix)
        stream << "0x";

    stream << std::hex << value;
    return stream.str();
}
