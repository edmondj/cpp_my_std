#include "end_point.hpp"
#include "catch.hpp"

using namespace std;
using namespace my_std;

TEST_CASE("Testing end point", "[end_point]")
{
    SECTION("Test any")
    {
        end_point a = end_point::any(12345);

        REQUIRE(a.get_addr() == INADDR_ANY);
        REQUIRE(a.get_port() == 12345);
    }
}