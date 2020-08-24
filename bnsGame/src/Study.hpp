#pragma once
#include "Common.hpp"
#include "Sikou.hpp"

namespace bnsGame {
    class Study : public MyScene::Scene {
    private:
        Turn m_turn;

        Aspect m_aspect;
        
        std::vector<Sikou> m_sikou;
        std::vector<uint32_t> m_winNums;

        uint32_t m_playerIndex;
        uint32_t m_enemyIndex;

        enum class StudyState { Studying, Mating, Exit } m_state;
        ;

        void ClearDisplay() const {
            system("cls");
        }

        void Init(const Turn& turn_);

        void PlayerTurn();

        void EnemyTurn();

        void Result();

        void ChangeTurn() {
            m_turn = ((m_turn == Turn::Player) ? Turn::Enemy : Turn::Player);
            if (m_turn == Turn::Player) {
                if (m_aspect.MakeLegalPuts(Self) <= 0) {
                    if (m_aspect.MakeLegalPuts(Enemy) <= 0) {
                        m_turn = Turn::Result;
                    }
                }
            }
            else {
                if (m_aspect.MakeLegalPuts(Enemy) <= 0) {
                    if (m_aspect.MakeLegalPuts(Self) <= 0) {
                        m_turn = Turn::Result;
                    }
                }
            }
        }

    public:
        Study(const InitData& init_);

        void Update() override;

        void UpdateChangeOut() override;

        void Draw() const override;
    };
}  // namespace bnsGame
