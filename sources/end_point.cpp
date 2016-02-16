#include <stdexcept>
#include <cstring>
#if _WIN32
# include <Ws2tcpip.h>
#else
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
#endif
#include "EndPoint.hpp"

using namespace std;

const unsigned long EndPoint::AddressAny = INADDR_ANY;
const unsigned long AddressLoopback = INADDR_LOOPBACK;
const unsigned long AddressBroadcast = INADDR_BROADCAST;
const unsigned long AddressNone = INADDR_NONE;

EndPoint EndPoint::Any(unsigned short port) {
    return EndPoint(EndPoint::AddressAny, port);
}

EndPoint::EndPoint() {
    memset(&this->_addr, 0, sizeof(this->_addr));
}

EndPoint::EndPoint(const sockaddr_in& addr) {
    memcpy(&this->_addr, &addr, sizeof(this->_addr));
}

EndPoint::EndPoint(unsigned long addr, unsigned short port) {
    this->init(addr, port);
}

EndPoint::EndPoint(const std::string& hostname, unsigned short port) {
    struct addrinfo* ai;
    struct addrinfo* ptr;

    if (getaddrinfo(hostname.c_str(), nullptr, nullptr, &ai) != 0)
        throw runtime_error("Unable to find host: " + hostname);
    ptr = ai;
    while (ptr != nullptr && ptr->ai_family != AF_INET)
        ptr = ptr->ai_next;
    if (ptr == nullptr) {
        freeaddrinfo(ai);
        throw runtime_error("Unable to find host: " + hostname);
    }
    memcpy(&this->_addr, ptr->ai_addr, sizeof(this->_addr));
    this->_addr.sin_port = htons(port);
    freeaddrinfo(ai);
}

EndPoint::EndPoint(const EndPoint& other) {
    memcpy(&this->_addr, &other._addr, sizeof(this->_addr));
}

EndPoint::~EndPoint() {

}

bool EndPoint::operator==(const EndPoint& other) const {
    return memcmp(&this->_addr, &other._addr, sizeof(this->_addr)) == 0;
}

bool EndPoint::operator<(const EndPoint& other) const {
    return this->_addr.sin_addr.s_addr < other._addr.sin_addr.s_addr;
}

socklen_t EndPoint::get_len() const {
    return sizeof(this->_addr);
}

unsigned long EndPoint::get_addr() const {
    return this->_addr.sin_addr.s_addr;
}

string EndPoint::get_addr_str() const {
    char buff[17];

    return string(inet_ntop(AF_INET, (void *)&this->_addr.sin_addr, buff, sizeof(buff)));
}

unsigned short EndPoint::get_port() const {
    return ntohs(this->_addr.sin_port);
}

struct sockaddr* EndPoint::get_sockaddr() {
    return (struct sockaddr*)&this->_addr;
}

const struct sockaddr* EndPoint::get_sockaddr() const {
    return (const struct sockaddr*)&this->_addr;
}

void EndPoint::init(unsigned long addr, unsigned short port)
{
    addr = htonl(addr);
    port = htons(port);
    memset(&this->_addr, 0, sizeof(this->_addr));
    this->_addr.sin_addr.s_addr = addr;
    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = port;
}

std::ostream& operator<<(std::ostream& out, const EndPoint& ep)
{
    return out << ep.get_addr_str() << ':' << ep.get_port();
}
