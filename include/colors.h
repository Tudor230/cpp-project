#ifndef COLORS_H
#define COLORS_H

#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#endif

namespace Colors
{
    // ANSI Color codes
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";

    // Text colors
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";

    // Bright colors
    const std::string BRIGHT_RED = "\033[91m";
    const std::string BRIGHT_GREEN = "\033[92m";
    const std::string BRIGHT_YELLOW = "\033[93m";
    const std::string BRIGHT_BLUE = "\033[94m";
    const std::string BRIGHT_MAGENTA = "\033[95m";
    const std::string BRIGHT_CYAN = "\033[96m";
    const std::string BRIGHT_WHITE = "\033[97m";

    // Background colors
    const std::string BG_RED = "\033[41m";
    const std::string BG_GREEN = "\033[42m";
    const std::string BG_YELLOW = "\033[43m";
    const std::string BG_BLUE = "\033[44m";

    // Initialize colors for Windows
    inline void initColors()
    {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE)
            return;

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode))
            return;

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
#endif
    }

    // Utility functions for colored text
    inline std::string colorize(const std::string &text, const std::string &color)
    {
        return color + text + RESET;
    }

    inline std::string success(const std::string &text)
    {
        return BRIGHT_GREEN + BOLD + text + RESET;
    }

    inline std::string error(const std::string &text)
    {
        return BRIGHT_RED + BOLD + text + RESET;
    }

    inline std::string warning(const std::string &text)
    {
        return BRIGHT_YELLOW + BOLD + text + RESET;
    }

    inline std::string info(const std::string &text)
    {
        return BRIGHT_BLUE + BOLD + text + RESET;
    }

    inline std::string highlight(const std::string &text)
    {
        return BRIGHT_CYAN + BOLD + text + RESET;
    }

    inline std::string title(const std::string &text)
    {
        return BRIGHT_MAGENTA + BOLD + text + RESET;
    }
}

#endif // COLORS_H