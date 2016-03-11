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
#include "..\includes\end_point.hpp"
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
#include <cstring>
#if _WIN32
# include <Ws2tcpip.h>
#else
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
#endif
#include "end_point.hpp"

using namespace std;

static const unsigned long address_any = INADDR_ANY;
static const unsigned long address_loopback = INADDR_LOOPBACK;
static const unsigned long address_broadcast = INADDR_BROADCAST;
static const unsigned long address_none = INADDR_NONE;

my_std::end_point my_std::end_point::any(unsigned short port)
{
    return my_std::end_point(address_any, port);
}

my_std::end_point my_std::end_point::loopback(unsigned short port)
{
    return end_point(address_loopback, port);
}

my_std::end_point::end_point()
{
    memset(&this->_addr, 0, sizeof(this->_addr));
}

my_std::end_point::end_point(const sockaddr_in& addr)
{
    memcpy(&this->_addr, &addr, sizeof(this->_addr));
}

my_std::end_point::end_point(unsigned long addr, unsigned short port)
{
    this->init(addr, port);
}

my_std::end_point::end_point(const std::string& hostname, unsigned short port)
{
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

my_std::end_point::end_point(const my_std::end_point& other)
{
    memcpy(&this->_addr, &other._addr, sizeof(this->_addr));
}

my_std::end_point::~end_point()
{

}

bool my_std::end_point::operator==(const end_point& other) const
{
    return memcmp(&this->_addr, &other._addr, sizeof(this->_addr)) == 0;
}

bool my_std::end_point::operator<(const end_point& other) const
{
    return this->_addr.sin_addr.s_addr < other._addr.sin_addr.s_addr;
}

socklen_t my_std::end_point::get_len() const
{
    return sizeof(this->_addr);
}

unsigned long my_std::end_point::get_addr() const
{
    return this->_addr.sin_addr.s_addr;
}

string my_std::end_point::get_addr_str() const
{
    char buff[17];

    return string(inet_ntop(AF_INET, (void *)&this->_addr.sin_addr, buff, sizeof(buff)));
}

unsigned short my_std::end_point::get_port() const
{
    return ntohs(this->_addr.sin_port);
}

struct sockaddr* my_std::end_point::get_sockaddr()
{
    return (struct sockaddr*)&this->_addr;
}

const struct sockaddr* my_std::end_point::get_sockaddr() const
{
    return (const struct sockaddr*)&this->_addr;
}

void my_std::end_point::init(unsigned long addr, unsigned short port)
{
    addr = htonl(addr);
    port = htons(port);
    memset(&this->_addr, 0, sizeof(this->_addr));
    this->_addr.sin_addr.s_addr = addr;
    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = port;
}

std::ostream& operator<<(std::ostream& out, const my_std::end_point& ep)
{
    return out << ep.get_addr_str() << ':' << ep.get_port();
}
