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
 * @tparam _Rows Number of rows
 * @tparam _Columns Number of columns
 * @tparam _T Type of matrix element
 */
template <uint64_t _Rows, uint64_t _Columns, typename _T = double>
class matrix
{
private:
    std::array<std::array<_T, _Columns>, _Rows> m_array{};

public:
    using value_type = _T;

    using internal_matrix_t = decltype(m_array);

    using row_t = typename internal_matrix_t::value_type;

    using const_row_t = const row_t;


    /**
     * @brief Constructor for matrix class
     *
     */
    constexpr explicit matrix(bool i_identity = true) noexcept
    {
        for (auto row{ 0_ui64 }; row < Rows(); ++row)
        {
            for (auto col{ 0_ui64 }; col < Columns(); ++col)
            {
                if (row == col && i_identity)
                {
                    m_array[row][col] = static_cast<value_type>(1);
                }
                else
                {
                    m_array[row][col] = static_cast<value_type>(0);
                }

            }
        }
    }


    template <typename _Iter>
    constexpr matrix(_Iter i_begin, _Iter i_end) noexcept
    {
        auto idx{ 0_ui64 };

        for (auto iter{ i_begin }; iter != i_end && idx < Rows() * Columns(); ++iter, ++idx)
        {
            auto row{ idx / Columns() };
            auto col{ idx % Columns() };

            m_array[row][col] = std::move(*iter);
        }
    }


    /**
     * @brief Constructor for matrix class
     *
     * @param i_list Input initializer list
     */
    constexpr explicit matrix(std::initializer_list<value_type>&& i_list) noexcept
        : matrix{ std::begin(i_list), std::end(i_list) }
    {
    }


    /**
     * @brief Constructor for matrix class
     *
     * @param i_arr initializer array
     */
    constexpr explicit matrix(std::array<std::array<value_type, _Columns>, _Rows> i_arr) noexcept
        : m_array{ std::move(i_arr) }
    {
    }


    /**
     * @brief get iterator to the start of the matrix
     *
     * @return iterator
     */
    constexpr typename internal_matrix_t::iterator begin() noexcept
    {
        return m_array.begin();
    }


    /**
     * @brief get iterator to beyond-the-end of the matrix
     *
     * @return iterator
     */
    constexpr typename internal_matrix_t::iterator end() noexcept
    {
        return m_array.end();
    }


    /**
     * @brief get constant iterator to the start of the matrix
     *
     * @return const iterator
     */
    constexpr typename internal_matrix_t::const_iterator begin() const noexcept
    {
        return m_array.begin();
    }


