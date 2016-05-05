#pragma once

#include <type_traits>
#include <functional>

#if (!defined(_MSC_VER) && __cplusplus < 201402L) || _MSC_VER < 1900

namespace std
{
    template<typename T, T... Ints>
    class integer_sequence
    {
        static_assert(is_integral<T>::value, "T must be integral");

        typedef T value_type;

        static constexpr std::size_t size()
        {
            return sizeof...(Ints);
        }
    };

    namespace _
    {
        template<bool isNeg, bool isZero, typename T, T Current, T... Values>
        struct make_int_seq
        {
            static_assert(isNeg, "Negative size integer sequence");
        };

        template<typename T, T Current, T... Values>
        struct make_int_seq<false, true, T, Current, Values...> : public integer_sequence<T, Values...>
        { };

        template<typename T, T Current, T... Values>
        struct make_int_seq<false, false, T, Current, Values...> : public make_int_seq < Current < 0, Current == 1, T, Current - 1, Current - 1, Values...>
        { };
    }

    template<typename T, T N>
    using make_integer_sequence = _::make_int_seq<N < 0, N == 0, T, N>;
}

#endif

namespace my_std
{
    template<int N>
    struct placeholder
    {
        static placeholder ph;
    };

    template<int N>
    placeholder<N> placeholder<N>::ph;

    namespace _
    {
        template<typename TVal, typename TRet, typename TArg1, typename... TArgs, int... Values>
        std::function<TRet(TArgs...)> bind_first(std::function<TRet(TArg1, TArgs...)> func, TVal val, std::integer_sequence<int, Values...>)
        {
            return std::bind(func, val, placeholder<Values + 1>::ph...);
        }
    }

    template<typename TVal, typename TRet, typename TArg1, typename... TArgs>
    std::function<TRet(TArgs...)> bind_first(const std::function<TRet(TArg1, TArgs...)>& func, TVal val)
    {
        return _::bind_first(func, val, std::make_integer_sequence<int, sizeof...(TArgs)>());
    }

    template<typename TVal, typename TRet, typename TArg1, typename... TArgs>
    std::function<TRet(TArgs...)> bind_first(TRet (*func)(TArg1, TArgs...), TVal val)
    {
        return _::bind_first(std::function<TRet(TArg1, TArgs...)>(func), val, std::make_integer_sequence<int, sizeof...(TArgs)>());
    }
}

namespace std
{
    template<int N>
    struct is_placeholder<my_std::placeholder<N>> : public std::integral_constant<int, N>
    { };
}