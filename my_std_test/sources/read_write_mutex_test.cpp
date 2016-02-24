#include <atomic>
#include <thread>
#include "catch.hpp"
#include "read_write_mutex.hpp"

using namespace std;
using namespace my_std;

TEST_CASE("Testing read write mutex", "[thread][rwmutex]")
{
    read_write_mutex mutex;

    SECTION("Basic")
    {
        atomic<int> reading = 0;

        for (int i = 0; i < 4; i++)
        {
            thread([&reading, &mutex, i]()
            {
                this_thread::sleep_for(chrono::milliseconds(i * 100));
                mutex.read_lock();
                ++reading;
                this_thread::sleep_for(chrono::milliseconds(150));
                --reading;
                mutex.read_unlock();
            }).detach();
        }
        this_thread::sleep_for(chrono::milliseconds(150));
        mutex.write_lock();
        REQUIRE(reading == 0);
        mutex.write_unlock();
    }
}