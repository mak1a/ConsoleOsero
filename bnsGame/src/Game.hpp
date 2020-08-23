#pragma once

#include "Common.hpp"
#include "Sikou.hpp"
#include <vector>
#include <array>
#include <string>
#include <iostream>

namespace bnsGame {
    /// <summary>
    /// É}ÉXÇÃèÛë‘
    /// </summary>
    enum class StoneState {
	    None,
        White,
        Black,
        Wall
	};

    /// <summary>
    /// êŒ
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

            void Draw() const override {
                std::cout << "ÅE";
            }
        };

        class Black : public utl::State<StoneState> {
        private:
            Stone& m_stone;
        public:
            Black(Stone& stone_)
                : State<StoneState>(StoneState::Black)
                , m_stone(stone_) {}

            void Draw() const override {
                std::cout << "Åõ";
            }
        };

        class White : public utl::State<StoneState> {
        private:
            Stone& m_stone;
        public:
            White(Stone& stone_)
                : State<StoneState>(StoneState::White)
                , m_stone(stone_) {}

            void Draw() const override {
                std::cout << "Åú";
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
            InitializeStateMachine();
            GoToState(state_);
        }
        Stone() : Stone(StoneState::Wall) {}

        void InitializeStateMachine() override {
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

        Aspect m_aspect;
        Sikou m_sikou;

        enum class Turn {
            Player,
            Enemy,
            Result
        } m_turn;

        void ClearDisplay() const {
            system("cls");
        }

        void Input();

        void PlayerTurn();

        void EnemyTurn();

        void ChangeTurn() {
            m_turn = ((m_turn == Turn::Player) ? Turn::Enemy : Turn::Player);
            if (m_turn == Turn::Player) {
                if (m_aspect.MakeLegalPuts(Self) <= 0) {
                    m_turn = Turn::Result;
                }
            }
            else {
                if (m_aspect.MakeLegalPuts(Enemy) <= 0) {
                    m_turn = Turn::Result;
                }
            }
        }

        bool ChangeStones(const StoneState& stone_);
    public:
        Game(const InitData& init_);

        void Update() override;

        //void UpdateChangeOut() override;

        void Draw() const override;
    };
}  // namespace bnsGame