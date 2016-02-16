#pragma once

#include "Socket.hpp"
#include "EndPoint.hpp"

class TCPSocket : public Socket {
  public:
    TCPSocket();
    TCPSocket(TCPSocket&& other);
    TCPSocket& operator=(TCPSocket&& other);

    int connect(const EndPoint& endPoint);

    using Socket::send;
    using Socket::recv;

    bool no_delay() const;
    bool no_delay(bool value);

private:
    TCPSocket(const TCPSocket& other) = delete;
    TCPSocket& operator=(const TCPSocket& other) = delete;
};
