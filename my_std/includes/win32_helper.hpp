#pragma once
#ifdef _WIN32

#include <string>
# include <Windows.h>

namespace my_std
{
    namespace _win32
    {
        std::string strerror(DWORD err);
    }
}

#endif