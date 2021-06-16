/**
 * @file password_generator_tests.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-10-16
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "gtest/gtest.h"

#include "passwordlib/password_generator.hpp"


TEST(PasswordTests, AllValidSpecialCharacters)
{
    auto expected_length{ 12 };

    auto random_str{ password_generator::get_random_string(expected_length, password_generator::all_special_characters) };

    EXPECT_EQ(random_str.size(), expected_length);
}


TEST(PasswordTests, NoSpecialCharacters)
{
    auto expected_length{ 23 };

    auto random_str{ password_generator::get_random_string(expected_length, password_generator::no_special_characters) };

    EXPECT_EQ(random_str.size(), expected_length);

    for (auto&& char_ : "`~!@#$%^&*()-_=+[{}]\\|;:\'\",<.>/? ")
    {
        EXPECT_TRUE(random_str.find(char_) == std::string::npos);
    }
}