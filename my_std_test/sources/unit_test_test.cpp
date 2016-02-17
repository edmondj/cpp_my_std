#include <iostream>
#include "catch.hpp"

TEST_CASE("Testing the unit test", "[unit_test]")
{
    SECTION("Say hello")
    {
        std::cout << "Hello" << std::endl;
    }

    SECTION("Count to 10")
    {
        size_t i;
        for (i = 0; i < 10; i++)
        {
            std::cout << i << std::endl;
        }
        REQUIRE(i == 10);
    }

    SECTION("Throw")
    {
        throw std::exception();
    }
}