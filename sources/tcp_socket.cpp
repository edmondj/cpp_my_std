#ifndef _WIN32
# include <netinet/tcp.h>
#endif
#include "TCPSocket.hpp"

#include <iostream>
TCPSocket::TCPSocket()
    : Socket(socket(AF_INET, SOCK_STREAM, 0)) {
}

TCPSocket::TCPSocket(TCPSocket&& other)
    : Socket(std::move(other))
{
}

TCPSocket& TCPSocket::operator=(TCPSocket&& other)
{
    if (this != &other)
    {
        this->Socket::operator=(std::move(other));
    }
    return *this;
}

int TCPSocket::connect(const EndPoint& endPoint) {
    return ::connect(this->get_fd(), endPoint.get_sockaddr(), endPoint.get_len());
}

bool TCPSocket::no_delay() const {
    int val;
    socklen_t size = sizeof(val);

    if (this->get_sock_opt(IPPROTO_TCP, TCP_NODELAY, (char*)&val, &size) != 0)
        return false;
    return val != 0;
}

bool TCPSocket::no_delay(bool value) {
    int val = value;

    if (this->set_sock_opt(IPPROTO_TCP, TCP_NODELAY, (const char*)&val, sizeof(val)) != 0)
        return false;
    return value;
}
