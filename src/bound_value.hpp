/**
 * @file bound_value.hpp
 * @author ashwinn76
 * @brief This file contains an implementation of a generic wrapper that can hold an object within specified limits 
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include <stdexcept>

#include "type_trait_utils.hpp"
#include "algo_utils.hpp"


template <BoundType auto _Min, decltype(_Min) _Max>
class BoundValue
{
public:
    using value_type = decltype(_Min);

private:
    static_assert(_Min < _Max, "Minimum value cannot be greater than Maximum!");

    value_type m_value{ _Min };

    constexpr friend bool operator==(const BoundValue& i_lhs, const value_type& i_rhs) noexcept
    {
        return i_lhs.value() == i_rhs;
    }


    constexpr friend bool operator==(const value_type& i_lhs, const BoundValue& i_rhs) noexcept
    {
        return i_lhs == i_rhs.value();
    }


    constexpr friend bool operator==(const BoundValue& i_lhs, const BoundValue& i_rhs) noexcept
    {
        return i_lhs.value() == i_rhs.value();
    }

public:
    constexpr BoundValue() noexcept = default;

    constexpr explicit BoundValue(value_type i_value)
        : m_value{ i_value }
    {
        if (!in_range(m_value, _Min, _Max))
        {
            throw std::out_of_range{ "Value out of bounds!" };
        }
    }


    constexpr static value_type min() noexcept
    {
        return _Min;
    }


    constexpr static value_type max() noexcept
    {
        return _Max;
    }


    constexpr value_type value() const noexcept
    {
        return m_value;
    }


    constexpr operator value_type() const noexcept
    {
        return value();
    }
};