#pragma once

template <typename Enum>
constexpr int toIndex(Enum value)
{
    return static_cast<int>(value);
}