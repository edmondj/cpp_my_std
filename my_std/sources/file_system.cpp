/*
Copyright (c) 2016 Julien Edmond

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#include <sys/stat.h>
#include "file_system.hpp"

#ifdef _WIN32

#include <direct.h>
#include <Windows.h>

using namespace std;

string my_std::fs::get_self_file_name()
{
    CHAR path[MAX_PATH];
    GetModuleFileName(nullptr, path, ARRAYSIZE(path));
    return string(path);
}

string my_std::fs::get_cwd()
{
    CHAR path[MAX_PATH];

    path[0] = 0;
    _getcwd(path, sizeof(path));
    return string(path);
}

bool my_std::fs::set_cwd(const string& path)
{
    return _chdir(path.c_str()) == 0;
}

#else

#include <unistd.h>

using namespace std;

string my_std::fs::get_self_file_name()
{
    char buffer[260];
    buffer[readlink("/proc/self/exe", buffer, sizeof(buffer))] = 0;
    return string(buffer);
}

string my_std::fs::get_cwd()
{
    char buffer[260];

    buffer[0] = 0;
    getcwd(buffer, sizeof(buffer));
    return string(buffer);
}

bool my_std::fs::set_cwd(const string& path)
{
    return chdir(path.c_str()) == 0;
}

#endif

bool my_std::fs::file_exists(const string& path)
{
    struct stat buffer;
    return stat(path.c_str(), &buffer) == 0;
}

bool my_std::fs::get_file_size(const std::string& path, size_t& size)
{
    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0)
        return false;
    size = buffer.st_size;
    return true;
}

static bool is_file_separator(char c)
{
    return c == '/' || c == '\\';
}

string my_std::fs::get_file_name(const string& path)
{
    size_t i = path.size();

    while (i > 0 && !is_file_separator(path[i - 1]) && path[i - 1] != ':')
        --i;
    return path.substr(i);
}

std::string my_std::fs::get_file_extension(const std::string& path)
{
    string fn = get_file_name(path);
    size_t offset = fn.find_last_of('.');

    if (offset == string::npos)
        return "";
    return fn.substr(offset + 1);
}

string my_std::fs::get_file_name_without_extension(const string& path)
{
    string sent = my_std::fs::get_file_name(path);

    return sent.substr(0, sent.find_last_of('.'));
}

string my_std::fs::get_directory_name(const string& path)
{
    size_t i = path.size();

    while (i > 0 && !is_file_separator(path[i - 1]) && path[i - 1] != ':')
        --i;
    return path.substr(0, i);
}

std::string my_std::fs::combine(const std::string& left, const std::string& right)
{
    if (left.empty())
        return right;
    if (right.empty())
        return left;
    if (is_file_separator(left.back()) || is_file_separator(right.front()))
        return left + right;
    return left + '/' + right;
}

string my_std::fs::combine(initializer_list<string> components)
{
    string sent;

    for (const std::string& component : components)
    {
        sent = combine(sent, component);
    }
    return sent;
}
