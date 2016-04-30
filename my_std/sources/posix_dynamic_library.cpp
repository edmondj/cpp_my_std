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

#ifndef _WIN32
# include <string.h>
# include "posix_dynamic_library.hpp"
# include "file_system.hpp"

using namespace std;
using namespace my_std;

# ifndef __CYGWIN__

my_std::_dl::handle my_std::_dl::load(const std::string& f)
{
    string dir = fs::get_directory_name(f);
    string filename = fs::get_file_name(f);
    string ext = fs::get_file_extension(f);

    if (filename.substr(0, 3) != "lib")
        filename = "lib" + filename;

    if (!dir.empty())
        filename = fs::combine(dir, filename);

    if (ext != ".so")
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

# endif

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
