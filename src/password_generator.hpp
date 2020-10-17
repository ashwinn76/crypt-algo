/**
 * @file password_generator.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-10-16
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <string>
#include <string_view>
#include <array>
#include <algorithm>

#include "bound_value.hpp"
#include "algo_utils.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace password_generator
{

enum class die_number
{
    one = 1,
    two,
    three,
    four,
    five,
    six
};


using die_result_t = bound_value<die_number::one, die_number::six>;


struct char_valid_info_s
{
    bool valid{ false };
    std::string_view characters{};
};


constexpr auto all_special_characters = char_valid_info_s{};
constexpr auto no_special_characters = char_valid_info_s{ true };


auto get_random_string(int i_length, char_valid_info_s i_character_info) noexcept
{
    constexpr auto first_grid = std::array<char, 36>
    {
        'A', 'B', 'C', 'D', 'E', 'F',
        'G', 'H', 'I', 'J', 'K', 'L',
        'M', 'N', 'O', 'P', 'Q', 'R',
        'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9',
    };

    constexpr auto second_grid = std::array<char, 36>
    {
        'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l',
        'm', 'n', 'o', 'p', 'q', 'r',
        's', 't', 'u', 'v', 'w', 'x',
        'y', 'z',
    };

    constexpr auto third_grid = std::array<char, 36>
    {
        '!', '@', '#', '$', '%', '^',
        '&', '*', '(', ')', '-', '=',
        '+', '[', ']', '{', '}', '\\',
        '|', '`', ';', ':', '\'', '\"',
        '<', '>', '/', '?', '.', ',',
        '~', '_', ' ',
    };

    auto shuffle_and_filter = [chars = std::move(i_character_info.characters), valid = i_character_info.valid](auto&& i_original_grid, bool i_filter = false)
    {
        auto randomized_grid{ i_original_grid };

        std::shuffle(std::begin(randomized_grid), std::end(randomized_grid), std::random_device{});

        if (i_filter)
        {
            std::for_each(std::begin(randomized_grid), std::end(randomized_grid), [&chars, &valid](auto&& _char)
            {
                auto found{ chars.find(_char) != std::string_view::npos };

                auto should_clear{ found != valid };

                if (should_clear)
                {
                    _char = '\0';
                }
            });
        }

        return randomized_grid;
    };

    auto grids = std::array{ shuffle_and_filter(first_grid), shuffle_and_filter(second_grid), shuffle_and_filter(third_grid, true) };

    auto get_char = [&grids](die_result_t&& _1, die_result_t&& _2, die_result_t&& _3)
    {
        auto grid_number{ static_cast<int>(_1) % 2 == 0 ? static_cast<int>(_1) / 2 : (static_cast<int>(_1) + 1) / 2 };

        auto& grid{ grids[grid_number - 1] };

        auto row_num{ static_cast<int>(_3) - 1 };
        auto col_num{ static_cast<int>(_2) - 1 };

        return grid[row_num * 6 + col_num];
    };

    auto random_str{ ""s };
    random_str.reserve(i_length);

    for (auto i{ 0 }; i < i_length; ++i)
    {
        auto char_{ '\0' };

        do
        {

            char_ = get_char(get_random_value<die_result_t>(), get_random_value<die_result_t>(), get_random_value<die_result_t>());

        } while (char_ == '\0');

        random_str.push_back(char_);
    }

    return random_str;
}

}