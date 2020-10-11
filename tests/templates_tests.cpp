/**
 * @file TemplatesTests.cpp
 * @author ashwinn76
 * @brief File with tests for generic algorithms, concepts and type traits
 * @version 0.1
 * @date 2020-08-23
 *
 * @copyright Copyright (c) 2020
 *
 */


#include "gtest/gtest.h"

#include "../src/type_trait_utils.hpp"
#include "../src/bound_value.hpp"

#include "../src/macro_utils.hpp"


#include "../src/matrix.hpp"

TEST(TemplatesTests, BoundValueTemplateTests)
{
    static_assert(IsBound<BoundValue<1, 2>>);
}


TEST(TemplatesTests, NumericConverterTests_64bit)
{
    static_assert(std::is_same_v<decltype(2_ui64), std::uint64_t>);
    static_assert(std::is_same_v<decltype(1_i64), std::int64_t>);
}


TEST(TemplatesTests, NumericConverterTests_32bit)
{
    static_assert(std::is_same_v<decltype(34_ui32), std::uint32_t>);
    static_assert(std::is_same_v<decltype(23_i32), std::int32_t>);
}


TEST(TemplatesTests, NumericConverterTests_16bit)
{
    static_assert(std::is_same_v<decltype(123_ui16), std::uint16_t>);
    static_assert(std::is_same_v<decltype(543_i16), std::int16_t>);
}


TEST(TemplatesTests, NumericConverterTests_8bit)
{
    static_assert(std::is_same_v<decltype(11_ui8), std::uint8_t>);
    static_assert(std::is_same_v<decltype(12_i8), std::int8_t>);
}


TEST(TemplatesTests, NotInRangeTests)
{
    static_assert(!in_range(1, 2, 3));
    static_assert(!in_range(1, 3, 2));
}


TEST(TemplatesTests, InRangeTests)
{
    static_assert(in_range(1, 0, 2));
    static_assert(in_range(1, 2, 0));

    static_assert(in_range(23, -90, 89));
    static_assert(in_range(23, 89, -90));

    static_assert(in_range(-1, 0, -2));
    static_assert(in_range(-1, -2, 0));
}


TEST(TemplatesTests, BoundValueMinMaxTests)
{
    static_assert(BoundValue<11, 22>::min() == 11);
    static_assert(BoundValue<23, 45>::max() == 45);

    static_assert(BoundValue<-23, -12>::min() == -23);
    static_assert(BoundValue<-11, -10>::max() == -10);
}


TEST(TemplatesTests, BoundValueEqualityTests)
{
    using sample_bound = BoundValue<12, 23>;

    static_assert(12 == sample_bound{ 12 });
    static_assert(sample_bound{ 12 } == 12);

    static_assert(sample_bound{ 23 } == sample_bound{ 23 });
    static_assert(!(sample_bound{ 20 } == sample_bound{ 21 }));

    EXPECT_THROW(sample_bound{ 1 }, std::out_of_range);
    EXPECT_NO_THROW(sample_bound{ 22 });
}


TEST(TemplatesTests, MatrixTemplatesTests)
{
    using product_t_1 = matrix_product_t<Matrix<int, 2, 6>, Matrix<int, 6, 3>>;

    static_assert(std::is_same_v<product_t_1, Matrix<int, 2, 3>>);

    static_assert(std::is_same_v<matrix_transpose_t<Matrix<double, 2, 3>>, Matrix<double, 3, 2>>);
}


TEST(TemplatesTests, MatrixEqualityTests)
{
    constexpr auto firstMatrix = Matrix<int, 2, 2>{ -1, -4, 6, 9 };

    constexpr auto sameAsFirstMatrix = firstMatrix;

    static_assert(firstMatrix == sameAsFirstMatrix);

    constexpr auto defaultMatrix = Matrix<int, 2, 2>{};

    static_assert(firstMatrix != defaultMatrix);

    auto mat = Matrix<int, 3, 3>
    {
        -2, -3, 1,
         4, -6, 6,
         0,  3, 7
    };

    EXPECT_TRUE(mat[0][0] == -2);
    EXPECT_TRUE(mat[1][2] == 6);
    EXPECT_TRUE(mat[1][1] == -6);
    EXPECT_TRUE(mat[0][2] == 1);
    EXPECT_TRUE(mat[2][1] == 3);
}


