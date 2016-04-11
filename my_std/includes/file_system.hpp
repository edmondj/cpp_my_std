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
#include "my_std_export.hpp"

namespace my_std
{
    namespace fs
    {
        MY_STD_EXPORT_TAG bool file_exists(const std::string& path);
        MY_STD_EXPORT_TAG bool get_file_size(const std::string& path, size_t& size);
        MY_STD_EXPORT_TAG std::string get_self_file_name();
        MY_STD_EXPORT_TAG std::string get_file_name(const std::string& path);
        MY_STD_EXPORT_TAG std::string get_file_extension(const std::string& path);
        MY_STD_EXPORT_TAG std::string get_file_name_without_extension(const std::string& path);
        MY_STD_EXPORT_TAG std::string get_directory_name(const std::string& path);
        MY_STD_EXPORT_TAG std::string combine(const std::string& left, const std::string& right);
        MY_STD_EXPORT_TAG std::string combine(std::initializer_list<std::string> components);
        MY_STD_EXPORT_TAG std::string get_cwd();
        MY_STD_EXPORT_TAG bool set_cwd(const std::string& path);
    }
}