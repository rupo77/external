#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <sstream>
#include <iomanip>
#include "colors.h"

void print_line(const std::string& line, int time) // this prints a line character by character so it looks good
{
    for (char c : line) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }
    std::cout << std::endl;
}

void log(const std::string& line, int type) // 0 info ; 1 good thing ; 2 error ; 3 injection (red text)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);

    switch (type)
    {
    case 0:
        print_line(std::string(SNOW_WHITE) + "[" + FROST_CYAN + "*" + SNOW_WHITE + "]" + ICE_BLUE + " " + line, 0);
        break;
    case 1:
        print_line(std::string(SNOW_WHITE) + "[" + FROST_BLUE + "+" + SNOW_WHITE + "]" + ICE_BLUE + " " + line, 0);
        break;
    case 2:
        print_line(std::string(SNOW_WHITE) + "[" + FROST_PURPLE + "-" + SNOW_WHITE + "]" + ICE_BLUE + " " + line, 0);
        break;
    case 3:
        print_line(std::string(SNOW_WHITE) + "[" + FROST_CYAN + ">" + SNOW_WHITE + "]" + FROST_BLUE + " " + line, 0);
        break;
    default:
        break;
    }

    // this is better than stacking if statements guys
}

void setup_console_window()
{
    // Set console window size and position
    HWND console = GetConsoleWindow();
    if (console != NULL)
    {
        SetWindowPos(console, 0, 100, 100, 900, 600, SWP_SHOWWINDOW);
    }
    
    // Set console buffer size
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = {120, 40};
    SetConsoleScreenBufferSize(hOut, bufferSize);
    
    // Set console title
    SetConsoleTitleA("FrostWare - Free Roblox External");
}

void print_frostware_banner()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);

    // Setup console window
    setup_console_window();

    std::cout << FROST_CYAN << BOLD << "\n";
    std::cout << "  ================================================\n";
    std::cout << "  ||                                            ||\n";
    std::cout << "  ||          F R O S T W A R E   v2.0          ||\n";
    std::cout << "  ||                                            ||\n";
    std::cout << "  ================================================\n";
    std::cout << RESET << FROST_BLUE << BOLD << "\n        >> Free Roblox External <<\n" << RESET;
    std::cout << ICE_BLUE << "\n           Developed by Rupo!\n" << RESET;
    std::cout << FROST_PURPLE << BOLD << "\n        Join our community: " << FROST_CYAN << UNDERLINE << "discord.gg/getfrost" << RESET << "\n";
    std::cout << SNOW_WHITE << "\n  " << FROST_BLUE << "[" << FROST_CYAN << "SYSTEM" << FROST_BLUE << "]" << ICE_BLUE << " Initializing FrostWare systems..." << RESET << "\n";
    std::cout << SNOW_WHITE << "  " << FROST_BLUE << "[" << FROST_CYAN << "SYSTEM" << FROST_BLUE << "]" << ICE_BLUE << " Loading enhanced features..." << RESET << "\n";
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
