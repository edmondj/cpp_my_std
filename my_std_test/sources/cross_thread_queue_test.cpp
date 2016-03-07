#include "catch.hpp"
#include "cross_thread_queue.hpp"

using namespace my_std;

TEST_CASE("Testing cross thread queue", "[cross_thread_queue][thread]")
{
    SECTION("Compilation")
    {
        cross_thread_queue<int> queue;
    }
}
