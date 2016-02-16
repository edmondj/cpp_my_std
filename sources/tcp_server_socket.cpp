#include <stdexcept>
#include <thread>
#include <chrono>
#include "TCPServerSocket.hpp"

TCPServerSocket::TCPServerSocket()
{}

TCPServerSocket::~TCPServerSocket() {
}

TCPClientSocket* TCPServerSocket::accept() {
    EndPoint dummy;
    return this->accept(dummy);
}

TCPClientSocket* TCPServerSocket::accept(EndPoint& endpoint) {
    socklen_t len = endpoint.get_len();
    Socket::socket_type s = ::accept(this->get_fd(), endpoint.get_sockaddr(), &len);

    if (s == Socket::invalid_socket)
        return nullptr;
    return new TCPClientSocket(s, endpoint);
}
