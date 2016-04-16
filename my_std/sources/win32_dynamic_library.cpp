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