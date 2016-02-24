#include <future>
#include "network.hpp"
#include "catch.hpp"

using namespace std;
using namespace my_std;

TEST_CASE("Testing TCP socket", "[network][tcp]")
{
    SECTION("Test server client echo")
    {
        tcp_server_socket serv;

        serv.reuse_addr(true);
        REQUIRE(serv.bind(end_point::any(12345)) == 0);
        REQUIRE(serv.listen(10) == 0);
        future<void> f = async([&serv]()
        {
            tcp_client_socket* client = serv.accept();

            REQUIRE(client != nullptr);

            uint32_t received;
            REQUIRE(client->recv(received) == sizeof(received));
            REQUIRE(received == 42);
        });

        tcp_socket sock;

        REQUIRE(sock.connect(end_point::loopback(12345)) == 0);
        REQUIRE(sock.send((uint32_t)42) == sizeof(uint32_t));

        REQUIRE_NOTHROW(f.get());
    }
}