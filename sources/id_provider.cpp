#include "IDProvider.hpp"

IDProvider::IDProvider(unsigned int max)
    : _current(0), _max(max)
{
}

unsigned int IDProvider::get_next()
{
    unsigned int sent = this->_current;

    if (++this->_current >= this->_max)
        this->_current = 0;
    return sent;
}
