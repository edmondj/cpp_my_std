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
#pragma once

#include <string>
#include <type_traits>
#include "my_std_export.hpp"

#ifdef _WIN32
# include "win32_dynamic_library.hpp"
#else
# include "posix_dynamic_library.hpp"
#endif

namespace my_std
{
    class MY_STD_EXPORT_TAG dynamic_library
    {
    public:
        dynamic_library();
        dynamic_library(dynamic_library&& other);

        ~dynamic_library();

        dynamic_library& operator=(dynamic_library&& other);

        bool open(const std::string& library);

        template<typename T>
        T* get_symbol(const std::string& sym_name)
        {
            T* sent = reinterpret_cast<T*>(_dl::get_symbol(this->_handle, sym_name));

            this->set_error();
            return sent;
        }

        bool close();

        const std::string& get_last_error() const;

    private:
        void set_error();

        bool _free_handle;
        _dl::handle _handle;
        std::string* _error;
    };
}