#pragma once

#include <string>
#include <functional>
#include <map>

class unit_test_manager
{
public:
    static void start_console();
    static void register_test(const std::string& scope, const std::string& name, const std::function<void()>& test);
    static void run_all_scope();
    static void run_all_tests(const std::string& scope);
    static void run_test(const std::string& scope, const std::string& name);

private:
    std::map<std::string, std::map<std::string, std::function<void()>>> tests;

    static unit_test_manager instance;
};
