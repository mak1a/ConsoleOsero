#pragma once
#include "Utility/SceneMaster.hpp"
#include "Utility/Allocator/MyAllocator.hpp"

namespace bnsGame {
    enum class Scene {
        Title,
        Game,
    };

    class GameData {
    public:
        std::unique_ptr<allocator::iMemoryAllocator> m_alloc;

        GameData(void* ptr_, const size_t size_) : m_alloc(std::make_unique<allocator::MyAllocator>(ptr_, size_)) {}
    };

    using MyScene = bnsGame::utl::SceneMaster<Scene, GameData>;
}  // namespace bnsGame