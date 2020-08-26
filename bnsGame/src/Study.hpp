#pragma once
#include "Common.hpp"
#include "Sikou.hpp"

namespace bnsGame {
    class Study : public MyScene::Scene {
    private:
        bool m_isChangeScene;

        int32_t m_parentNum;
        int32_t m_geneNum;

        Turn m_turn;

        std::vector<Sikou> m_sikous;
        std::vector<std::pair<uint32_t, uint32_t>> m_winNums;

        Aspect m_aspect;

        uint32_t m_playerIndex;
        uint32_t m_enemyIndex;

        enum class StudyState { Studying, Mating, Exit } m_state;
        ;

        void ClearDisplay() const {
            system("cls");
        }

        void Init(const Turn& turn_);

        void Mating();

        void PlayerTurn();

        void EnemyTurn();

        void Result();

        void ChangeTurn() {
            m_turn = ((m_turn == Turn::Player) ? Turn::Enemy : Turn::Player);
            if (m_turn == Turn::Player) {
                if (m_aspect.MakeLegalPuts(Self) <= 0) {
                    if (m_aspect.MakeLegalPuts(Enemy) <= 0) {
                        m_turn = Turn::Result;
                        return;
                    }

                    m_turn = Turn::Enemy;
                }
            }
            else {
                if (m_aspect.MakeLegalPuts(Enemy) <= 0) {
                    if (m_aspect.MakeLegalPuts(Self) <= 0) {
                        m_turn = Turn::Result;
                        return;
                    }

                    m_turn = Turn::Player;
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
