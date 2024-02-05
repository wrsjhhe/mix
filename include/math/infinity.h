#pragma once

#include <limits>

namespace mix {

    template<typename T>
    constexpr T Infinity = std::numeric_limits<T>::infinity();

}
