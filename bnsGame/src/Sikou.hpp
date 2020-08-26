#pragma once
#include "Aspect.hpp"
#include "Common.hpp"

namespace bnsGame {
    class Sikou {
        std::array<std::array<int32_t, 10>, 10> m_evaluationValues;
        
        std::vector<utl::Point> m_bestPoint;

        int32_t m_depthMax;

        bool m_isExperiment;

        [[nodiscard]] int32_t MiniMax(const int32_t isSelfOrEnemy_, Aspect& aspect_, const int32_t depth_);
    
        [[nodiscard]] int32_t ReadThrough(const int32_t isSelfOrEnemy_, Aspect& aspect_, const int32_t depth_);
    public:
        explicit Sikou();
        explicit Sikou(const bool isSelected);
        explicit Sikou(const std::array<std::array<int32_t, 10>, 10>& values_, const bool isExperiment_ = true);
        Sikou(const std::array<std::array<int32_t, 10>, 10>& values_, const int32_t depth_);

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
