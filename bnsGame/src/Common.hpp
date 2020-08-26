#pragma once
#include "Utility/SceneMaster.hpp"
#include "Utility/Allocator/MyAllocator.hpp"
#include <random>
#include <array>

namespace bnsGame {
    namespace utl {
        namespace Private {
            inline std::random_device rd;
            inline std::mt19937 mt(rd());
        }  // namespace Private

        [[nodiscard]] inline bool RandomBool(double p_ = 0.5) noexcept {
            std::bernoulli_distribution getBool(p_);
            return getBool(Private::mt);
        }

        [[nodiscard]] inline uint32_t RandomUint32(const uint32_t min_, const uint32_t max_) noexcept {
            std::uniform_int_distribution<uint32_t> getNum(min_, max_);
            return getNum(Private::mt);
        }

        [[nodiscard]] inline uint32_t RandomInt32(const int32_t min_, const int32_t max_) noexcept {
            std::uniform_int_distribution<int32_t> getNum(min_, max_);
            return getNum(Private::mt);
        }
    }

    enum class Scene {
        Title,
        Game,
        Study
    };

    enum class Turn { 
        Player, 
        Enemy, 
        Result 
    };

    enum class Winner {
        Player,
        Enemy,
        Draw
    };

    class GameData {
    private:
        std::array<std::array<int32_t, 10>, 10> m_evaluationValues;
    public:
        std::unique_ptr<allocator::iMemoryAllocator> m_alloc;

        GameData(void* ptr_, const size_t size_) : m_alloc(std::make_unique<allocator::MyAllocator>(ptr_, size_)) {
            for (int32_t y{}; y < 10; ++y) {
                for (int32_t x{}; x < 10; ++x) {
                    m_evaluationValues[y][x] = 0;
                }
            }

            m_evaluationValues[1][1] = 128;
            m_evaluationValues[1][8] = 128;
            m_evaluationValues[8][1] = 128;
            m_evaluationValues[8][8] = 128;
        }

        void SetValues(const std::array<std::array<int32_t, 10>, 10>& values_) {
            m_evaluationValues = values_;
        }

        [[nodiscard]] std::array<std::array<int32_t, 10>, 10> GetValues() const noexcept {
            return m_evaluationValues;
        }
    };

    using MyScene = bnsGame::utl::SceneMaster<Scene, GameData>;
}  // namespace bnsGame