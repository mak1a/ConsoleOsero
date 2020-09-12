#pragma once

#include "Common.hpp"
#include "Sikou.hpp"
#include "Utility/StateMachine.hpp"
#include <vector>
#include <array>
#include <string>
#include <iostream>

namespace bnsGame {
    /// <summary>
    /// マスの状態
    /// </summary>
    enum class StoneState {
	    None,
        White,
        Black,
        Wall
	};

    /// <summary>
    /// 石
    /// </summary>
    class Stone : public utl::StateMachine<StoneState> {
    private:
        class None : public utl::State<StoneState> {
        private:
            Stone& m_stone;
        public:
            None(Stone& stone_)
                : State<StoneState>(StoneState::None)
                , m_stone(stone_) {}

            void Draw(const bool selectPos_) const override {
                std::cout << "\x1b[42m・" << "\x1b[49m";
            }
        };

        class Black : public utl::State<StoneState> {
        private:
            Stone& m_stone;
        public:
            Black(Stone& stone_)
                : State<StoneState>(StoneState::Black)
                , m_stone(stone_) {}

            void Draw(const bool selectPos_) const override {
                if (selectPos_) {
                    std::cout << "\x1b[43m" << "\x1b[30m●" << "\x1b[39m" << "\x1b[49m";
                    return;
                }
                std::cout << "\x1b[42m" << "\x1b[30m●" << "\x1b[39m" << "\x1b[49m";
            }
        };

        class White : public utl::State<StoneState> {
        private:
            Stone& m_stone;
        public:
            White(Stone& stone_)
                : State<StoneState>(StoneState::White)
                , m_stone(stone_) {}

            void Draw(const bool selectPos_) const override {
                if (selectPos_) {
                    std::cout << "\x1b[43m" << "\x1b[37m●" << "\x1b[39m" << "\x1b[49m";
                    return;
                }
                std::cout << "\x1b[42m" << "\x1b[37m●" << "\x1b[39m" << "\x1b[49m";
            }
        };

        class Wall : public utl::State<StoneState> {
        private:
            Stone& m_stone;
        public:
            Wall(Stone& stone_)
                : State<StoneState>(StoneState::Wall)
                , m_stone(stone_) {}

        };

    public:
        Stone(const StoneState& state_) {
            Initialize();
            ChangeState(state_);
        }
        Stone() : Stone(StoneState::Wall) {}

        void Initialize() override {
            AddState(std::make_shared<None>(*this));
            AddState(std::make_shared<Black>(*this));
            AddState(std::make_shared<White>(*this));
            AddState(std::make_shared<Wall>(*this));
        }
    };

    class Game : public MyScene::Scene {
    private:
        static constexpr uint32_t k_boardWidth = 10;
        static constexpr uint32_t k_boardHeight = 10;

        std::array<std::array<Stone, k_boardWidth>, k_boardHeight> m_displayBuffer;
        std::array<std::array<Stone, k_boardWidth>, k_boardHeight> m_boards;

        utl::Point m_stonePutPos;

        std::string m_inputString;

        Turn m_turn;
        
        Sikou m_sikou;
        
        // 学習がちゃんとできてるかの確認用
        Sikou m_testSikou;

        Aspect m_aspect;
        
        StoneState m_playerColor;

        void ClearDisplay() const {
            system("cls");
        }

        void Input();

        void PlayerTurn();
        void PlayerAITurn();

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

        bool ChangeStones(const StoneState& stone_);
    public:
        Game(const InitData& init_);

        void Update() override;

        void UpdateChangeOut() override;

        void Draw() const override;
    };
}  // namespace bnsGame