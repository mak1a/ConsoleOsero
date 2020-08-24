#pragma once
#include "Utility/SceneMaster.hpp"
#include "Utility/Allocator/MyAllocator.hpp"
#include <random>

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
    public:
        std::unique_ptr<allocator::iMemoryAllocator> m_alloc;

        GameData(void* ptr_, const size_t size_) : m_alloc(std::make_unique<allocator::MyAllocator>(ptr_, size_)) {}
    };

    using MyScene = bnsGame::utl::SceneMaster<Scene, GameData>;
}  // namespace bnsGame