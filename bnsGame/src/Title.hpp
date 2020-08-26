#pragma once

#include "Common.hpp"

namespace bnsGame {
    class Title : public MyScene::Scene {
    private:
        static constexpr uint32_t k_boardWidth = 19;
        static constexpr uint32_t k_boardHeight = 9;

        std::vector<int32_t*> m_displayBuffer;

        void ClearDisplay() const {
            system("cls");
        }

        enum class Color {
            Green,  // óŒ
            White,  // îí
            Black   // çï
        };

        void DrawColorRect(const Color color_, std::string_view str_) const;
    public:
        Title(const InitData& init_);

        void Update() override;

        void UpdateChangeOut() override;

        void Draw() const override;
    };
}