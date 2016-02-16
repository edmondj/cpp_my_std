#pragma once
#if _WIN32

# include <Winsock2.h>
# pragma comment(lib,"ws2_32.lib") //Winsock Library

typedef int socklen_t;

class Win32Network {
  private:
    Win32Network();

    ~Win32Network();

    static Win32Network instance;

    WSADATA wsaData;
};

#endif