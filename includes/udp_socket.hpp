#pragma once
#include <string>
#include "Socket.hpp"
#include "EndPoint.hpp"

#if _WIN32
# include "Win32Network.hpp"
#endif

class UDPSocket : public Socket {
  public:
    UDPSocket();
    virtual ~UDPSocket();

    using Socket::bind;
    using Socket::listen;

    int sendto(const char* data, unsigned int length, const EndPoint& endpoint);
    int sendto(const std::string& data, const EndPoint& endpoint);
    int recvfrom(char* buf, unsigned int bufsize, EndPoint& endpoint);
    int recvfrom(char* buf, unsigned int bufsize);
};