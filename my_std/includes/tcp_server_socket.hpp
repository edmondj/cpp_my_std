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
#pragma once

#include "tcp_socket.hpp"
#include "socket.hpp"
#include "tcp_client_socket.hpp"
#include "end_point.hpp"
#include "attributes.hpp"

namespace my_std
{
    class SHARED_EXPORT tcp_server_socket : public tcp_socket
    {
    public:
        tcp_server_socket();
        ~tcp_server_socket();

        tcp_client_socket* accept();
        tcp_client_socket* accept(end_point& endpoint);

        using socket::listen;
        using socket::bind;

    private:
        tcp_server_socket(const tcp_server_socket& other) = delete;
        tcp_server_socket(tcp_server_socket&& other) = delete;
        tcp_server_socket& operator=(const tcp_server_socket& other) = delete;
        tcp_server_socket& operator=(tcp_server_socket&& other) = delete;
    };
}