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
#ifndef _WIN32
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
#endif

#include "tcp_client_socket.hpp"

my_std::tcp_client_socket::tcp_client_socket(socket::socket_type fd, const end_point& ep)
    : socket(fd), _ep(ep) 
{
}

const my_std::end_point& my_std::tcp_client_socket::endpoint() const
{
    return this->_ep;
}

bool my_std::tcp_client_socket::no_delay() const 
{
    int val;
    socklen_t size = sizeof(val);

    if (this->get_sock_opt(IPPROTO_TCP, TCP_NODELAY, (char*)&val, &size) != 0)
        return false;
    return val != 0;
}

bool my_std::tcp_client_socket::no_delay(bool value) 
{
    int val = value;

    if (this->set_sock_opt(IPPROTO_TCP, TCP_NODELAY, (const char*)&val, sizeof(val)) != 0)
        return false;
    return value;
}
