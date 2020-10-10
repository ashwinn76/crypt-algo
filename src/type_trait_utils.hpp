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

#ifdef __CONCEPTS

 /**
  * @brief Constraint to check whether type limits are bound
  *
  * @tparam _T Template type
  */
template <typename _T>
concept IsBound = requires
{
    { RAW(_T)::min() } noexcept -> std::same_as<typename RAW(_T)::value_type>;
    { RAW(_T)::max() } noexcept -> std::same_as<typename RAW(_T)::value_type>;
};

#else

template <typename, typename = std::void_t<>, typename = std::void_t<>>
struct is_bound : std::false_type {};

template <typename _T>
struct is_bound<
    _T,
    std::enable_if_t<std::is_same_v<std::invoke_result_t<decltype(&RAW(_T)::min)>, typename _T::value_type> && noexcept(RAW(_T)::min())>,
    std::enable_if_t<std::is_same_v<std::invoke_result_t<decltype(&RAW(_T)::max)>, typename _T::value_type> && noexcept(RAW(_T)::max())>> : std::true_type{};


/**
 * @brief type trait to check whether type limits are bound
 *
 * @tparam _T template type to check
 */
template <typename _T>
constexpr auto inline IsBound = is_bound<RAW(_T)>::value;

#endif


#ifdef __CONCEPTS

/**
 * @brief Constraint to check if type can be stored in a matrix
 *
 * @tparam _T Type to check
 */
template <typename _T>
concept MatrixElement = requires(_T x)
{
    std::default_initializable<RAW(_T)>;

    { x + x } -> std::same_as<RAW(_T)>;
    { x - x } -> std::same_as<RAW(_T)>;
    { x * x } -> std::same_as<RAW(_T)>;
};


/**
 * @brief Constraint to check if type is matrix
 *
 * @tparam _T Type to check
 */
template <typename _T>
concept IsMatrix = requires (_T x)
{
    MatrixElement<typename RAW(_T)::value_type>;

    { RAW(_T)::Rows() } noexcept -> std::same_as<uint64_t>;
    { RAW(_T)::Columns() } noexcept -> std::same_as<uint64_t>;

    { x + x } -> std::convertible_to<_T>;
    { x - x } -> std::convertible_to<_T>;
};


/**
 * @brief Constraint to check if matrix is square
 * 
 * @tparam _T Type to check
 */
template <typename _T>
concept IsSquareMatrix = IsMatrix<RAW(_T)> && (RAW(_T)::Rows() == RAW(_T)::Columns());

#else


template <typename, typename = std::void_t<>, typename = std::void_t<>, typename = std::void_t<>, typename = std::void_t<>>
struct is_matrix_element : std::false_type{};

template <typename _T>
struct is_matrix_element<
    _T,
    std::enable_if_t<std::is_default_constructible_v<RAW(_T)>>,
    std::enable_if_t<std::is_same_v<decltype(std::declval<RAW(_T)>() + std::declval<RAW(_T)>()), RAW(_T)>>,
    std::enable_if_t<std::is_same_v<decltype(std::declval<RAW(_T)>() - std::declval<RAW(_T)>()), RAW(_T)>>,
    std::enable_if_t<std::is_same_v<decltype(std::declval<RAW(_T)>() * std::declval<RAW(_T)>()), RAW(_T)>>> : std::true_type{};

/**
 * @brief Type trait to check if type can be stored in a matrix
 * 
 * @tparam _T Type to check
 */
template <typename _T>
constexpr auto inline MatrixElement = is_matrix_element<RAW(_T)>::value;


template <typename _T, typename = std::void_t<>, typename = std::void_t<>, typename = std::void_t<>, typename = std::void_t<>, typename = std::void_t<>>
struct is_matrix_type : std::false_type{};

template <typename _T>
struct is_matrix_type<
    _T,
    std::enable_if_t<MatrixElement<typename RAW(_T)::value_type>>,
    std::enable_if_t<std::is_same_v<std::invoke_result_t<decltype(&(RAW(_T)::Rows))>, uint64_t> && noexcept(RAW(_T)::Rows())>,
    std::enable_if_t<std::is_same_v<std::invoke_result_t<decltype(&(RAW(_T)::Columns))>, uint64_t> && noexcept(RAW(_T)::Columns())>,
    std::enable_if_t<std::is_convertible_v<decltype(std::declval<RAW(_T)>() + std::declval<RAW(_T)>()), _T>>,
    std::enable_if_t<std::is_convertible_v<decltype(std::declval<RAW(_T)>() - std::declval<RAW(_T)>()), _T>>> : std::true_type{};

/**
 * @brief Type trait to check if type is matrix-ish
 * 
 * @tparam _T Type to check
 */
template <typename _T>
constexpr auto inline IsMatrix = is_matrix_type<RAW(_T)>::value;

/**
 * @brief Type trait to check if matrix is square
 * 
 * @tparam _T Type to check
 */
template <typename _T>
constexpr auto inline IsSquareMatrix = IsMatrix<RAW(_T)> && (RAW(_T)::Columns() == RAW(_T)::Rows());

#endif
