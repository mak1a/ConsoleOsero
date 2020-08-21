#pragma once

#include "Common.hpp"

namespace bnsGame {
    class Title : public MyScene::Scene {
    private:
        void ClearDisplay() const {
            system("cls");
        }

    public:
        Title(const InitData& init_);

        void Update() override;

        void Draw() const override;
    };
}