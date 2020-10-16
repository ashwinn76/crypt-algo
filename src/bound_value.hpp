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

 /**
  * @brief a wrapper class to contain a bound value
  *
  * @tparam _Min minimum bound
  * @tparam _Max maximum bound
  */
template <auto _Min, decltype(_Min) _Max>
class bound_value
{
public:
    using value_type = decltype(_Min);

private:
    static_assert(_Min < _Max, "Minimum value cannot be greater than Maximum!");

    value_type m_value{ _Min };   // contained value

    /**
     * @brief Equality operator
     *
     * @param i_lhs bound value wrapper object
     * @param i_rhs raw object
     * @return true if raw object and contained object are equal
     */
    constexpr friend auto operator==(const bound_value& i_lhs, const value_type& i_rhs) noexcept
    {
        return i_lhs.value() == i_rhs;
    }


    /**
     * @brief Equality operator
     *
     * @param i_lhs raw object
     * @param i_rhs bound value wrapper object
     * @return true if raw object and contained object are equal
     */
    constexpr friend auto operator==(const value_type& i_lhs, const bound_value& i_rhs) noexcept
    {
        return i_lhs == i_rhs.value();
    }


    /**
     * @brief Equality operator
     *
     * @param i_lhs first bound value wrapper object
     * @param i_rhs second bound value wrapper object
     * @return true if both objects are equal
     */
    constexpr friend auto operator==(const bound_value& i_lhs, const bound_value& i_rhs) noexcept
    {
        return i_lhs.value() == i_rhs.value();
    }

public:
    /**
     * @brief Default constructor
     *
     */
    constexpr bound_value() noexcept = default;


    /**
     * @brief Parameterized constructor
     *
     * @param i_value input value
     */
    constexpr explicit bound_value(value_type i_value)
        : m_value{ i_value }
    {
        if (!in_range(m_value, _Min, _Max))
        {
            throw std::out_of_range{ "Value out of bounds!" };
        }
    }


    /**
     * @brief get the minimum allowed value
     *
     * @return minimum value
     */
    constexpr static auto min() noexcept
    {
        return _Min;
    }


    /**
     * @brief get the maximum allowed value
     *
     * @return maximum value
     */
    constexpr static auto max() noexcept
    {
        return _Max;
    }


    /**
     * @brief get the contained value
     *
     * @return contained value
     */
    constexpr auto value() const noexcept
    {
        return m_value;
    }


    /**
     * @brief explicit conversion operator for contained value_type
     *
     * @return value_type
     */
    constexpr operator value_type() const noexcept
    {
        return value();
    }
};