#ifndef REFLECTION_CPP_COMPILE_STRING_H
#define REFLECTION_CPP_COMPILE_STRING_H

#include <algorithm>

template <size_t Size>
struct compile_string
{
    static constexpr size_t s_size{Size};

    constexpr compile_string(const char(&_str)[Size]) noexcept
    {
        std::ranges::copy_n(_str, Size, m_buffer);
    }

    constexpr operator const char*() const noexcept
    {
        return m_buffer;
    }

    template <size_t OtherSize>
    constexpr bool operator==(const compile_string<OtherSize>& _other) const noexcept
    {
        if constexpr (Size != OtherSize)
        {
            return false;
        }

        return std::ranges::equal(m_buffer, _other.m_buffer);
    }

    template <size_t OtherSize>
    constexpr bool operator!=(const compile_string<OtherSize>& _other) const noexcept
    {
        return !(*this == _other);
    }

    char m_buffer[Size]{};
};



#endif //REFLECTION_CPP_COMPILE_STRING_H
