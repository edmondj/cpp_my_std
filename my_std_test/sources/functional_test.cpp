#include "catch.hpp"
#include "my_functional.hpp"

using namespace my_std;

int minus(int l, int r)
{
    return l - r;
}

TEST_CASE("Testing functional", "[functional]")
{
    SECTION("Placeholders")
    {
        auto func = std::bind(minus, placeholder<2>::ph, placeholder<1>::ph);

        REQUIRE(func(3, 1) == 1 - 3);
    }

    SECTION("Bind fisrt")
    {
        auto func = bind_first(&minus, 3);

        REQUIRE(func(1) == 3 - 1);

        auto func2 = bind_first(func, 1);

        REQUIRE(func2() == 3 - 1);
    }
}
