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
#include "my_std_export.hpp"

namespace my_std
{
    class MY_STD_EXPORT_TAG tcp_socket : public socket
    {
    public:
        tcp_socket();
        tcp_socket(tcp_socket&& other);
        tcp_socket& operator=(tcp_socket&& other);

        int connect(const end_point& endPoint);

        using socket::send;
        using socket::recv;

        bool no_delay() const;
        bool no_delay(bool value);

    private:
        tcp_socket(const tcp_socket& other) = delete;
        tcp_socket& operator=(const tcp_socket& other) = delete;
    };
}