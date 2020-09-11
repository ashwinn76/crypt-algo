/**
 * @file matrix.hpp
 * @author ashwinn76
 * @brief Implementation of a generic matrix class
 * @version 0.1
 * @date 2020-08-22
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <array>
#include <concepts>

#include <ranges>

#include "type_trait_utils.hpp"


 /**
  * @brief Generic matrix class
  *
  * @tparam _T Type of matrix element
  * @tparam _Rows Number of rows
  * @tparam _Columns Number of columns
  */
template <MatrixElement _T, uint64_t _Rows, uint64_t _Columns> requires (_Rows != 0_ui64 && _Columns != 0_ui64)
class Matrix
{
private:
    std::array<std::array<_T, _Columns>, _Rows> m_array{};

public:
    using value_type = _T;

    using row_t = typename decltype(m_array)::value_type;

    using const_row_t = const row_t;

    /**
     * @brief Default constructor for Matrix class
     *
     */
    constexpr explicit Matrix() noexcept = default;


    /**
     * @brief Constructor for Matrix class
     *
     * @param i_list Input initializer list
     */
    constexpr explicit Matrix(std::initializer_list<_T>&& i_list) noexcept
    {
        auto idx{ 0_ui64 };

        for (auto iter{ std::begin(i_list) }; iter != std::end(i_list) && idx < _Rows * _Columns; ++iter, ++idx)
        {
            auto row{ idx / _Columns };
            auto col{ idx % _Columns };

            m_array[row][col] = std::move(*iter);
        }
    }


    /**
     * @brief Constructor for Matrix class
     *
     */
    constexpr explicit Matrix(std::array<std::array<_T, _Columns>, _Rows> arr) noexcept : m_array{ std::move(arr) }
    {
    }


    /**
     * @brief Generic binary operator
     *
     * @tparam _Nr Number of rows in the second matrix
     * @tparam _Nc Number of columns in the second matrix
     * @tparam _Predicate Binary Predicate Type
     * @param i_lhs First matrix
     * @param i_rhs Second matrix
     * @param binary_p Binary predicate
     * @return Binary operation result
     */
    template <uint64_t _Nr, uint64_t _Nc, typename _Predicate>
    friend constexpr auto basic_binary_operator(const Matrix<_T, _Rows, _Columns>& i_lhs, const Matrix<_T, _Nr, _Nc>& i_rhs, _Predicate&& binary_p) noexcept
    {
        static_assert(_Rows == _Nr, "Both matrices should have same number of rows!");
        static_assert(_Columns == _Nc, "Both matrices should have same number of columns!");

        auto newMat{ Matrix<_T, _Rows, _Columns>{} };

        for (auto i{ 0_ui64 }; i < _Rows; i++)
        {
            for (auto j{ 0_ui64 }; j < _Columns; j++)
            {
                newMat[i][j] = std::invoke(binary_p, i_lhs[i][j], i_rhs[i][j]);
            }
        }

        return newMat;
    }


    /**
     * @brief Multiply two matrices
     *
     * @tparam _Nr Number of rows in the second matrix
     * @tparam _Nc Number of columns in the second matrix
     * @param i_lhs First matrix
     * @param i_rhs Second matrix
     * @return Product of multiplication
     */
    template <uint64_t _Nr, uint64_t _Nc>
    friend constexpr auto operator*(const Matrix<_T, _Rows, _Columns>& i_lhs, const Matrix<_T, _Nr, _Nc>& i_rhs) noexcept
    {
        static_assert(_Columns == _Nr, "Number of columns of first matrix should be equal to number of rows of second matrix!");

        auto mat{ Matrix<_T, _Rows, _Nc>{} };

        for (auto i{ 0_ui64 }; i < _Rows; ++i)
        {
            for (auto j{ 0_ui64 }; j < _Nc; ++j)
            {
                for (auto k{ 0_ui64 }; k < _Columns; ++k)
                {
                    mat[i][j] += (i_lhs[i][k] * i_rhs[k][j]);
                }
            }
        }

        return mat;
    }


    /**
     * @brief Adds two matrices and return the sum
     *
     * @tparam _Nr Number of rows in the second matrix
     * @tparam _Nc Number of columns in the second matrix
     * @param i_lhs First matrix
     * @param i_rhs Second matrix
     * @return Sum of input matrices
     */
    template <uint64_t _Nr, uint64_t _Nc>
    friend constexpr auto operator+(const Matrix<_T, _Rows, _Columns>& i_lhs, const Matrix<_T, _Nr, _Nc>& i_rhs) noexcept
    {
        static_assert(_Rows == _Nr, "Both matrices should have same number of rows!");
        static_assert(_Columns == _Nc, "Both matrices should have same number of columns!");

        return basic_binary_operator(i_lhs, i_rhs, [](auto&& l_ele, auto&& r_ele)
        {
            return l_ele + r_ele;
        });
    }


    /**
     * @brief Subtracts two matrices and return the result
     *
     * @tparam _Nr Number of rows in the second matrix
     * @tparam _Nc Number of columns in the second matrix
     * @param i_lhs First matrix
     * @param i_rhs Second matrix
     * @return Difference of input matrices
     */
    template <uint64_t _Nr, uint64_t _Nc>
    friend constexpr auto operator-(const Matrix<_T, _Rows, _Columns>& i_lhs, const Matrix<_T, _Nr, _Nc>& i_rhs) noexcept
    {
        static_assert(_Rows == _Nr, "Both matrices should have same number of rows!");
        static_assert(_Columns == _Nc, "Both matrices should have same number of columns!");

        return basic_binary_operator(i_lhs, i_rhs, [](auto&& l_ele, auto&& r_ele)
        {
            return l_ele - r_ele;
        });
    }


