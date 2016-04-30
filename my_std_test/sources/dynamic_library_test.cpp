#include "catch.hpp"
#include "dynamic_library.hpp"

using namespace my_std;

TEST_CASE("Testing dynamic library", "[dl]")
{
    SECTION("Basic")
    {
        dynamic_library dl;

        bool result = dl.open("./dyn_test");

        if (result)
           INFO(dl.get_last_error());
        REQUIRE(result == false);
    }

    SECTION("Get global")
    {
        dynamic_library dl;

        bool result = dl.open("./dyn_test");

        if (result)
            INFO(dl.get_last_error());
        REQUIRE(result == false);

        int* glob = dl.get_symbol<int>("global");

        if (glob == nullptr)
            INFO(dl.get_last_error());
        REQUIRE(glob != nullptr);

        REQUIRE(*glob == 42);
    }

    SECTION("Get func")
    {
        dynamic_library dl;

        bool result = dl.open("./dyn_test");

        if (result)
            INFO(dl.get_last_error());
        REQUIRE(result == false);

        int (*square)(int) = dl.get_symbol<int(int)>("square");

        if (square == nullptr)
            INFO(dl.get_last_error());
        REQUIRE(square != nullptr);

        REQUIRE(square(4) == 16);
    }
}
