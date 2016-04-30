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
#include "dynamic_library.hpp"

using namespace std;

my_std::dynamic_library::dynamic_library()
    : _free_handle(false), _handle(nullptr), _error(new string())
{ }

my_std::dynamic_library::dynamic_library(dynamic_library&& other)
    : _free_handle(other._free_handle), _handle(other._handle), _error(new string(*other._error))
{
    other._free_handle = false;
}

my_std::dynamic_library::~dynamic_library()
{
    this->close();
    delete this->_error;
}

my_std::dynamic_library& my_std::dynamic_library::operator=(dynamic_library&& other)
{
    if (&other != this)
    {
        this->_free_handle = other._free_handle;
        this->_handle = other._handle;
        *this->_error = move(*other._error);
        other._free_handle = false;
    }
    return *this;
}

bool my_std::dynamic_library::open(const std::string& library)
{
    this->close();
    this->_handle = _dl::load(library);
    this->set_error();
    this->_free_handle = (this->_handle != nullptr);
    return this->_handle == nullptr;
}

bool my_std::dynamic_library::close()
{
    bool sent = true;
    if (this->_free_handle)
    {
        sent = _dl::close(this->_handle);
        this->set_error();
        this->_handle = nullptr;
        this->_free_handle = false;
    }
    return sent;
}

const std::string& my_std::dynamic_library::get_last_error() const
{
    return *this->_error;
}

void my_std::dynamic_library::set_error()
{
    *this->_error = _dl::get_last_error();
}