TEST(TemplatesTests, MatrixArithmeticTests)
{
    constexpr auto firstMatrix = Matrix<int, 2, 2>{ -1, -4, 6, 9 };

    constexpr auto sameAsFirstMatrix = firstMatrix;

    constexpr auto defaultMatrix = Matrix<int, 2, 2>{};

    constexpr auto shouldBeAllZerosMatrix = firstMatrix - sameAsFirstMatrix;

    static_assert(defaultMatrix == shouldBeAllZerosMatrix);

    constexpr auto shouldBeSameAsFirstMatrix = firstMatrix + decltype(firstMatrix){};

    static_assert(firstMatrix == shouldBeSameAsFirstMatrix);


    auto firstMat = Matrix<int, 3, 3>
    {
        -2, -3, 1,
         4, -6, 6,
         0,  3, 7
    };

    auto sameAsFirstMat = firstMat;

    firstMat += decltype(firstMat){};

    EXPECT_TRUE(firstMat == sameAsFirstMat);

    firstMat -= sameAsFirstMat;

    EXPECT_TRUE(firstMat == decltype(firstMat){});

}


TEST(TemplatesTests, MatrixMultiplicationTests)
{
    constexpr auto matrix1 = Matrix<int, 4, 3>
    {
         1,  2,  3,
        53,  6, 45,
         3, 43,  3,
         2, 32,  3,
    };

    constexpr auto matrix2 = Matrix<int, 3, 6>
    {
        23, 3, 4, 54, 236,  8,
        56, 4, 3, 78, 711,  8,
        6,  6, 6,  6,  41, 64,
    };

    constexpr auto product = matrix1 * matrix2;

    constexpr auto expected = matrix_product_t<decltype(matrix1), decltype(matrix2)>
    {
         153,  29,  28,  228,  1781,  216,
        1825, 453, 500, 3600, 18619, 3352,
        2495, 199, 159, 3534, 31404,  560,
        1856, 152, 122, 2622, 23347,  464,
    };

    static_assert(product == expected);


    auto mat1 = Matrix<int, 2, 2>
    {
        3, 3,
        5, 3,
    };

    auto mat2 = Matrix<int, 2, 2>
    {
        42, 46,
        54, 68,
    };

    mat1 *= mat2;

    auto exp = Matrix<int, 2, 2>
    {
        288, 342,
        372, 434,
    };

    EXPECT_TRUE(exp == mat1);
}


TEST(TemplatesTests, MatrixTransposeTests)
{
    constexpr auto matrix1 = Matrix<double, 3, 2>{ -1.0, 3.0, 12.9, -12.78, -0.9, 900.8 };

    constexpr auto transpose = ~matrix1;

    constexpr auto expected = matrix_transpose_t<decltype(matrix1)>{ -1.0, 12.9, -0.9, 3.0, -12.78, 900.8 };

    static_assert(transpose == expected);
}


TEST(TemplatesTests, MatrixLeftoverElementsTests)
{
    constexpr auto matrix1 = Matrix<double, 3, 3>{ -1.0, 3.0, 12.9, -12.78, -0.9, 900.8, 23.4, 0.0, 69.8 };

    constexpr auto leftover = matrix1.leftover_elements(0_ui64, 0_ui64);

    constexpr auto expected_leftover = Matrix<double, 2, 2>{ -0.9, 900.8, 0.0, 69.8 };

    static_assert(leftover == expected_leftover);

    static_assert(expected_leftover.determinant() == -62.82);

    static_assert(abs(matrix1.determinant() - 66246.786L) < 1e-5);
}


TEST(TemplatesTests, MatrixTests)
{
    static_assert(IsMatrix<Matrix<int, 2, 3>>);

    static_assert(!IsMatrix<int>);

    static_assert(IsSquareMatrix<Matrix<int, 10, 10>>);

    static_assert(!IsSquareMatrix<Matrix<double, 2, 3>>);
}


TEST(TemplatesTests, RandomValueTests)
{
    constexpr auto min = -2;
    constexpr auto max = 101;

    using bound_type = BoundValue<min, max>;

    auto val3{ GetRandomValue<bound_type>() };

    EXPECT_TRUE(in_range(val3.value(), bound_type::min(), bound_type::max()));
}
