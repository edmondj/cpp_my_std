#include <stdexcept>
#if _WIN32
# include <Winsock2.h>
#else
# include <unistd.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <poll.h>
# include <errno.h>
#endif
#include "Socket.hpp"
#include <cstring>

using namespace std;

Socket::const_socket_type Socket::invalid_socket =
#if _WIN32
    INVALID_SOCKET
#else
    - 1
#endif
    ;

#ifdef _WIN32
std::string Socket::get_last_error() {
    int error = WSAGetLastError();
    LPSTR s;

    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error, 0, (LPSTR)&s, 0, nullptr) == 0)
        return "Failed to retrieve error";
    string sent(s);

    LocalFree(s);
    return sent;
}
#else
std::string Socket::get_last_error() {
    return string(strerror(errno));
}
#endif

Socket::Socket(Socket&& other)
    : _fd(other._fd)
{
    other._fd = -1;
}

Socket& Socket::operator=(Socket&& other) {
    if (this != &other)
    {
        this->close();
        this->_fd = other._fd;
        other._fd = -1;
    }
    return *this;
}

int Socket::close() {
#if _WIN32
    return ::closesocket(this->_fd);
#else
    ::shutdown(this->_fd, SHUT_RDWR);
    return ::close(this->_fd);
#endif
}

Socket::~Socket() {
    this->close();
}

bool Socket::reuse_addr() const {
    int val;
    socklen_t size = sizeof(val);

    if (this->get_sock_opt(SOL_SOCKET, SO_REUSEADDR, &val, &size) != 0)
        return false;
    return val != 0;
}

bool Socket::reuse_addr(bool value) {
    int val = value;

    if (this->set_sock_opt(SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) != 0)
        return false;
    return value;
}

int Socket::rcv_buf_size() const {
    int val;
    socklen_t size = sizeof(val);

    if (this->get_sock_opt(SOL_SOCKET, SO_RCVBUF, &val, &size) != 0)
        return -1;
    return val;
}

int Socket::rcv_buf_size(int value) {
    if (this->set_sock_opt(SOL_SOCKET, SO_RCVBUF, &value, sizeof(value)) != 0)
        return -1;
    return value;
}

int Socket::snd_buf_size() const {
    int val;
    socklen_t size = sizeof(val);

    if (this->get_sock_opt(SOL_SOCKET, SO_SNDBUF, &val, &size) != 0)
        return -1;
    return val;
}

int Socket::snd_buf_size(int value) {
    if (this->set_sock_opt(SOL_SOCKET, SO_SNDBUF, &value, sizeof(value)) != 0)
        return -1;
    return value;
}

int Socket::get_sock_opt(int level, int optname, void* optval, socklen_t* optlen) const {
    return getsockopt(this->_fd, level, optname, (char*)optval, optlen);
}

int Socket::set_sock_opt(int level, int optname, const void * optval, socklen_t optlen) {
    return setsockopt(this->_fd, level, optname, (char*)optval, optlen);
}

#ifdef _WIN32
int Socket::poll(short& events, int timeout)
{
    WSAPOLLFD fd;

    fd.events = events;
    fd.fd = this->get_fd();
    fd.revents = 0;
    int ret = WSAPoll(&fd, 1, timeout);
    events = fd.revents;
    return ret;
}
#else
int Socket::poll(short& events, int timeout)
{
    struct pollfd fd;

    fd.events = events;
    fd.fd = this->get_fd();
    fd.revents = 0;
    int ret = ::poll(&fd, 1, timeout);
    events = fd.revents;
    return ret;
}
#endif

Socket::Socket(Socket::socket_type fd)
    : _fd(fd) {
    if (this->_fd <= 0) {
        throw runtime_error("Invalid file descriptor");
    }
}

Socket::const_socket_type Socket::get_fd() const {
    return this->_fd;
}

Socket::socket_type Socket::get_fd() {
    return this->_fd;
}

int Socket::bind(const EndPoint& endpoint) {
    return ::bind(this->get_fd(), endpoint.get_sockaddr(), endpoint.get_len());
}

int Socket::listen(int backlog) {
    return ::listen(this->get_fd(), backlog);
}

int Socket::send(const char * str, unsigned int len) {
    return ::send(this->_fd, str, len, 0);
}

int Socket::send(const std::string & str) {
    return this->send(str.c_str(), (unsigned int)str.size());
}

int Socket::send(uint32_t value)
{
    value = htonl(value);
    return this->send((const char*)&value, sizeof(uint32_t));
}

int Socket::send(uint16_t value) {
    value = htons(value);
    return this->send((const char*)&value, sizeof(uint16_t));
}

int Socket::recv(char * buf, unsigned int len) {
    return ::recv(this->_fd, buf, len, 0);
}

int Socket::recv(uint32_t& buf) {
    int ret = this->recv((char*)&buf, sizeof(uint32_t));
    if (ret == sizeof(uint32_t))
        buf = ntohl(buf);
    return ret;
}

int Socket::recv(uint16_t& buf) {
    int ret = this->recv((char*)&buf, sizeof(uint16_t));
    if (ret == sizeof(uint16_t))
        buf = ntohs(buf);
    return ret;
}

