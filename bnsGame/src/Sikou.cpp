#include "Sikou.hpp"
#include <algorithm>

bnsGame::Sikou::Sikou() {
    for (int32_t y{1}; y <= 8; ++y) {
        for (int32_t x{1}; x <= 8; ++x) {
            m_evaluationValues[y][x] = utl::RandomUint32(0, 8);
        }
    }
}

bnsGame::Sikou::Sikou(const bool isSelected) {
    m_evaluationValues[1][1] = 8;
    m_evaluationValues[1][8] = 8;
    m_evaluationValues[8][1] = 8;
    m_evaluationValues[8][8] = 8;
}

bnsGame::utl::Point bnsGame::Sikou::Think(const int32_t isSelfOrEnemy, Aspect aspect_) noexcept {
    std::vector<utl::Point> valids{aspect_.GetStoneValids()};
    std::shuffle(valids.begin(), valids.end(), utl::Private::mt);

    std::vector<std::pair<int32_t, utl::Point>> candidates;
    for (const auto& stonePos : valids) {
        candidates.emplace_back(m_evaluationValues[stonePos.y][stonePos.x], stonePos);
    }

    auto bestStone = *std::max_element(candidates.begin(), candidates.end(),
        [](const auto& a, const auto& b) { 
            return a.first < b.first;
        });

    //return valids.front();
    return bestStone.second;
}
