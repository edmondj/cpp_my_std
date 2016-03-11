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

#include <iostream>
#include <string>
#if _WIN32
# include "win32_network.hpp"
#else
# include <netinet/ip.h>
#endif
#include "my_std_export.hpp"

namespace my_std
{
    class MY_STD_EXPORT_TAG end_point
    {
    public:
        static end_point any(unsigned short port);
        static end_point loopback(unsigned short port);

        end_point();
        end_point(const struct sockaddr_in& addr);
        end_point(unsigned long addr, unsigned short port);
        end_point(const std::string& hostname, unsigned short port);
        end_point(const end_point& other);
        ~end_point();

        bool operator==(const end_point& other) const;
        bool operator<(const end_point& other) const;

        socklen_t get_len() const;
        unsigned long get_addr() const;
        std::string get_addr_str() const;
        unsigned short get_port() const;
        struct sockaddr* get_sockaddr();
        const struct sockaddr* get_sockaddr() const;

    private:
        void init(unsigned long addr, unsigned short port);

        struct sockaddr_in _addr;
    };
}

MY_STD_EXPORT_TAG std::ostream& operator<<(std::ostream& out, const my_std::end_point& ep);
