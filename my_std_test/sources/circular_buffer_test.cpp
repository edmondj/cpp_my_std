#include "catch.hpp"
#include "circular_buffer.hpp"

using namespace my_std;

TEST_CASE("Testing circular buffer", "[circular_buffer]")
{
    SECTION("Push and for each")
    {
        circular_buffer<int> buff(3);

        buff.push(1);
        buff.push(2);
        buff.push(3);
        buff.push(4);

        int i = 0;
        for (circular_buffer<int>::const_iterator it = buff.cbegin(); it != buff.cend(); ++it)
        {
            if (i == 0)
                REQUIRE(*it == 2);
            else if (i == 1)
                REQUIRE(*it == 3);
            else if (i == 2)
                REQUIRE(*it == 4);
            else
                FAIL("Too many data");
            ++i;
        }
        REQUIRE(i == 3);
    }
}
