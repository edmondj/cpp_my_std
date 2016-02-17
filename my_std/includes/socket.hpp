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

#include <string>

#if _WIN32
# include "win32_network.hpp"
#else
# include <unistd.h>
# include <sys/socket.h>
# include <poll.h>
#endif
#include "end_point.hpp"
#include "my_std_export.hpp"

namespace my_std
{
    enum class poll_mode : short
    {
        In = POLLIN,
        RDNorm = POLLRDNORM,
        RDBand = POLLRDBAND,
        Pri = POLLPRI,
        Out = POLLOUT,
        WRNorm = POLLWRNORM,
        WRBand = POLLWRBAND,
        Err = POLLERR,
        Hup = POLLHUP,
        Nval = POLLNVAL
    };

    class MY_STD_EXPORT_TAG socket
    {
    public:
        static std::string get_last_error();

        socket(socket&& other);
        socket& operator=(socket&& other);

        int close();
        virtual ~socket();

        bool reuse_addr() const;
        bool reuse_addr(bool value);

        int rcv_buf_size() const;
        int rcv_buf_size(int value);

        int snd_buf_size() const;
        int snd_buf_size(int value);

        int get_sock_opt(int level, int optname, void* optval, socklen_t* optlen) const;
        int set_sock_opt(int level, int optname, const void* optval, socklen_t optlen);

        int poll(poll_mode& events, int timeout = -1);

    protected:
#if _WIN32
        typedef SOCKET socket_type;
        typedef const SOCKET const_socket_type;
#else
        typedef int socket_type;
        typedef int const_socket_type;
#endif
        static const_socket_type invalid_socket;

        socket(socket_type fd);

        const_socket_type get_fd() const;
        socket_type get_fd();

    protected:
        int bind(const end_point& endpoint);
        int listen(int backlog);

        int send(const char* str, unsigned int len);
        int send(const std::string& str);
        int send(uint32_t value);
        int send(uint16_t value);

        int recv(char* buf, unsigned int len);
        int recv(uint32_t& buf);
        int recv(uint16_t& buf);

    private:

        socket_type _fd;

        socket() = delete;
        socket(const socket& other) = delete;
        socket& operator=(const socket& other) = delete;
    };
}