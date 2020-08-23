#include "Sikou.hpp"
#include "Common.hpp"
#include <random>

bnsGame::utl::Point bnsGame::Sikou::Think(const int32_t isSelfOrEnemy, Aspect aspect_) noexcept {
    std::vector<utl::Point> valids{aspect_.GetStoneValids()};
    std::shuffle(valids.begin(), valids.end(), utl::Private::mt);

    return valids.front();
}
