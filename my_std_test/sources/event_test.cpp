#include "catch.hpp"
#include "event.hpp"
#include <iostream>

using namespace my_std;
using namespace std;

class event_test
{
public:
    event_test()
    {
        this->e.add(bind(&event_test::event_handler, this, placeholders::_1, placeholders::_2));
    }

    event<event_test, int&> e;

    void notify()
    {
        int i = 42;
        this->e(*this, i);
        REQUIRE(i == 43);
    }

    void event_handler(const event_test&, int& i)
    {
        REQUIRE(i == 42);
        ++i;
    }
};

TEST_CASE("Testing events", "[event]")
{
    SECTION("Basic")
    {
        event_test e;

        e.notify();
    }
}