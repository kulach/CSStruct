#pragma once

#include <stdint.h>
#include <utility>
#include <string_view>

namespace cssv {

    template <typename T>
    constexpr std::pair<std::string_view, T> property(std::string_view name, T p) {
        return std::make_pair(name, p);
    }

}
