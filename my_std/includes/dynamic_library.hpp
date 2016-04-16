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