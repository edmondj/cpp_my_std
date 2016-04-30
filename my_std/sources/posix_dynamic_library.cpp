#ifndef _WIN32
#include <string.h>
#include "posix_dynamic_library.hpp"
#include "file_system.hpp"

using namespace std;
using namespace my_std;

# ifndef __CYGWIN__

my_std::_dl::handle my_std::_dl::load(const std::string& f)
{
    string filename = f;
    string ext = fs::get_file_extension(filename);

    if (ext != "so")
        filename += ".so";

    return dlopen(filename.c_str(), RTLD_LAZY | RTLD_GLOBAL);
}

# else

my_std::_dl::handle my_std::_dl::load(const std::string& f)
{
    string dir = fs::get_directory_name(f);
    string filename = fs::get_file_name(f);
    string ext = fs::get_file_extension(f);

    if (filename.substr(0, 3) != "cyg")
        filename = "cyg" + filename;

    if (!dir.empty())
        filename = fs::combine(dir, filename);

    if (ext != ".dll")
        filename += ".dll";

    return dlopen(filename.c_str(), RTLD_LAZY | RTLD_GLOBAL);
}

#endif

bool my_std::_dl::close(handle h)
{
    return dlclose(h) != 0;
}

std::string my_std::_dl::get_last_error()
{
    char* sent = dlerror();
    return (sent == nullptr ? "" : sent);
}

my_std::_dl::symbol_addr my_std::_dl::get_symbol(handle h, const std::string& name)
{
    return dlsym(h, name.c_str());
}

#endif