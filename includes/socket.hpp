#pragma once

#include <string>

#if _WIN32
# include "Win32Network.hpp"
#else
# include <unistd.h>
# include <sys/socket.h>
# include <poll.h>
#endif
#include "EndPoint.hpp"

class Socket {
public:
    static std::string get_last_error();

    Socket(Socket&& other);
    Socket& operator=(Socket&& other);

    int close();
    virtual ~Socket();

    bool reuse_addr() const;
    bool reuse_addr(bool value);

    int rcv_buf_size() const;
    int rcv_buf_size(int value);

    int snd_buf_size() const;
    int snd_buf_size(int value);

    int get_sock_opt(int level, int optname, void* optval, socklen_t* optlen) const;
    int set_sock_opt(int level, int optname, const void* optval, socklen_t optlen);

    int poll(short& events, int timeout = -1);

    enum PollMode
    {
        PollIn = POLLIN,
        PollRDNorm = POLLRDNORM,
        PolRDBand = POLLRDBAND,
        PollPri = POLLPRI,
        PollOut = POLLOUT,
        PollWRNorm = POLLWRNORM,
        PolWRBand = POLLWRBAND,
        PollErr = POLLERR,
        PollHup = POLLHUP,
        PollNval = POLLNVAL
    };

protected:
#if _WIN32
    typedef SOCKET socket_type;
    typedef const SOCKET const_socket_type;
#else
    typedef int socket_type;
    typedef int const_socket_type;
#endif
    static const_socket_type invalid_socket;

    Socket(socket_type fd);

    const_socket_type get_fd() const;
    socket_type get_fd();

protected:
    int bind(const EndPoint& endpoint);
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

    Socket() = delete;
    Socket(const Socket& other) = delete;
    Socket& operator=(const Socket& other) = delete;
};
