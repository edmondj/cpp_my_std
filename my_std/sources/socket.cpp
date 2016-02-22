/*
Copyright (c) 2016 Julien Edmond

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
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
#include <cstring>
#include "socket.hpp"

using namespace std;

my_std::socket::const_socket_type my_std::socket::invalid_socket =
#if _WIN32
    INVALID_SOCKET
#else
    - 1
#endif
    ;

#ifdef _WIN32

std::string my_std::socket::get_last_error()
{
    int error = WSAGetLastError();
    LPSTR s;

    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error, 0, (LPSTR)&s, 0, nullptr) == 0)
        return "Failed to retrieve error";
    string sent(s);

    LocalFree(s);
    return sent;
}

#else

std::string Socket::get_last_error() 
{
    return string(strerror(errno));
}

#endif

my_std::socket::socket(socket&& other)
    : _fd(other._fd)
{
    other._fd = -1;
}

my_std::socket& my_std::socket::operator=(socket&& other)
{
    if (this != &other)
    {
        this->close();
        this->_fd = other._fd;
        other._fd = -1;
    }
    return *this;
}

int my_std::socket::close() 
{
#if _WIN32
    return ::closesocket(this->_fd);
#else
    ::shutdown(this->_fd, SHUT_RDWR);
    return ::close(this->_fd);
#endif
}

my_std::socket::~socket() 
{
    this->close();
}

bool my_std::socket::reuse_addr() const 
{
    int val;
    socklen_t size = sizeof(val);

    if (this->get_sock_opt(SOL_SOCKET, SO_REUSEADDR, &val, &size) != 0)
        return false;
    return val != 0;
}

bool my_std::socket::reuse_addr(bool value) 
{
    int val = value;

    if (this->set_sock_opt(SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) != 0)
        return false;
    return value;
}

int my_std::socket::rcv_buf_size() const 
{
    int val;
    socklen_t size = sizeof(val);

    if (this->get_sock_opt(SOL_SOCKET, SO_RCVBUF, &val, &size) != 0)
        return -1;
    return val;
}

int my_std::socket::rcv_buf_size(int value) 
{
    if (this->set_sock_opt(SOL_SOCKET, SO_RCVBUF, &value, sizeof(value)) != 0)
        return -1;
    return value;
}

int my_std::socket::snd_buf_size() const 
{
    int val;
    socklen_t size = sizeof(val);

    if (this->get_sock_opt(SOL_SOCKET, SO_SNDBUF, &val, &size) != 0)
        return -1;
    return val;
}

int my_std::socket::snd_buf_size(int value) 
{
    if (this->set_sock_opt(SOL_SOCKET, SO_SNDBUF, &value, sizeof(value)) != 0)
        return -1;
    return value;
}

int my_std::socket::get_sock_opt(int level, int optname, void* optval, socklen_t* optlen) const 
{
    return getsockopt(this->_fd, level, optname, (char*)optval, optlen);
}

int my_std::socket::set_sock_opt(int level, int optname, const void * optval, socklen_t optlen) 
{
    return setsockopt(this->_fd, level, optname, (char*)optval, optlen);
}

#ifdef _WIN32

int my_std::socket::poll(poll_mode& events, int timeout)
{
    WSAPOLLFD fd;

    fd.events = (short)events;
    fd.fd = this->get_fd();
    fd.revents = 0;
    int ret = WSAPoll(&fd, 1, timeout);
    events = (poll_mode)fd.revents;
    return ret;
}

#else

int my_std::socket::poll(poll_mode& events, int timeout)
{
    struct pollfd fd;

    fd.events = (short)events;
    fd.fd = this->get_fd();
    fd.revents = 0;
    int ret = ::poll(&fd, 1, timeout);
    events = (poll_mode)fd.revents;
    return ret;
}

#endif

my_std::socket::socket(my_std::socket::socket_type fd)
    : _fd(fd) 
{
    if (this->_fd <= 0) 
    {
        throw runtime_error("Invalid file descriptor");
    }
}

my_std::socket::const_socket_type my_std::socket::get_fd() const 
{
    return this->_fd;
}

my_std::socket::socket_type my_std::socket::get_fd() 
{
    return this->_fd;
}

int my_std::socket::bind(const end_point& endpoint) 
{
    return ::bind(this->get_fd(), endpoint.get_sockaddr(), endpoint.get_len());
}

int my_std::socket::listen(int backlog) 
{
    return ::listen(this->get_fd(), backlog);
}

int my_std::socket::send(const char* str, unsigned int len)
{
    return this->_send(str, len, nullptr);
}

int my_std::socket::send(const std::string& str)
{
    return this->_send(str, nullptr);
}

int my_std::socket::send(uint32_t value)
{
    return this->_send(value, nullptr);
}

int my_std::socket::send(uint16_t value)
{
    return this->_send(value, nullptr);
}

int my_std::socket::sendto(const char* str, unsigned int len, const end_point& to)
{
    return this->_send(str, len, &to);
}

int my_std::socket::sendto(const std::string& str, const end_point& to)
{
    return this->_send(str, &to);
}

int my_std::socket::sendto(uint32_t value, const end_point& to)
{
    return this->_send(value, &to);
}

int my_std::socket::sendto(uint16_t value, const end_point& to)
{
    return this->_send(value, &to);
}

int my_std::socket::recv(char* buf, unsigned int len)
{
    return this->_recv(buf, len, nullptr);
}

int my_std::socket::recv(uint32_t& buf)
{
    return this->_recv(buf, nullptr);
}

int my_std::socket::recv(uint16_t& buf)
{
    return this->_recv(buf, nullptr);
}

int my_std::socket::recvfrom(char* buf, unsigned int len, end_point& from)
{
    return this->_recv(buf, len, &from);
}

int my_std::socket::recvfrom(uint32_t& buf, end_point& from)
{
    return this->_recv(buf, &from);
}

int my_std::socket::recvfrom(uint16_t& buf, end_point& from)
{
    return this->_recv(buf, &from);
}

int my_std::socket::_send(const char* str, unsigned int len, const end_point* to)
{
    if (to == nullptr)
        return ::send(this->_fd, str, len, 0);
    else
        return ::sendto(this->_fd, str, len, 0, to->get_sockaddr(), to->get_len());
}

int my_std::socket::_send(const std::string& str, const end_point* to)
{
    return this->_send(str.c_str(), str.size(), to);
}

int my_std::socket::_send(uint32_t value, const end_point* to)
{
    value = htonl(value);
    return this->_send((const char*)&value, sizeof(value), to);
}

int my_std::socket::_send(uint16_t value, const end_point* to)
{
    value = htons(value);
    return this->_send((const char*)&value, sizeof(value), to);;
}

int my_std::socket::_recv(char* buf, unsigned int len, end_point* from)
{
    if (from == nullptr)
        return ::recv(this->_fd, buf, len, 0);
    else
    {
        struct sockaddr_in remaddr;
        socklen_t addrlen = sizeof(remaddr);
        int ret = ::recvfrom(this->_fd, buf, len, 0, (sockaddr*)&remaddr, &addrlen);
        if (ret > 0)
            *from = end_point(remaddr);
        return ret;
    }
}

int my_std::socket::_recv(uint32_t& buf, end_point* from)
{
    int ret = this->_recv((char*)&buf, sizeof(buf), from);
    if (ret == sizeof(buf))
        ret = ntohl(buf);
    return ret;
}

int my_std::socket::_recv(uint16_t& buf, end_point * from)
{
    int ret = this->_recv((char*)&buf, sizeof(buf), from);
    if (ret == sizeof(buf))
        ret = ntohs(buf);
    return ret;
}
