#pragma once
#include "Utility/StateMachine.hpp"
#include "Utility/Point.hpp"
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

        int32_t m_value;
        void SetValue(const int32_t value_) {
            m_value = value_;
        }

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
        void Put(const int32_t isSelfOrEnemy_, const utl::Point& coordinate_);

        uint32_t MakeLegalPuts(const int32_t isSelfOrEnemy_);

    };
}
