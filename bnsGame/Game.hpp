#pragma once

#include "Common.hpp"

namespace bnsGame {
    class Game : public MyScene::Scene {
    private:
    public:
        Game(const InitData& init_);

        void Update() override;

        void Draw() const override;
    };
}  // namespace bnsGame