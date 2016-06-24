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
#include <algorithm>
#include <string>
#include <sstream>

namespace my_std
{
    inline std::string to_lower(const std::string& str)
    {
        std::string sent = str;
        transform(sent.begin(), sent.end(), sent.begin(), [](char c) { return tolower(c); });
        return sent;
    }

    template<typename T>
    std::string to_string(const T& value)	// Generic to_string
    {
        std::ostringstream out;

        out << value;
        return out.str();
    }

    template<typename T>
    bool from_string(const std::string& str, T& value)
    {
        std::istringstream is(str);

        return (bool)(is >> value);
    }

#ifdef __CYGWIN__

#include <stdexcept>

    inline int stoi(const std::string& str)
    {
        std::istringstream in(str);
        int sent = 0;

        if (!(in >> sent))
            throw std::invalid_argument("Bad formatted number");
        return sent;
    }

    inline unsigned long stoul(const std::string& str)
    {
        std::istringstream in(str);
        unsigned long sent = 0;

        if (!(in >> sent))
            throw std::invalid_argument("Bad formatted number");
        return sent;
    }

#endif
}