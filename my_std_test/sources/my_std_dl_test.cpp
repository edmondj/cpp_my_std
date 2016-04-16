#include "attributes.hpp"

extern "C"
{
    SHARED_EXPORT int square(int m)
    {
        return m * m;
    }

    SHARED_EXPORT int global = 42;
}