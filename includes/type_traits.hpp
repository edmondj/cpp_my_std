#pragma once

#include <iostream>
#include <type_traits>

template<typename T>
struct __is_container_helper
{
    template<typename U, typename V, V(U::*)(), V(U::*)(), size_t(U::*)()const, typename W> struct __test {};
    template<typename U> static constexpr std::true_type test(__test<U, typename U::iterator, &U::begin, &U::end, &U::size, typename U::value_type>*);
    template<typename U> static constexpr std::false_type test(...);

    typedef decltype(test<T>(0)) type;
};

template<typename T>
struct is_container : public __is_container_helper<T>::type
{ };

namespace __stream_helper
{
    template<typename U> static constexpr std::false_type operator<<(std::ostream&, U);
    template<typename U> static constexpr std::false_type operator>>(std::istream&, U);
    static std::iostream* _io;

    template<typename T>
    struct __has_stream_output
    {
        typedef typename std::is_same<std::ostream&, decltype((std::ostream&)(*_io) << std::declval<T>())>::type type;
    };

    template<typename T>
    struct __has_stream_input
    {
        typedef typename std::is_same<std::istream&, decltype((std::istream&)(*_io) >> std::declval<T&>())>::type type;
    };
}

template<typename T>
struct has_stream_output : public __stream_helper::__has_stream_output<T>::type
{ };

template<>
struct has_stream_output<std::string> : public std::true_type
{ };

template<>
struct has_stream_output<char> : public std::true_type
{ };

template<typename T>
struct has_stream_input : public __stream_helper::__has_stream_input<T>::type
{ };

template<>
struct has_stream_input<std::string> : public std::true_type
{ };

template<>
struct has_stream_input<char> : public std::true_type
{ };

template<typename T>
struct has_stream_io : public std::integral_constant<bool, has_stream_output<T>::value && has_stream_input<T>::value>
{ };
