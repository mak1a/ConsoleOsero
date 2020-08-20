#pragma once

#include "Common.hpp"
#include "MyAllocator.hpp"
#include <array>

namespace bnsGame {
    class Game : public MyScene::Scene {
    private:
        std::array<char*, 22> m_displayBuffer;
    public:
        Game(const InitData& init_);

        void Update() override;

        void UpdateChangeOut() override;

        void Draw() const override;
    };
}  // namespace bnsGame