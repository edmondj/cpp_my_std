/*
Copyright (c) 2016 Julien Edmond

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#if _WIN32

#include <stdexcept>
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "win32_network.hpp"

using namespace std;

my_std::win32_network::win32_network()
{
    if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0) 
    {
        throw runtime_error("Network initialisation failed");
    }
}

my_std::win32_network::~win32_network()
{
    WSACleanup();
}

my_std::win32_network my_std::win32_network::instance;

#endif