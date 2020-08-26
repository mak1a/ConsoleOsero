#pragma once
#include "Utility/Point.hpp"
#include "Common.hpp"
#include <iostream>
#include <array>

namespace bnsGame {
    enum StoneType : int32_t {
        None = 0,
        Self = 1,
        Enemy = -1,
        Wall = 2
    };

    class Aspect {
    private:
        std::array<int32_t, 10 * 10> m_boards;

        std::vector<utl::Point> m_stoneValids;

        int32_t m_tesu;

        int32_t m_finalValue;

        int32_t m_value;
        void SetValue(const int32_t value_) {
            m_value = value_;
        }

        void SetCurrentValue(const std::array<std::array<int32_t, 10>, 10>& values_);

    public:
        Aspect() = default;
        Aspect(const int32_t firstTurn_) noexcept;

        /// <summary>
        /// 有効な場所を返す
        /// </summary>
        /// <returns>置くことが可能な場所</returns>
        [[nodiscard]] std::vector<utl::Point> GetStoneValids() const noexcept {
            return m_stoneValids;
        }

        int32_t GetValue() const {
            return m_value;
        }

        __declspec(property(get = GetValue, put = SetValue)) int32_t value;

        /// <summary>
        /// 石を置く
        /// </summary>
        /// <param name="isSelfOrEnemy_">手番</param>
        /// <param name="coordinate_">座標</param>
        void Put(const int32_t isSelfOrEnemy_, const utl::Point& coordinate_, const std::array<std::array<int32_t, 10>, 10>& values_);

        uint32_t MakeLegalPuts(const int32_t isSelfOrEnemy_);

        [[nodiscard]] Winner GetWinner() const noexcept {
            int32_t count{};
            for (uint32_t y{1}; y <= 8; ++y) {
                for (uint32_t x{1}; x <= 8; ++x) {
                    count += m_boards[y * 10 + x];
                }
            }

            if (count > 0) {
                std::cout << "プレイヤーWin!" << std::endl;
                Print();
                return Winner::Player;
            }

            if (count < 0) {
                std::cout << "プレイヤーLose..." << std::endl;
                Print();
                return Winner::Enemy;
            }

            std::cout << "引き分け" << std::endl;
            Print();
            return Winner::Draw;
        }

        void Print() const noexcept {
            uint32_t playerStoneCount{};
            uint32_t enemyStoneCount{};

            for (uint32_t y{1}; y <= 8; ++y) {
                for (uint32_t x{1}; x <= 8; ++x) {
                    if (m_boards[y * 10 + x] == Self) {
                        ++playerStoneCount;
                        continue;
                    }

                    if (m_boards[y * 10 + x] == Enemy) {
                        ++enemyStoneCount;
                        continue;
                    }
                }
            }

            std::cout << "プレイヤー : " << playerStoneCount << ", 敵 : " << enemyStoneCount << std::endl;
        }

        [[nodiscard]] int32_t GetTesu() const noexcept {
            return m_tesu;
        }

        [[nodiscard]] int32_t GetFinalValue() const noexcept {
            return m_finalValue;
        }
    };
}
