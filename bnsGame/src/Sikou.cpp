#include "Sikou.hpp"
#include <algorithm>


bnsGame::Sikou::Sikou()
    : m_bestPoint(12, utl::Point())
    , m_depthMax(1)
    , m_isExperiment(false) {
    for (int32_t y{}; y < 10; ++y) {
        for (int32_t x{}; x < 10; ++x) {
            m_evaluationValues[y][x] = 0;
        }
    }
}

bnsGame::Sikou::Sikou(const bool isSelected)
    : m_bestPoint(12, utl::Point())
    , m_depthMax(1)
    , m_isExperiment(false) {
    for (int32_t y{}; y < 10; ++y) {
        for (int32_t x{}; x < 10; ++x) {
            m_evaluationValues[y][x] = 0;
        }
    }

    m_evaluationValues[1][1] = 128;
    m_evaluationValues[1][8] = 128;
    m_evaluationValues[8][1] = 128;
    m_evaluationValues[8][8] = 128;
}

bnsGame::Sikou::Sikou(const std::array<std::array<int32_t, 10>, 10>& values_, const bool isExperiment_)
: m_bestPoint(12, utl::Point())
, m_depthMax((isExperiment_) ? 1 : 5)
, m_evaluationValues(values_)
, m_isExperiment(isExperiment_) {
    Mutation();
}

int32_t bnsGame::Sikou::MiniMax(const int32_t isSelfOrEnemy_, Aspect& aspect_, const int32_t depth_) {
    if (aspect_.MakeLegalPuts(isSelfOrEnemy_) <= 0) {
        if (aspect_.MakeLegalPuts((isSelfOrEnemy_ == Self) ? Enemy : Self) <= 0) {
            return aspect_.value;
        }

        return MiniMax((isSelfOrEnemy_ == Self) ? Enemy : Self, aspect_, depth_ + 1);
    }

    if (depth_ >= m_depthMax) {

        std::vector<utl::Point> valids{aspect_.GetStoneValids()};
        std::shuffle(valids.begin(), valids.end(), utl::Private::mt);

        int32_t retVal{(isSelfOrEnemy_ == Self) ? -1000000 : 1000000};

        for (uint32_t i{}; i < valids.size(); ++i) {
            Aspect a{aspect_};
            a.Put(isSelfOrEnemy_, valids[i], m_evaluationValues);

            if ((isSelfOrEnemy_ == Self && a.value > retVal) || (isSelfOrEnemy_ == Enemy && a.value < retVal)) {
                retVal = a.value;
            }
        }

        return retVal;
    }

    std::vector<utl::Point> valids{aspect_.GetStoneValids()};
    std::shuffle(valids.begin(), valids.end(), utl::Private::mt);

    int32_t retVal{(isSelfOrEnemy_ == Self) ? -1000000 : 1000000};

    for (const auto& stonePos : valids) {
        Aspect a{aspect_};
        a.Put(isSelfOrEnemy_, stonePos, m_evaluationValues);

        int32_t value{MiniMax((isSelfOrEnemy_ == Self) ? Enemy : Self, a, depth_ + 1)};

        if ((isSelfOrEnemy_ == Self && value > retVal) || (isSelfOrEnemy_ == Enemy && value < retVal)) {
            retVal = value;
            m_bestPoint[depth_] = stonePos;
        }
    }
    return retVal;
}

int32_t bnsGame::Sikou::ReadThrough(const int32_t isSelfOrEnemy_, Aspect& aspect_, const int32_t depth_) {
    if (aspect_.MakeLegalPuts(isSelfOrEnemy_) <= 0) {
        if (aspect_.MakeLegalPuts((isSelfOrEnemy_ == Self) ? Enemy : Self) <= 0) {
            return aspect_.GetFinalValue();
        }

        return ReadThrough((isSelfOrEnemy_ == Self) ? Enemy : Self, aspect_, depth_);
    }

    if (aspect_.GetFinalValue() >= 63) {
        std::vector<utl::Point> valids{aspect_.GetStoneValids()};
        std::shuffle(valids.begin(), valids.end(), utl::Private::mt);

        int32_t retVal{(isSelfOrEnemy_ == Self) ? -1000000 : 1000000};

        for (uint32_t i{}; i < valids.size(); ++i) {
            Aspect a{aspect_};
            a.Put(isSelfOrEnemy_, valids[i], m_evaluationValues);

            if ((isSelfOrEnemy_ == Self && a.GetFinalValue() > retVal) || (isSelfOrEnemy_ == Enemy && a.GetFinalValue() < retVal)) {
                retVal = a.GetFinalValue();
            }
        }

        return retVal;
    }

    std::vector<utl::Point> valids{aspect_.GetStoneValids()};
    std::shuffle(valids.begin(), valids.end(), utl::Private::mt);

    int32_t retVal{(isSelfOrEnemy_ == Self) ? -1000000 : 1000000};

    for (const auto& stonePos : valids) {
        Aspect a{aspect_};
        a.Put(isSelfOrEnemy_, stonePos, m_evaluationValues);

        int32_t value{ReadThrough((isSelfOrEnemy_ == Self) ? Enemy : Self, a, depth_ + 1)};

        if ((isSelfOrEnemy_ == Self && value > retVal) || (isSelfOrEnemy_ == Enemy && value < retVal)) {
            retVal = value;
            m_bestPoint[depth_] = stonePos;
        }
    }
    return retVal;
}

bnsGame::utl::Point bnsGame::Sikou::Think(const int32_t isSelfOrEnemy_, Aspect aspect_) noexcept {
    if (aspect_.GetTesu() >= 52) {
        int32_t value = ReadThrough(isSelfOrEnemy_, aspect_, 0);
        return m_bestPoint[0];
    }
    int32_t value = MiniMax(isSelfOrEnemy_, aspect_, 0);
    return m_bestPoint[0];
}
