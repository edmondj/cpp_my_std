#pragma once

#include <iostream>
#include <string>
#if _WIN32
# include "Win32Network.hpp"
#else
# include <netinet/ip.h>
#endif

class EndPoint {
  public:
    static const unsigned long AddressAny;
    static const unsigned long AddressLoopback;
    static const unsigned long AddressBroadcast;
    static const unsigned long AddressNone;

    static EndPoint Any(unsigned short port);

    EndPoint();
    EndPoint(const struct sockaddr_in& addr);
    EndPoint(unsigned long addr, unsigned short port);
    EndPoint(const std::string& hostname, unsigned short port);
    EndPoint(const EndPoint& other);
    ~EndPoint();

    bool operator==(const EndPoint& other) const;
    bool operator<(const EndPoint& other) const;

    socklen_t get_len() const;
    unsigned long get_addr() const;
    std::string get_addr_str() const;
    unsigned short get_port() const;
    struct sockaddr* get_sockaddr();
    const struct sockaddr* get_sockaddr() const;

  private:
    void init(unsigned long addr, unsigned short port);

    struct sockaddr_in _addr;
};

std::ostream& operator<<(std::ostream& out, const EndPoint& ep);