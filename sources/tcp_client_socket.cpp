#ifndef _WIN32
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
#endif

#include "TCPClientSocket.hpp"

TCPClientSocket::TCPClientSocket(Socket::socket_type fd, const EndPoint& ep)
    : Socket(fd), _ep(ep) {
}

const EndPoint & TCPClientSocket::endpoint() const {
    return this->_ep;
}

bool TCPClientSocket::no_delay() const {
    int val;
    socklen_t size = sizeof(val);

    if (this->get_sock_opt(IPPROTO_TCP, TCP_NODELAY, (char*)&val, &size) != 0)
        return false;
    return val != 0;
}

bool TCPClientSocket::no_delay(bool value) {
    int val = value;

    if (this->set_sock_opt(IPPROTO_TCP, TCP_NODELAY, (const char*)&val, sizeof(val)) != 0)
        return false;
    return value;
}
