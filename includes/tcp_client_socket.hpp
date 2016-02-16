#pragma once

#include "Socket.hpp"
#include "EndPoint.hpp"

class TCPClientSocket : public Socket {
  public:
    TCPClientSocket(Socket::socket_type fd, const EndPoint& ep);
    ~TCPClientSocket() = default;

    // Moved socket send and recv to public scope
    using Socket::send;
    using Socket::recv;

    const EndPoint& endpoint() const;

    bool no_delay() const;
    bool no_delay(bool value);

  private:
    EndPoint _ep;

    TCPClientSocket() = delete;
    TCPClientSocket(const TCPClientSocket& other) = delete;
    TCPClientSocket(TCPClientSocket&& other) = delete;
    TCPClientSocket& operator=(const TCPClientSocket& other) = delete;
    TCPClientSocket& operator=(TCPClientSocket&& other) = delete;
};