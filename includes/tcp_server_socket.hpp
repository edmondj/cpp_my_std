#pragma once

#include "TCPSocket.hpp"
#include "Socket.hpp"
#include "TCPClientSocket.hpp"
#include "EndPoint.hpp"

class TCPServerSocket : public TCPSocket {
  public:
    TCPServerSocket();
    ~TCPServerSocket();

    TCPClientSocket* accept();
    TCPClientSocket* accept(EndPoint& endpoint);

    using Socket::listen;
    using Socket::bind;

  private:
    TCPServerSocket(const TCPServerSocket& other) = delete;
    TCPServerSocket(TCPServerSocket&& other) = delete;
    TCPServerSocket& operator=(const TCPServerSocket& other) = delete;
    TCPServerSocket& operator=(TCPServerSocket&& other) = delete;
};
