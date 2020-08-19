#pragma once
#include "SceneMaster.hpp"

namespace bnsGame {
    enum class Scene {
        Title,
        Game,
    };

    class GameData {
    public:
        constexpr GameData() = default;
    };

    using MyScene = bnsGame::utl::SceneMaster<Scene, GameData>;
}  // namespace bnsGame