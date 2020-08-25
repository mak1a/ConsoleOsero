#pragma once
#include "Aspect.hpp"
#include "Common.hpp"

namespace bnsGame {
    class Sikou {
        std::array<std::array<int32_t, 10>, 10> m_evaluationValues;
        
        std::pair<int32_t, utl::Point> m_bestPoint;

        [[nodiscard]] int32_t MiniMax(const int32_t isSelfOrEnemy_, Aspect& aspect_, const int32_t depth_);
    public:
        Sikou();
        Sikou(const bool isSelected);
        Sikou(const std::array<std::array<int32_t, 10>, 10>& values_);

        void Mutation() {
            for (uint32_t y{1}; y <= 8; ++y) {
                for (uint32_t x{1}; x <= 8; ++x) {
                    if (utl::RandomBool(0.05)) {
                        m_evaluationValues[y][x] = utl::RandomInt32(-16, 16);
                    }
                }
            }
        }

        [[nodiscard]] int32_t GetValue(const uint32_t x_, const uint32_t y_) const noexcept {
            return m_evaluationValues[y_][x_];
        }

        [[nodiscard]] std::array<std::array<int32_t, 10>, 10> GetValues() const noexcept {
            return m_evaluationValues;
        }

        utl::Point Think(const int32_t isSelfOrEnemy_, Aspect aspect_) noexcept;

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
