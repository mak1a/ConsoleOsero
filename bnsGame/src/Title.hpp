#pragma once

#include "Common.hpp"

namespace bnsGame {
    class Title : public MyScene::Scene {
    private:
        static constexpr uint32_t k_boardWidth = 8;
        static constexpr uint32_t k_boardHeight = 8;

        std::vector<int32_t*> m_displayBuffer;

        void ClearDisplay() const {
            system("cls");
        }
    public:
        Title(const InitData& init_);

        void Update() override;

        void UpdateChangeOut() override;

        void Draw() const override;
    };
}