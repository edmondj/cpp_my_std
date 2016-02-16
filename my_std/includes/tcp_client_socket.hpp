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

#include "socket.hpp"
#include "end_point.hpp"
#include "attributes.hpp"

namespace my_std
{
    class SHARED_EXPORT tcp_client_socket : public socket 
    {
    public:
        tcp_client_socket(socket::socket_type fd, const end_point& ep);
        ~tcp_client_socket() = default;

        // Moved socket send and recv to public scope
        using socket::send;
        using socket::recv;

        const end_point& endpoint() const;

        bool no_delay() const;
        bool no_delay(bool value);

    private:
        end_point _ep;

        tcp_client_socket() = delete;
        tcp_client_socket(const tcp_client_socket& other) = delete;
        tcp_client_socket(tcp_client_socket&& other) = delete;
        tcp_client_socket& operator=(const tcp_client_socket& other) = delete;
        tcp_client_socket& operator=(tcp_client_socket&& other) = delete;
    };
}
