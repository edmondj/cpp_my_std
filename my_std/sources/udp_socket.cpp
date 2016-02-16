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
#include "udp_socket.hpp"

my_std::udp_socket::udp_socket()
    : socket(::socket(AF_INET, SOCK_DGRAM, 0)) 
{}

my_std::udp_socket::~udp_socket()
{}

int my_std::udp_socket::sendto(const char* data, unsigned int length, const my_std::end_point& endpoint)
{
    return ::sendto(this->get_fd(), data, length, 0, endpoint.get_sockaddr(), endpoint.get_len());
}

int my_std::udp_socket::sendto(const std::string& data, const my_std::end_point& endpoint)
{
    return this->sendto(data.c_str(), (unsigned int)data.size(), endpoint);
}

int my_std::udp_socket::recvfrom(char* buf, unsigned int bufsize, my_std::end_point& endpoint)
{
    struct sockaddr_in remaddr;
    socklen_t addrlen = sizeof(remaddr);
    int sent = ::recvfrom(this->get_fd(), buf, bufsize, 0, (struct sockaddr*)&remaddr, &addrlen);
    if (sent >= 0)
        endpoint = end_point(remaddr);
    return sent;
}

int my_std::udp_socket::recvfrom(char* buf, unsigned int bufsize)
{
    return ::recvfrom(this->get_fd(), buf, bufsize, 0, nullptr, nullptr);
}
