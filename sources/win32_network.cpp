#if _WIN32

#include <stdexcept>
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "Win32Network.hpp"

using namespace std;

Win32Network::Win32Network() {
    if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0) {
        throw runtime_error("Network initialisation failed");
    }
}

Win32Network::~Win32Network() {
    WSACleanup();
}

Win32Network Win32Network::instance;


#endif