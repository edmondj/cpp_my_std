#pragma once

#include <string>
#include "Attributes.hpp"

namespace FileSystem
{
    EXPORT bool file_exists(const std::string& path);
    EXPORT std::string get_self_file_name();
    EXPORT std::string get_file_name(const std::string& path);
    EXPORT std::string get_file_name_without_extension(const std::string& path);
    EXPORT std::string get_directory_name(const std::string& path);
    EXPORT std::string combine(const std::string& left, const std::string& right);
    EXPORT std::string combine(std::initializer_list<std::string> components);
    EXPORT std::string get_cwd();
    EXPORT bool set_cwd(const std::string& path);
}