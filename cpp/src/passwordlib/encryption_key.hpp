/**
 * @file encryption_key.h
 * @author Aashwinn76
 * @brief Declarations for CEncryptionKey class and helper structs and functions
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <fstream>
#include <string>

#include "algo_utils.hpp"

namespace encryption
{

namespace
{


constexpr auto EncryptionHelperKey = std::string_view{ "I8Cdcw3No4PMlcoHNT5rc8KJXs2Fydu9SX7iNx7OCE66jRYvpPqFmDY80Hd8xDbe" };

constexpr auto IdealKeySize = 32_ui64;

auto get_additional_character()
{
    auto random_pos{ get_random_value(0_sz, EncryptionHelperKey.size()) };

    return EncryptionHelperKey.at(random_pos);
}

}

/**
 * @struct encryption_edit_s struct
 *
 * @brief Struct storing information about the enhancement done to the encryption key.
 */
struct encryption_edit_s
{
    uint64_t Position{ std::string::npos };
    char Character{};

    /**
     * @brief Write out encryption_edit_s object to a stream.
     *
     * @param io_stream stream object to be written to
     * @param i_edit Edit information
     * @return std::ostream&
     */
    friend std::ostream& operator<<(std::ostream& io_stream, const encryption_edit_s& i_edit)
    {
        return io_stream.write(IO_INFORMATION(i_edit));
    }

    /**
     * @brief Read in encryption_edit_s object from a stream.
     *
     * @param io_stream stream object to be read from
     * @param io_edit Edit information will be read into this object
     * @return std::istream&
     */
    friend std::istream& operator>>(std::istream& io_stream, encryption_edit_s& io_edit)
    {
        return io_stream.read(IO_INFORMATION(io_edit));
    }

private:

    /**
     * @brief Equality comparison for encryption_edit_s objects.
     *
     * @param i_lhs left hand operand
     * @param i_rhs right hand operand
     * @return true if objects are equal
     * @return false is objects are not equal
     */
    friend constexpr bool operator==(const encryption_edit_s& i_lhs, const encryption_edit_s& i_rhs) noexcept
    {
        return i_lhs.Position == i_rhs.Position && i_lhs.Character == i_rhs.Character;
    }

    /**
     * @brief Inequality comparison for encryption_edit_s objects.
     *
     * @param i_lhs left hand operand
     * @param i_rhs right hand operand
     * @return true if objects are unequal
     * @return false is objects are equal
     */
    friend constexpr bool operator!=(const encryption_edit_s& i_lhs, const encryption_edit_s& i_rhs) noexcept
    {
        return !(i_lhs == i_rhs);
    }
};


constexpr auto no_key_edit = encryption_edit_s{};



/**
 * @class encryption_key class
 *
 * @brief class to hold the encryption key specified by the user, and apply enhancements if required.
 *
 */
class encryption_key
{
public:
    /**
     * @brief Construct a new CEncryptionKey object
     *
     * @param i_key Encryption Key
     *
     * @param i_enhance Flag to enhance the key
     */
    explicit encryption_key(std::string i_key, bool i_enhance = true)
        : m_originalKey{ std::move(i_key) }
    {
        if (m_originalKey.size() < IdealKeySize)
        {
            throw std::length_error{ "Minimum key size of 16 required!" };
        }

        if (i_enhance)
        {
            m_editType = { get_random_value(0_ui64, IdealKeySize), get_additional_character() };
        }
    }

    /**
     * @brief Get the encryption key with enhancements
     *
     * @return std::string Corrected encryption key
     */
    auto string() const noexcept
    {
        auto enhancedStr{ std::string{m_originalKey} };

        if (m_editType.Position != std::string::npos)
        {
            enhancedStr.at(m_editType.Position) = m_editType.Character;
        }

        return enhancedStr;
    }

private:
    encryption_edit_s m_editType{ no_key_edit };

    std::string m_originalKey{};
};

}
