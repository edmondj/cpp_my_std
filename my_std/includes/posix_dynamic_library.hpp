#pragma once
#ifndef _WIN32

# include <string>
# include <dlfcn.h>

# include "my_std_export.hpp"

namespace my_std
{
    namespace _dl
    {
        typedef void* handle;
        typedef void* symbol_addr;

        handle load(const std::string& filename);
        bool close(handle h);
        std::string get_last_error();
        MY_STD_EXPORT_TAG symbol_addr get_symbol(handle h, const std::string& name);
    }
}

#endif