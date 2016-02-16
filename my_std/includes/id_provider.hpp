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

#include "attributes.hpp"

namespace my_std
{
    class SHARED_EXPORT id_provider
    {
    public:
        id_provider(unsigned int max = 1000);
        id_provider(const id_provider& other) = default;
        id_provider& operator=(const id_provider& other) = default;
        id_provider(id_provider&& other) = default;
        id_provider& operator=(id_provider&& other) = default;
        ~id_provider() = default;

        unsigned int get_next();

    private:
        unsigned int _current;
        unsigned int _max;
    };
}