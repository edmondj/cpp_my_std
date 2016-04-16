#include "catch.hpp"

TEST_CASE("Say hello", "[unit_test]")
{
    INFO("Hello");
}

TEST_CASE("Count to 10", "[unit_test]")
{
    size_t i;
    for (i = 0; i < 10; i++)
    {
        INFO(i);
    }
    REQUIRE(i == 10);
}
