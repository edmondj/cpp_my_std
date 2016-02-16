#include <sys/stat.h>
#include "FileSystem.hpp"

using namespace std;

#ifdef _WIN32

#include <direct.h>
#include <Windows.h>

string FileSystem::get_self_file_name()
{
    CHAR path[MAX_PATH];
    GetModuleFileName(nullptr, path, ARRAYSIZE(path));
    return string(path);
}

string FileSystem::get_cwd()
{
    CHAR path[MAX_PATH];

    path[0] = 0;
    _getcwd(path, sizeof(path));
    return string(path);
}

bool FileSystem::set_cwd(const string& path)
{
    return _chdir(path.c_str()) == 0;
}

#else

#include <unistd.h>

string FileSystem::get_self_file_name()
{
    char buffer[260];
    buffer[readlink("/proc/self/exe", buffer, sizeof(buffer))] = 0;
    return string(buffer);
}

string FileSystem::get_cwd()
{
    char buffer[260];

    buffer[0] = 0;
    getcwd(buffer, sizeof(buffer));
    return string(buffer);
}

bool FileSystem::set_cwd(const string& path)
{
    return chdir(path.c_str()) == 0;
}

#endif

bool FileSystem::file_exists(const string& path)
{
    struct stat buffer;
    return stat(path.c_str(), &buffer) == 0;
}

static bool is_file_separator(char c)
{
    return c == '/' || c == '\\';
}

string FileSystem::get_file_name(const string& path)
{
    size_t i = path.size();

    while (i > 0 && !is_file_separator(path[i - 1]) && path[i - 1] != ':')
        --i;
    return path.substr(i);
}

string FileSystem::get_file_name_without_extension(const string& path)
{
    string sent = get_file_name(path);

    return sent.substr(0, sent.find_last_of('.'));
}

string FileSystem::get_directory_name(const string& path)
{
    size_t i = path.size();

    while (i > 0 && !is_file_separator(path[i - 1]) && path[i - 1] != ':')
        --i;
    return path.substr(0, i);
}

std::string FileSystem::combine(const std::string& left, const std::string& right)
{
    if (left.empty())
        return right;
    if (right.empty())
        return left;
    if (is_file_separator(left.back()) || is_file_separator(right.front()))
        return left + right;
    return left + '/' + right;
}

string FileSystem::combine(initializer_list<string> components)
{
    string sent;

    for (const std::string& component : components)
    {
        sent = combine(sent, component);
    }
    return sent;
}
