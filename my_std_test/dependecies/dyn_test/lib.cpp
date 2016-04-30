#include "attributes.hpp"

extern "C"
{
    int SHARED_EXPORT global = 42;

    int SHARED_EXPORT square(int value)
    {
        return value * value;
    }
}