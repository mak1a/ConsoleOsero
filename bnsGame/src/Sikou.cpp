#include "Sikou.hpp"
#include <algorithm>


bnsGame::Sikou::Sikou()
: m_bestPoint(0, utl::Point()) {
    for (int32_t y{}; y < 10; ++y) {
        for (int32_t x{}; x < 10; ++x) {
            m_evaluationValues[y][x] = 0;
        }
    }

    for (int32_t y{1}; y <= 8; ++y) {
        for (int32_t x{1}; x <= 8; ++x) {
            m_evaluationValues[y][x] = utl::RandomInt32(-16, 16);
            if (m_evaluationValues[y][x] < 0) {
                std::cout << m_evaluationValues[y][x] << std::endl;
            }
        }
    }
}

bnsGame::Sikou::Sikou(const bool isSelected) : m_bestPoint(0, utl::Point()) {
    for (int32_t y{}; y < 10; ++y) {
        for (int32_t x{}; x < 10; ++x) {
            m_evaluationValues[y][x] = 0;
        }
    }

    m_evaluationValues[1][1] = 16;
    m_evaluationValues[1][8] = 16;
    m_evaluationValues[8][1] = 16;
    m_evaluationValues[8][8] = 16;
}

bnsGame::Sikou::Sikou(const std::array<std::array<int32_t, 10>, 10>& values_) : m_bestPoint(0, utl::Point()), m_evaluationValues(values_) {
    Mutation();
}

int32_t bnsGame::Sikou::MiniMax(const int32_t isSelfOrEnemy_, Aspect& aspect_, const int32_t depth_) {
    
    std::vector<utl::Point> valids{aspect_.GetStoneValids()};
    std::shuffle(valids.begin(), valids.end(), utl::Private::mt);
    
    if (depth_ <= 0) {
        if (aspect_.MakeLegalPuts(isSelfOrEnemy_) <= 0) {
            return ((isSelfOrEnemy_ == Self) ? -99999 : 99999);
        }

        std::pair<int32_t, utl::Point> candidate(((isSelfOrEnemy_ == Self) ? -99999 : 99999), utl::Point());

        for (const auto& pos : valids) {
            Aspect aspect{aspect_};
            aspect.Put(isSelfOrEnemy_, pos, m_evaluationValues);

            if (isSelfOrEnemy_ == Self) {
                if (candidate.first < aspect.value) {
                    candidate = std::make_pair(aspect.value, pos);
                }
                continue;
            }

            if (candidate.first > aspect.value) {
                candidate = std::make_pair(aspect.value, pos);
            }
        }
        return -candidate.first;
    }

    m_bestPoint = std::make_pair(((isSelfOrEnemy_ == Self) ? -99999 : 99999), utl::Point());

    for (const auto& pos : valids) {
        Aspect aspect{aspect_};
        aspect.Put(isSelfOrEnemy_, pos, m_evaluationValues);

        if (isSelfOrEnemy_ == Self) {
            int32_t value = MiniMax(((isSelfOrEnemy_ == Self) ? Enemy : Self), aspect, depth_ - 1);
            if (m_bestPoint.first < value) {
                m_bestPoint = std::make_pair(value, pos);
            }
            continue;
        }

        int32_t value = MiniMax(((isSelfOrEnemy_ == Self) ? Enemy : Self), aspect, depth_ - 1);
        if (m_bestPoint.first > value) {
            m_bestPoint = std::make_pair(value, pos);
        }
    }

    return m_bestPoint.first;
}

bnsGame::utl::Point bnsGame::Sikou::Think(const int32_t isSelfOrEnemy_, Aspect aspect_) noexcept {
    int32_t value = MiniMax(isSelfOrEnemy_, aspect_, 1);
    return m_bestPoint.second;
}
