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

#include "type_trait_utils.hpp"


 /**
  * @brief Generic matrix class
  *
  * @tparam _T Type of matrix element
  * @tparam _Rows Number of rows
  * @tparam _Columns Number of columns
  */
template <typename _T, uint64_t _Rows, uint64_t _Columns>
class Matrix
{
private:
    std::array<std::array<_T, _Columns>, _Rows> m_array{};

public:
    using value_type = _T;

    using row_t = typename decltype(m_array)::value_type;

    using const_row_t = const row_t;

    static_assert(MatrixElement<value_type>, "Contained element needs to be a valid matrix type!");
    static_assert(_Rows != 0 && _Columns != 0, "Rows and columns have to be non-zero!");

    /**
     * @brief Default constructor for Matrix class
     *
     */
    constexpr explicit Matrix() noexcept = default;


    template <typename _Iter>
    constexpr Matrix(_Iter i_begin, _Iter i_end) noexcept
    {
        auto idx{ 0_ui64 };

        for (auto iter{ i_begin }; iter != i_end && idx < _Rows * _Columns; ++iter, ++idx)
        {
            auto row{ idx / _Columns };
            auto col{ idx % _Columns };

            m_array[row][col] = std::move(*iter);
        }
    }


    /**
     * @brief Constructor for Matrix class
     *
     * @param i_list Input initializer list
     */
    constexpr explicit Matrix(std::initializer_list<_T>&& i_list) noexcept
        : Matrix{ std::begin(i_list), std::end(i_list) }
    {
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
                newMat[i][j] = binary_p(i_lhs[i][j], i_rhs[i][j]);
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
        static_assert(IsSquareMatrix<Matrix>, "Original matrix needs to be a square matrix!");
        static_assert(IsSquareMatrix<decltype(i_matrix)>, "Input matrix needs to be a square matrix!");
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

#if __cpp_lib_array_constexpr >= 201811
            return i_lhs.m_array == i_rhs.m_array;
#else
            auto equal{ true };

            for (auto row{ 0_ui64 }; row < _Rows && equal; ++row)
            {
                for (auto col{ 0_ui64 }; col < _Columns && equal; ++col)
                {
                    equal &= (i_lhs.m_array[row][col] == i_rhs.m_array[row][col]);
                }
            }

            return equal;
#endif
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


    /**
     * @brief transpose operator
     *
     * @return transpose of the matrix
     */
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
    __CONSTEVAL static auto Rows() noexcept
    {
        return _Rows;
    }


    /**
     * @brief Number of columns in the matrix
     *
     * @return number of columns
     */
    __CONSTEVAL static auto Columns() noexcept
    {
        return _Columns;
    }


    /**
     * @brief Elements leftover after excluding a row and column
     * 
     * @param i_row Row to exclude
     * @param i_col Column to exclude
     * @return Matrix form of leftover elements
     */
    constexpr auto leftover_elements(uint64_t i_row, uint64_t i_col) const noexcept
    {
        static_assert(IsSquareMatrix<Matrix> && _Rows > 2_ui64, "Matrix has to be a square matrix of minimum order 3");

        constexpr auto new_rows = _Rows - 1_ui64;
        constexpr auto new_cols = _Columns - 1_ui64;

        auto leftover_eles{ std::array<_T, new_rows* new_cols>{} };

        auto iter{ leftover_eles.begin() };

        for (auto r{ 0_ui64 }; r < _Rows; ++r)
        {
            for (auto c{ 0_ui64 }; c < _Columns; ++c)
            {
                if (r != i_row && c != i_col)
                {
                    *iter = m_array[r][c];
                    ++iter;
                }
            }
        }

        return Matrix<_T, new_rows, new_cols>{ leftover_eles.begin(), leftover_eles.end() };
    }


    /**
     * @brief Calculate determinant of square matrix
     * 
     * @return Value of determinant
     */
    constexpr auto determinant() const noexcept
    {
        static_assert(IsSquareMatrix<Matrix>, "Matrix has to be a square matrix!");

        if constexpr (_Columns == 1_ui64)
        {
            return m_array[0][0];
        }
        else if constexpr (_Columns == 2_ui64)
        {
            return ((m_array[0][0] * m_array[1][1]) - (m_array[0][1] * m_array[1][0]));
        }
        else
        {
            auto det{ 0.0l };

            constexpr auto row_num = 0_ui64;

            for (auto col{ 0_ui64 }; col < Columns(); ++col)
            {
                auto multiplier{ col % 2 == 0 ? 1 : -1 };

                auto leftover{ leftover_elements(row_num, col) };

                det += (m_array[row_num][col] * multiplier * leftover.determinant());
            }

            return det;
        }
    }
};


/**
 * @brief Product type of two matrices
 *
 * @tparam _M1 First matrix type
 * @tparam _M2 Second matrix type
 */
template <typename _M1, typename _M2>
using matrix_product_t = std::enable_if_t<IsMatrix<_M1>&& IsMatrix<_M2> && (_M1::Columns() == _M2::Rows()), Matrix<typename _M1::value_type, _M1::Rows(), _M2::Columns()>>;


/**
 * @brief Type of transpose matrix
 *
 * @tparam _M original matrix type
 */
template <typename _M>
using matrix_transpose_t = std::enable_if_t<IsMatrix<_M>, Matrix<typename _M::value_type, _M::Columns(), _M::Rows()>>;
