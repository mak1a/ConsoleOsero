#pragma once
#include "Aspect.hpp"

namespace bnsGame {
    class Sikou {
    public:
        Sikou() = default;

        utl::Point Think(const int32_t isSelfOrEnemy, Aspect aspect_) noexcept;
    };
}  // namespace bnsGame
