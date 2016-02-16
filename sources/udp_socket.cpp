#include <stdexcept>
#include "UDPSocket.hpp"

using namespace std;

UDPSocket::UDPSocket()
    : Socket(socket(AF_INET, SOCK_DGRAM, 0)) {
}

UDPSocket::~UDPSocket() {
}

int UDPSocket::sendto(const char* data, unsigned int length, const EndPoint& endpoint) {
    return ::sendto(this->get_fd(), data, length, 0, endpoint.get_sockaddr(), endpoint.get_len());
}

int UDPSocket::sendto(const std::string& data, const EndPoint& endpoint) {
    return this->sendto(data.c_str(), (unsigned int)data.size(), endpoint);
}

int UDPSocket::recvfrom(char* buf, unsigned int bufsize, EndPoint& endpoint) {
    struct sockaddr_in remaddr;
    socklen_t addrlen = sizeof(remaddr);
    int sent = ::recvfrom(this->get_fd(), buf, bufsize, 0, (struct sockaddr*)&remaddr, &addrlen);
    if (sent >= 0)
        endpoint = EndPoint(remaddr);
    return sent;
}

int UDPSocket::recvfrom(char* buf, unsigned int bufsize) {
    return ::recvfrom(this->get_fd(), buf, bufsize, 0, nullptr, nullptr);
}
