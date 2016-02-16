#include <iostream>
#include "unit_test_manager.hpp"

using namespace std;

class unit_test_test
{
public:
    static void say_hello()
    {
        cout << "Hello" << endl;
    }

    static void count()
    {
        for (size_t i = 0; i < 100; i++)
        {
            cout << i << endl;
        }
    }

    unit_test_test()
    {
        unit_test_manager::register_test("unit_test", "say hello", &unit_test_test::say_hello);
        unit_test_manager::register_test("unit_test", "count", &unit_test_test::count);
    }

    static unit_test_test instance;
};

unit_test_test unit_test_test::instance;