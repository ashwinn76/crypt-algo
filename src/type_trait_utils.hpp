/**
 * @file type_trait_utils.hpp
 * @author ashwinn76
 * @brief Utility functions using type_traits and concepts
 * @version 0.1
 * @date 2020-08-22
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <concepts>

#include "macro_utils.hpp"


template <typename _T>
concept BoundType = std::equality_comparable<_T> && std::totally_ordered<_T>;


 // Is Bound concept and type trait

 /**
  * @brief Constraint to check whether type limits are bound
  *
  * @tparam _T Template type
  */
template <typename _T>
concept IsBound = requires
{
    requires BoundType<typename _T::value_type>;

    { _T::min() } noexcept -> std::same_as<typename _T::value_type>;
    { _T::max() } noexcept -> std::same_as<typename _T::value_type>;
};


template <typename, typename = std::void_t<>, typename = std::void_t<>>
struct is_bound : std::false_type {};

template <typename _T>
struct is_bound<
    _T,
    std::enable_if_t<std::is_same_v<std::invoke_result_t<decltype(&_T::min)>, typename _T::value_type> && noexcept(_T::min())>,
    std::enable_if_t<std::is_same_v<std::invoke_result_t<decltype(&_T::max)>, typename _T::value_type> && noexcept(_T::max())>> : std::true_type{};


/**
 * @brief type trait to check whether type limits are bound
 *
 * @tparam _T template type to check
 */
template <typename _T>
constexpr auto inline is_bound_v = is_bound<_T>::value;


/**
 * @brief Constraint to check if type can be stored in a matrix
 *
 * @tparam _T Type to check
 */
template <typename _T>
concept MatrixElement = requires(_T x)
{
    std::default_initializable<_T>;

    { x + x } -> std::same_as<_T>;
    { x - x } -> std::same_as<_T>;
    { x * x } -> std::same_as<_T>;
};


/**
 * @brief Constraint to check if type is matrix
 *
 * @tparam _T Type to check
 */
template <typename _T>
concept IsMatrix = requires (_T x)
{
    MatrixElement<typename std::remove_cvref_t<_T>::value_type>;

    { _T::Rows() } noexcept -> std::same_as<uint64_t>;
    { _T::Columns() } noexcept -> std::same_as<uint64_t>;

    { x + x } -> std::convertible_to<_T>;
    { x - x } -> std::convertible_to<_T>;
};
