#include <stdexcept>
#include <iostream>
#include <chrono>
#include <typeinfo>
#include "unit_test_manager.hpp"

using namespace std;

void unit_test_manager::start_console()
{
    while (true)
    {
        cout << "Select a scope:" << endl;
        cout << "0. back" << endl;
        cout << "1. all" << endl;
        size_t selected_scope = 1;
        for (auto& kv : instance.tests)
            cout << ++selected_scope << ". " << kv.first << endl;
        if (cin >> selected_scope)
        {
            if (selected_scope == 0)
                break;
            else if (selected_scope == 1)
                run_all_scope();
            else if (selected_scope - 2 >= instance.tests.size())
            {
                cout << "Invalid" << endl;
                continue;
            }
            else
            {
                auto scope = instance.tests.begin();
                while (selected_scope > 2)
                {
                    ++scope;
                    --selected_scope;
                }

                while (true)
                {
                    cout << "Select a test:" << endl;
                    cout << "0. back" << endl;
                    cout << "1. all" << endl;
                    size_t selected_test = 1;
                    for (auto& kv : scope->second)
                        cout << ++selected_test << ". " << kv.first << endl;
                    if (cin >> selected_test)
                    {
                        if (selected_test == 0)
                            break;
                        else if (selected_test == 1)
                            run_all_tests(scope->first);
                        else if (selected_test - 2 >= scope->second.size())
                        {
                            cout << "Invalid" << endl;
                            continue;
                        }
                        else
                        {
                            auto test = scope->second.begin();
                            while (selected_test > 2)
                            {
                                ++test;
                                --selected_test;
                            }
                            run_test(scope->first, test->first);
                        }
                    }
                    else
                    {
                        cin.ignore(std::numeric_limits<std::streamsize>::max());
                    }
                }
            }
        }
        else
        {
            cin.ignore(std::numeric_limits<std::streamsize>::max());
        }
    }
}

void unit_test_manager::register_test(const string& scope, const string& name, const function<void()>& test)
{
    if (!test)
        throw std::runtime_error("Empty test function");
    instance.tests[scope][name] = test;
}

void unit_test_manager::run_all_scope()
{
    for (auto& kv : instance.tests)
        run_all_tests(kv.first);
}

void unit_test_manager::run_all_tests(const string& scope)
{
    for (auto& kv : instance.tests.at(scope))
        run_test(scope, kv.first);
}

void unit_test_manager::run_test(const string& scope, const string& name)
{
    const function<void()>& test(instance.tests.at(scope).at(name));

    cout << "--- Testing " << scope << " / " << name << endl;
    auto start = chrono::steady_clock::now();
    try
    {
        test();
    }
    catch (const std::exception& except)
    {
        cout << "--- Test threw " << typeid(except).name() << ": " << except.what() << endl;
    }
    cout << "--- End test " << scope << " / " << name << " duration: " << chrono::duration<double>(chrono::steady_clock::now() - start).count() << "s" << endl;
}

unit_test_manager unit_test_manager::instance;
