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

