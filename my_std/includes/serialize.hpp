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

#include <array>
#include <type_traits>
#include "my_type_traits.hpp"

namespace my_std
{
    template<typename TSerializer, typename T>
    constexpr auto ___has_put(TSerializer& s, const std::string& name, const T& data) -> decltype(s.put(name, data), std::true_type());

    constexpr std::false_type ___has_put(...);

    template<typename TSerializer, typename T>
    struct __has_put : decltype(___has_put(std::declval<TSerializer&>(), std::declval<const std::string&>(), std::declval<const T&>())) {};

    template<typename TSerializer, typename T>
    constexpr auto ___has_get(const TSerializer& s, const std::string& name, T& data) -> decltype(s.get(name, data), std::true_type());

    constexpr std::false_type ___has_get(...);

    template<typename TSerializer, typename T>
    struct __has_get : decltype(___has_get(std::declval<TSerializer&>(), std::declval<const std::string&>(), std::declval<const T&>())) {};

    template<typename TSerializer, typename T>
    struct __has_serialize_helper
    {
        template<typename U> struct __test {};
        template<typename U> static constexpr std::true_type test(__test<decltype(std::declval<U>().serialize(std::declval<TSerializer>()))>*);
        template<typename U> static constexpr std::false_type test(...);

        typedef decltype(test<T>(0)) type;
    };

    template<typename TSerializer, typename T>
    struct __has_deserialize_helper
    {
        template<typename U, bool(U::*)(TSerializer&)> struct __test {};
        template<typename U> static constexpr std::true_type test(__test<U, &U::deserialize>*);
        template<typename U> static constexpr std::false_type test(...);

        typedef decltype(test<T>(0)) type;
    };

    template<typename TSerializer, typename T>
    struct has_serialize : public __has_serialize_helper<TSerializer, T>::type
    {
    };

    template<typename TSerializer, typename T>
    struct has_deserialize : public __has_deserialize_helper<TSerializer, T>::type
    {
    };

    template<typename TSerializer, typename T>
    struct is_serializable : public std::integral_constant<bool, has_serialize<TSerializer, T>::value && has_deserialize<TSerializer, T>::value>
    {
    };

    template<typename TSerializer, typename T>
    struct __put_put
    {
        static void put(TSerializer& s, const std::string& name, const T& value)
        {
            s.put(name, value);
        }
    };

    template<typename TSerializer, typename T>
    struct __put_none
    {
        static void put(TSerializer&, const std::string&, const T&)
        {
            static_assert(std::false_type::value, "Unknown serialization for current type");
        }
    };

    template<typename TSerializer, typename T>
    struct __put_serializable
    {
        static void put(TSerializer& serializer, const std::string& name, const T& data)
        {
            serializer.put_serializable(name, data);
        }
    };

    template<typename TSerializer, typename T>
    struct __put_container
    {
        static void put(TSerializer& serializer, const std::string& name, const T& data)
        {
            serializer.put_container(name, data);
        }
    };

    template<typename T>
    struct is_std_array : std::false_type {};

    template<typename T, size_t size>
    struct is_std_array<std::array<T, size>> : std::true_type {};

    template<typename TSerializer, typename T>
    struct __put_array
    {
        static void put(TSerializer& serializer, const std::string& name, const T& data)
        {
            serializer.put_array(name, data, std::extent<T>::value);
        }
    };

    template<typename TSerializer, typename T, size_t size>
    struct __put_array<TSerializer, std::array<T, size> >
    {
        static void put(TSerializer& serializer, const std::string& name, const std::array<T, size>& data)
        {
            serializer.put_array(name, data, size);
        }
    };

    template<typename TSerializer, typename T>
    struct __put_io
    {
        static void put(TSerializer& serializer, const std::string& name, const T& data)
        {
            serializer.put_io(name, data);
        }
    };

    template<typename TSerializer, typename T>
    void put(TSerializer& serializer, const std::string& name, const T& data)
    {
        typename std::conditional<__has_put<TSerializer, T>::value, __put_put<TSerializer, T>,
            typename std::conditional<is_serializable<TSerializer, T>::value, __put_serializable<TSerializer, T>,
            typename std::conditional<std::is_array<T>::value || is_std_array<T>::value, __put_array<TSerializer, T>,
            typename std::conditional<has_stream_io<T>::value, __put_io<TSerializer, T>,
            typename std::conditional<is_container<T>::value, __put_container<TSerializer, T>, __put_none<TSerializer, T>>::type
            >::type
            >::type
            >::type
        >::type::put(serializer, name, data);
    }

    template<typename TSerializer, typename T>
    struct __get_none
    {
        static void get(TSerializer& serializer, const std::string& name, T& data)
        {
            static_assert(std::true_type::value, "Unknown serialization for current type");
        }

        static size_t get_all(TSerializer& serializer, const std::string& name, T& data)
        {
            static_assert(std::true_type::value, "Unknown serialization for current type");
            return 0;
        }
    };

    template<typename TSerializer, typename T>
    struct __get_get
    {
        static bool get(TSerializer& serializer, const std::string& name, T& data)
        {
            return serializer.get(name, data);
        }
    };

    template<typename TSerializer, typename T>
    struct __get_serializable
    {
        static bool get(TSerializer& serializer, const std::string& name, T& data)
        {
            return serializer.get_serializable(name, data);
        }
    };

    template<typename TSerializer, typename T>
    struct __get_container
    {
        static bool get(TSerializer& serializer, const std::string& name, T& data)
        {
            return serializer.get_container(name, data);
        }

        static size_t get_all(TSerializer& serializer, const std::string& name, T& data)
        {
            return serializer.get_all_container(name, data);
        }
    };

    template<typename TSerializer, typename T>
    struct __get_array
    {
        static bool get(TSerializer& serializer, const std::string& name, T& data)
        {
            return serializer.get_array(name, data, std::extent<T>::value);
        }

        static size_t get_all(TSerializer& serializer, const std::string& name, T& data)
        {
            return serializer.get_all_array(name, data);
        }
    };

    template<typename TSerializer, typename T>
    struct __get_io
    {
        static bool get(TSerializer& serializer, const std::string& name, T& data)
        {
            return serializer.get_io(name, data);
        }
    };

    template<typename TSerializer, typename T>
    bool get(TSerializer& serializer, const std::string& name, T& out)
    {
        return typename std::conditional<__has_get<TSerializer, T>::value, __get_get<TSerializer, T>,
            typename std::conditional<is_serializable<TSerializer, T>::value, __get_serializable<TSerializer, T>,
            typename std::conditional<std::is_array<T>::value || is_std_array<T>::value, __get_array<TSerializer, T>,
            typename std::conditional<has_stream_io<T>::value, __get_io<TSerializer, T>,
            typename std::conditional<is_container<T>::value, __get_container<TSerializer, T>, __get_none<TSerializer, T>>::type
            >::type
            >::type
            >::type
        >::type::get(serializer, name, out);
    }

    template<typename TSerializer, typename T, size_t size>
    size_t get_all(TSerializer& serializer, const std::string& name, std::array<T, size>&  out)
    {
        return serializer.get_all_array(name, out);
    }

    template<typename TSerializer, typename T>
    size_t get_all(TSerializer& serializer, const std::string& name, T& out)
    {
        return typename std::conditional<std::is_array<T>::value, __get_array<TSerializer, T>,
            typename std::conditional<is_container<T>::value, __get_container<TSerializer, T>, __get_none<TSerializer, T>>::type
        >::type::get_all(serializer, name, out);
    }
}