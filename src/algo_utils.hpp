/**
 * @file algo_utils.hpp
 * @author ashwinn76
 * @brief Utility algorithms
 * @version 0.1
 * @date 2020-08-22
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "type_trait_utils.hpp"

#include <random>

template <typename _T>
auto get_char_address(_T&& object)
{
    return (char*) std::addressof(object);
}

#define IO_INFORMATION(x) get_char_address(x), sizeof(std::remove_cv_t<std::remove_reference_t<decltype(x)>>)


template <typename _T>
constexpr auto in_range(const _T& value, const _T& value1, const _T& value2)
{
    return std::min(value1, value2) <= value && value <= std::max(value, value2);
}


template <auto _Min, decltype(_Min) _Max>
constexpr auto GetRandomValue() noexcept
{
    using _Type = decltype(_Min);

    static_assert(_Min <= _Max, "Min has to be lesser than Max");

    constexpr auto time_str_to_int = [](auto&& char_str)
    {
        auto integer{ static_cast<int>(*char_str - 48) * 10 + static_cast<int>(*(char_str + 1) - 48) };

        if (integer <= 0)
        {
            integer = 1;
        }

        return integer;
    };

    constexpr auto time_str{ __TIME__ };

    constexpr auto hours{ time_str_to_int(time_str) };

    constexpr auto minutes{ time_str_to_int(time_str + 3) };

    constexpr auto seconds{ time_str_to_int(time_str + 6) };

    auto diff{ _Max - _Min };

    return _Min + static_cast<_Type>(diff * (static_cast<double>(hours * minutes * seconds) / (24.0 * 60.0 * 60.0)));
}


template <typename _T> requires (std::is_integral_v<_T> || std::is_enum_v<_T>)
constexpr auto GetRandomValue(_T i_min, _T i_max)
{
    using my_gen = std::mt19937_64;

    auto generator{ my_gen{ static_cast<my_gen::result_type>(std::rand()) } };

    auto min{ static_cast<long long>(i_min) };
    auto max{ static_cast<long long>(i_max) };

    auto distributor{ std::uniform_int_distribution{ min, max } };

    return static_cast<decltype(i_min)>(distributor(generator));
}


/**
 * @brief Get randomized value within the limits set by the type of the input parameter.
 *
 * @tparam T Template type constrained by IsBound concept.
*/
template <typename _T> requires (IsBound<_T>)
constexpr auto GetRandomValue()
{
    if (std::is_constant_evaluated())
    {
        return _T{ GetRandomValue<_T::min(), _T::max()>() };
    }
    else
    {
        return _T{ GetRandomValue(_T::min(), _T::max()) };
    }
}
