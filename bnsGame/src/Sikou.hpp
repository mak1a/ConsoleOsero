#pragma once
#include "Aspect.hpp"
#include "Common.hpp"

namespace bnsGame {
    class Sikou {
        std::array<std::array<int32_t, 10>, 10> m_evaluationValues;
    public:
        Sikou();
        Sikou(const bool isSelected);

        void Mutation() {
            for (uint32_t y{1}; y <= 8; ++y) {
                for (uint32_t x{1}; x <= 8; ++x) {
                    if (utl::RandomBool(0.1)) {
                        m_evaluationValues[y][x] = utl::RandomUint32(0, 8);
                    }
                }
            }
        }

        utl::Point Think(const int32_t isSelfOrEnemy, Aspect aspect_) noexcept;

        void Print() const noexcept {
            std::cout << "各座標の評価値は" << std::endl;
            for (uint32_t y{1}; y <= 8; ++y) {
                for (uint32_t x{1}; x <= 8; ++x) {
                    std::cout << m_evaluationValues[y][x] << " ";
                }
                std::cout << std::endl;
            }
        }
    };
}  // namespace bnsGame
