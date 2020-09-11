/**
 * @file macro_utils.hpp
 * @author ashwinn76
 * @brief Macros used in the repo
 * @version 0.1
 * @date 2020-08-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include <stdint.h>

#define UNSIGNED_CONVERTER(x) consteval auto operator"" _ui##x(unsigned long long __n) noexcept{ return static_cast<uint##x##_t>(__n); }
#define SIGNED_CONVERTER(x) consteval auto operator"" _i##x(unsigned long long __n) noexcept{ return static_cast<int##x##_t>(__n); }

#define CONVERTER(x) UNSIGNED_CONVERTER(x) SIGNED_CONVERTER(x)

CONVERTER(8);
CONVERTER(16);
CONVERTER(32);
CONVERTER(64);
