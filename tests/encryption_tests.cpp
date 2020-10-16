/**
 * @file EncryptionKeyTests.cpp
 * @author ashwinn76
 * @brief Testing the basic functionality of CEncryptionKey class among others
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "gtest/gtest.h"

#include "../src/bound_value.hpp"
#include "../src/encryption_key.hpp"


static const auto key = std::string{ "this_is_a_random_encryptionkey__" };

TEST(EncryptionKeyTests, EnhancedKeyCreationTests)
{
    auto encryptionKey{ encryption::encryption_key{ key } };

    EXPECT_NE(key, encryptionKey.string());
}


TEST(EncryptionKeyTests, BasicKeyCreationTests)
{
    auto encryptionKey{ encryption::encryption_key{ key, false } };

    EXPECT_EQ(key, encryptionKey.string());
}


TEST(EncryptionKeyTests, EnhancementIOTests)
{
    auto ss{ std::stringstream{} };

    auto test_enhancement{ encryption::encryption_edit_s{ 20_ui64, 's' } };
    ss << test_enhancement;

    auto expected_enhancement{ encryption::encryption_edit_s{} };
    ss >> expected_enhancement;

    EXPECT_EQ(test_enhancement, expected_enhancement);
}