    /**
     * @brief get constant iterator to beyond-the-end of the matrix
     *
     * @return const iterator
     */
    constexpr typename internal_matrix_t::const_iterator end() const noexcept
    {
        return m_array.end();
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
    friend constexpr auto basic_binary_operator(const matrix& i_lhs, const matrix<_Nr, _Nc, value_type>& i_rhs, _Predicate&& binary_p) noexcept
    {
        static_assert(Rows() == _Nr, "Both matrices should have same number of rows!");
        static_assert(Columns() == _Nc, "Both matrices should have same number of columns!");

        auto newMat{ matrix{} };

        for (auto i{ 0_ui64 }; i < Rows(); i++)
        {
            for (auto j{ 0_ui64 }; j < Columns(); j++)
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
    friend constexpr auto operator*(const matrix& i_lhs, const matrix<_Nr, _Nc, value_type>& i_rhs) noexcept
    {
        static_assert(Columns() == _Nr, "Number of columns of first matrix should be equal to number of rows of second matrix!");

        auto mat{ matrix<Rows(), _Nc, value_type>{ 0 } };

        for (auto i{ 0_ui64 }; i < Rows(); ++i)
        {
            for (auto j{ 0_ui64 }; j < _Nc; ++j)
            {
                for (auto k{ 0_ui64 }; k < Columns(); ++k)
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
    friend constexpr auto operator+(const matrix& i_lhs, const matrix<_Nr, _Nc, value_type>& i_rhs) noexcept
    {
        static_assert(Rows() == _Nr, "Both matrices should have same number of rows!");
        static_assert(Columns() == _Nc, "Both matrices should have same number of columns!");

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
    friend constexpr auto operator-(const matrix& i_lhs, const matrix<_Nr, _Nc, value_type>& i_rhs) noexcept
    {
        static_assert(Rows() == _Nr, "Both matrices should have same number of rows!");
        static_assert(Columns() == _Nc, "Both matrices should have same number of columns!");

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
    constexpr auto operator+=(const matrix<_Nr, _Nc, value_type>& i_matrix) noexcept
    {
        static_assert(Rows() == _Nr, "Both matrices should have same number of rows!");
        static_assert(Columns() == _Nc, "Both matrices should have same number of columns!");

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
    constexpr auto operator-=(const matrix<_Nr, _Nc, value_type>& i_matrix) noexcept
    {
        static_assert(Rows() == _Nr, "Both matrices should have same number of rows!");
        static_assert(Columns() == _Nc, "Both matrices should have same number of columns!");

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
    constexpr auto operator*=(const matrix<_Nr, _Nc, value_type>& i_matrix) noexcept
    {
        static_assert(matrix::IsSquare(), "Original matrix needs to be a square matrix!");
        static_assert(RAW(decltype(i_matrix))::IsSquare(), "Input matrix needs to be a square matrix!");
        static_assert(Rows() == _Nr, "Both matrices need to be of same order!");

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
    friend constexpr bool operator==(const matrix& i_lhs, const matrix<_Nr, _Nc, value_type>& i_rhs) noexcept
    {
        if constexpr (Rows() == _Nr && Columns() == _Nc)
        {

#if __cpp_lib_array_constexpr >= 201811
            return i_lhs.m_array == i_rhs.m_array;
#else
            auto equal{ true };

            for (auto row{ 0_ui64 }; row < Rows() && equal; ++row)
            {
                for (auto col{ 0_ui64 }; col < Columns() && equal; ++col)
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
    friend constexpr bool operator!=(const matrix& i_lhs, const matrix<_Nr, _Nc, value_type>& i_rhs) noexcept
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
        auto transpose{ matrix<Columns(), Rows(), value_type>{ false } };

        for (auto i{ 0_ui64 }; i < Rows(); ++i)
        {
            for (auto j{ 0_ui64 }; j < Columns(); ++j)
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
     * @brief Check if matrix is square
     *
     * @return true if matrix is square
     */
    __CONSTEVAL static auto IsSquare() noexcept
    {
        return Rows() == Columns();
    }


    /**
     * @brief Elements leftover after excluding a row and column
     *
     * @param i_row Row to exclude
     * @param i_col Column to exclude
     * @return matrix form of leftover elements
     */
    constexpr auto leftover_elements(uint64_t i_row, uint64_t i_col) const noexcept
    {
        static_assert(IsSquare() && Rows() > 2_ui64, "matrix has to be a square matrix of minimum order 3");

        constexpr auto new_rows = Rows() - 1_ui64;
        constexpr auto new_cols = Columns() - 1_ui64;

        auto leftover_eles{ std::array<value_type, new_rows* new_cols>{} };

        auto iter{ leftover_eles.begin() };

        for (auto r{ 0_ui64 }; r < Rows(); ++r)
        {
            for (auto c{ 0_ui64 }; c < Columns(); ++c)
            {
                if (r != i_row && c != i_col)
                {
                    *iter = m_array[r][c];
                    ++iter;
                }
            }
        }

        return matrix<new_rows, new_cols, value_type>{ leftover_eles.begin(), leftover_eles.end() };
    }


    /**
     * @brief Calculate determinant of square matrix
     *
     * @return Value of determinant
     */
    constexpr auto determinant() const noexcept
    {
        static_assert(matrix::IsSquare(), "matrix has to be a square matrix!");

        if constexpr (Columns() == 1_ui64)
        {
            return m_array[0][0];
        }
        else if constexpr (Columns() == 2_ui64)
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


    /**
     * @brief Calculate adjoint of the matrix
     *
     * @return adjoint of the matrix
     */
    constexpr auto adjoint() const noexcept
    {
        static_assert(matrix::IsSquare(), "matrix has to be a square matrix!");

        auto cofactor_matrix{ matrix{} };

        for (auto row{ 0_ui64 }; row < Rows(); ++row)
        {
            for (auto col{ 0_ui64 }; col < Columns(); ++col)
            {
                auto multiplier{ (row + col) % 2 == 0 ? 1 : -1 };

                cofactor_matrix[row][col] = multiplier * leftover_elements(row, col).determinant();
            }
        }

        return ~cofactor_matrix;
    }


    /**
     * @brief Calculate the inverse of the matrix
     *
     * @return inverse of the matrix
     */
    constexpr auto inverse() const noexcept
    {
        static_assert(matrix::IsSquare(), "matrix has to be a square matrix!");

        auto det{ determinant() };

        assert(det != static_cast<decltype(det)>(0.0));

        if (det == static_cast<decltype(det)>(0.0))
        {
            return matrix{};
        }

        if constexpr (Columns() == 1_ui64)
        {
            return matrix{ 1.0 / det };
        }
        else if constexpr (Columns() == 2_ui64)
        {
            auto a{ (*this)[0][0] };
            auto b{ (*this)[0][1] };
            auto c{ (*this)[1][0] };
            auto d{ (*this)[1][1] };

            return matrix{ d / det, -b / det, -c / det, a / det };
        }
        else
        {
            auto adj{ adjoint() };

            auto inv{ matrix{} };

            for (auto row{ 0_ui64 }; row < Rows(); ++row)
            {
                for (auto col{ 0_ui64 }; col < Columns(); ++col)
                {
                    inv[row][col] = adj[row][col] / det;
                }
            }

            return inv;
        }
    }

    static_assert(std::is_floating_point_v<value_type>, "Contained element needs to be a valid matrix type!");
    static_assert(Rows() != 0 && Columns() != 0, "Rows and columns have to be non-zero!");
};


template <std::uint64_t _Order, typename _T = double>
constexpr static auto IdentityMatrix = matrix<_Order, _Order, _T>{};


/**
 * @brief Product type of two matrices
 *
 * @tparam _M1 First matrix type
 * @tparam _M2 Second matrix type
 */
template <typename _M1, typename _M2>
using matrix_product_t = std::enable_if_t<IsMatrix<_M1>&& IsMatrix<_M2> && (_M1::Columns() == _M2::Rows()), matrix<_M1::Rows(), _M2::Columns(), typename _M1::value_type>>;


/**
 * @brief Type of transpose matrix
 *
 * @tparam _M original matrix type
 */
template <typename _M>
using matrix_transpose_t = std::enable_if_t<IsMatrix<_M>, matrix<_M::Columns(), _M::Rows(), typename _M::value_type>>;


using Matrix2x2 = matrix<2, 2>;
using Matrix3x3 = matrix<3, 3>;
using Matrix4x4 = matrix<4, 4>;
