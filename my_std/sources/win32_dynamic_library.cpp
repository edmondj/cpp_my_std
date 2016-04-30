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
#ifdef _WIN32
#include "win32_dynamic_library.hpp"
#include "win32_helper.hpp"
#include "file_system.hpp"

using namespace std;

my_std::_dl::handle my_std::_dl::load(const std::string& f)
{
    string filename = f;
    string ext = fs::get_file_extension(filename);

    if (ext != "dll" && ext != "exe")
        filename += ".dll";

    return LoadLibrary(filename.c_str());
}

bool my_std::_dl::close(handle h)
{
    return FreeLibrary(h) != 0;
}

std::string my_std::_dl::get_last_error()
{
    DWORD err = GetLastError();
    return _win32::strerror(err);
}

my_std::_dl::symbol_addr my_std::_dl::get_symbol(handle h, const std::string& name)
{
    return GetProcAddress(h, name.c_str());
}

#endif