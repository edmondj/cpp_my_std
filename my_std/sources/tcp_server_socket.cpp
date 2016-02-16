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
#include <thread>
#include <chrono>
#include "tcp_server_socket.hpp"

my_std::tcp_server_socket::tcp_server_socket()
{}

my_std::tcp_server_socket::~tcp_server_socket() 
{}

my_std::tcp_client_socket* my_std::tcp_server_socket::accept()
{
    end_point dummy;
    return this->accept(dummy);
}

my_std::tcp_client_socket* my_std::tcp_server_socket::accept(end_point& endpoint)
{
    socklen_t len = endpoint.get_len();
    socket::socket_type s = ::accept(this->get_fd(), endpoint.get_sockaddr(), &len);

    if (s == socket::invalid_socket)
        return nullptr;
    return new tcp_client_socket(s, endpoint);
}