    /**
     * @brief Adds new matrix to original matrix
     *
     * @tparam _Nr Number of rows in the second matrix
     * @tparam _Nc Number of columns in the second matrix
     * @param i_matrix new matrix
     * @return matrix after addition
     */
    template <uint64_t _Nr, uint64_t _Nc>
    constexpr auto operator+=(const Matrix<_T, _Nr, _Nc>& i_matrix) noexcept
    {
        static_assert(_Rows == _Nr, "Both matrices should have same number of rows!");
        static_assert(_Columns == _Nc, "Both matrices should have same number of columns!");

        *this = (*this + i_matrix);

        return *this;
    }


    /**
     * @brief Subtracts new matrix from original matrix
     *
     * @tparam _Nr Number of rows in the second matrix
     * @tparam _Nc Number of columns in the second matrix
     * @param i_matrix new matrix
     * @return resultant matrix
     */
    template <uint64_t _Nr, uint64_t _Nc>
    constexpr auto operator-=(const Matrix<_T, _Nr, _Nc>& i_matrix) noexcept
    {
        static_assert(_Rows == _Nr, "Both matrices should have same number of rows!");
        static_assert(_Columns == _Nc, "Both matrices should have same number of columns!");

        *this = (*this - i_matrix);

        return *this;
    }


    /**
     * @brief Multiply matrix with current matrix
     *
     * @tparam _Nr Number of rows in the second matrix
     * @tparam _Nc Number of columns in the second matrix
     * @param i_matrix new matrix
     * @return resultant matrix
     */
    template <uint64_t _Nr, uint64_t _Nc>
    constexpr auto operator*=(const Matrix<_T, _Nr, _Nc>& i_matrix) noexcept
    {
        static_assert(_Rows == _Columns, "Original matrix needs to be a square matrix!");
        static_assert(_Nr == _Nc, "Input matrix needs to be a square matrix!");
        static_assert(_Rows == _Nr, "Both matrices need to be of same order!");

        *this = (*this * i_matrix);

        return *this;
    }


    /**
     * @brief Check if matrices are equal
     *
     * @tparam _Nr Number of rows in the second matrix
     * @tparam _Nc Number of columns in the second matrix
     * @param i_lhs First matrix
     * @param i_rhs Second matrix
     * @return true if matrices are equal
     * @return false if matrices are not equal
     */
    template <uint64_t _Nr, uint64_t _Nc>
    friend constexpr bool operator==(const Matrix<_T, _Rows, _Columns>& i_lhs, const Matrix<_T, _Nr, _Nc>& i_rhs) noexcept
    {
        if constexpr (_Rows == _Nr && _Columns == _Nc)
        {
            return i_lhs.m_array == i_rhs.m_array;
        }

        return false;
    }


    /**
     * @brief Check if matrices are not equal
     *
     * @tparam _Nr Number of rows in the second matrix
     * @tparam _Nc Number of columns in the second matrix
     * @param i_lhs First matrix
     * @param i_rhs Second matrix
     * @return true if matrices are not equal
     * @return false if matrices are equal
     */
    template <uint64_t _Nr, uint64_t _Nc>
    friend constexpr bool operator!=(const Matrix<_T, _Rows, _Columns>& i_lhs, const Matrix<_T, _Nr, _Nc>& i_rhs) noexcept
    {
        return !(i_lhs == i_rhs);
    }


    /**
     * @brief Get the row by index
     *
     * @param i_row input row index
     * @return row of elements
     */
    constexpr row_t& operator[](const uint64_t i_row)
    {
        return m_array[i_row];
    }


    /**
     * @brief Get constant row by index
     *
     * @param i_row input row index
     * @return const row of elements
     */
    constexpr const_row_t& operator[](const uint64_t i_row) const
    {
        return m_array[i_row];
    }


    constexpr auto operator~() const noexcept
    {
        auto transpose{ Matrix<_T, _Columns, _Rows>{} };

        for (auto i{ 0_ui64 }; i < _Rows; ++i)
        {
            for (auto j{ 0_ui64 }; j < _Columns; ++j)
            {
                transpose[j][i] = m_array[i][j];
            }
        }

        return transpose;
    }


    /**
     * @brief Number of rows in the matrix
     *
     * @return number of rows
     */
    constexpr static auto Rows() noexcept
    {
        return _Rows;
    }


    /**
     * @brief Number of columns in the matrix
     *
     * @return number of columns
     */
    constexpr static auto Columns() noexcept
    {
        return _Columns;
    }


    /**
     * @brief Print out elements of matrix
     *
     */
    void print() const noexcept requires requires(_T x) { std::cout << x; }
    {
        for (auto&& row : m_array)
        {
            for (auto&& ele : row)
            {
                std::cout << ele << "   ";
            }

            std::cout << std::endl;
        }
    }
};


/**
 * @brief Product type of two matrices
 *
 * @tparam _M1 First matrix type
 * @tparam _M2 Second matrix type
 */
template <IsMatrix _M1, IsMatrix _M2>
using matrix_product_t = std::enable_if_t<_M1::Columns() == _M2::Rows(), Matrix<typename _M1::value_type, _M1::Rows(), _M2::Columns()>>;


/**
 * @brief Type of transpose matrix
 *
 * @tparam _M original matrix type
 */
template <IsMatrix _M>
using matrix_transpose_t = Matrix<typename _M::value_type, _M::Columns(), _M::Rows()>;
