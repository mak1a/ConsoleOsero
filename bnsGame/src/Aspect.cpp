#include "Aspect.hpp"

bnsGame::Aspect::Aspect(const int32_t firstTurn_) noexcept : m_value(0) {
    std::fill(m_boards.begin(), m_boards.end(), Wall);
    for (uint32_t y{10}; y <= 80; y += 10) {
        for (uint32_t x{1}; x <= 8; ++x) {
            m_boards[y + x] = None;
        }
    }
    auto secondTurn = ((firstTurn_ == Self) ? Enemy : Self);

    m_boards[44] = secondTurn;
    m_boards[55] = secondTurn;
    m_boards[54] = firstTurn_;
    m_boards[45] = firstTurn_;
}

void bnsGame::Aspect::Put(const int32_t isSelfOrEnemy_, const utl::Point& coordinate_) {
    const int32_t prevStoneColor = ((isSelfOrEnemy_ == Self) ? Enemy : Self);
    // 上方向
    if (m_boards[(coordinate_.y - 1) * 10 + coordinate_.x] == prevStoneColor) {
        int32_t y{coordinate_.y - 2};
        bool isAllChanged{false};
        for (; !isAllChanged; --y) {
            if (m_boards[y * 10 + coordinate_.x] == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[y * 10 + coordinate_.x] != isSelfOrEnemy_) {
                continue;
            }

            // 実際に石を置く
            m_boards[coordinate_.y * 10 + coordinate_.x] = isSelfOrEnemy_;
            for (++y; y < coordinate_.y; ++y) {
                m_boards[y * 10 + coordinate_.x] = isSelfOrEnemy_;
            }
        }
    }

    // 下方向
    if (m_boards[(coordinate_.y + 1) * 10 + coordinate_.x] == prevStoneColor) {
        int32_t y{coordinate_.y + 2};
        bool isAllChanged{false};
        for (; !isAllChanged; ++y) {
            if (m_boards[y * 10 + coordinate_.x] == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[y * 10 + coordinate_.x] != isSelfOrEnemy_) {
                continue;
            }

            // 実際に石を置く
            m_boards[coordinate_.y * 10 + coordinate_.x] = isSelfOrEnemy_;
            for (--y; y > coordinate_.y; --y) {
                m_boards[y * 10 + coordinate_.x] = isSelfOrEnemy_;
            }
        }
    }

    // 右方向
    if (m_boards[coordinate_.y * 10 + coordinate_.x + 1] == prevStoneColor) {
        int32_t x{coordinate_.x + 2};
        bool isAllChanged{false};
        for (; !isAllChanged; ++x) {
            if (m_boards[coordinate_.y * 10 + x] == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[coordinate_.y * 10 + x] != isSelfOrEnemy_) {
                continue;
            }

            // 実際に石を置く
            m_boards[coordinate_.y * 10 + coordinate_.x] = isSelfOrEnemy_;
            for (--x; x > coordinate_.x; --x) {
                m_boards[coordinate_.y * 10 + x] = isSelfOrEnemy_;
            }
        }
    }

    // 左方向
    if (m_boards[coordinate_.y * 10 + coordinate_.x - 1] == prevStoneColor) {
        int32_t x{coordinate_.x - 2};
        bool isAllChanged{false};
        for (; !isAllChanged; --x) {
            if (m_boards[coordinate_.y * 10 + x] == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[coordinate_.y * 10 + x] != isSelfOrEnemy_) {
                continue;
            }

            // 実際に石を置く
            m_boards[coordinate_.y * 10 + coordinate_.x] = isSelfOrEnemy_;
            for (++x; x < coordinate_.x; ++x) {
                m_boards[coordinate_.y * 10 + x] = isSelfOrEnemy_;
            }
        }
    }

    // 右上
    if (m_boards[(coordinate_.y - 1) * 10 + coordinate_.x + 1] == prevStoneColor) {
        int32_t y{coordinate_.y - 2};
        int32_t x{coordinate_.x + 2};
        bool isAllChanged{false};
        for (; !isAllChanged; --y, ++x) {
            if (m_boards[y * 10 + x] == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[y * 10 + x] != isSelfOrEnemy_) {
                continue;
            }

            // 実際に石を置く
            m_boards[coordinate_.y * 10 + coordinate_.x] = isSelfOrEnemy_;
            for (++y, --x; y < coordinate_.y; ++y, --x) {
                m_boards[y * 10 + x] = isSelfOrEnemy_;
            }
        }
    }

    // 右下
    if (m_boards[(coordinate_.y + 1) * 10 + coordinate_.x + 1] == prevStoneColor) {
        int32_t y{coordinate_.y + 2};
        int32_t x{coordinate_.x + 2};
        bool isAllChanged{false};
        for (; !isAllChanged; ++y, ++x) {
            if (m_boards[y * 10 + x] == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[y * 10 + x] != isSelfOrEnemy_) {
                continue;
            }

            // 実際に石を置く
            m_boards[coordinate_.y * 10 + coordinate_.x] = isSelfOrEnemy_;
            for (--y, --x; y > coordinate_.y; --y, --x) {
                m_boards[y * 10 + x] = isSelfOrEnemy_;
            }
        }
    }

    // 左上
    if (m_boards[(coordinate_.y - 1) * 10 + coordinate_.x - 1] == prevStoneColor) {
        int32_t y{coordinate_.y - 2};
        int32_t x{coordinate_.x - 2};
        bool isAllChanged{false};
        for (; !isAllChanged; --y, --x) {
            if (m_boards[y * 10 + x] == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[y * 10 + x] != isSelfOrEnemy_) {
                continue;
            }

            // 実際に石を置く
            m_boards[coordinate_.y * 10 + coordinate_.x] = isSelfOrEnemy_;
            for (++y, ++x; y < coordinate_.y; ++y, ++x) {
                m_boards[y * 10 + x] = isSelfOrEnemy_;
            }
        }
    }

    // 左下
    if (m_boards[(coordinate_.y + 1) * 10 + coordinate_.x - 1] == prevStoneColor) {
        int32_t y{coordinate_.y + 2};
        int32_t x{coordinate_.x - 2};
        bool isAllChanged{false};
        for (; !isAllChanged; ++y, --x) {
            if (m_boards[y * 10 + x] == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[y * 10 + x] != isSelfOrEnemy_) {
                continue;
            }

            // 実際に石を置く
            m_boards[coordinate_.y * 10 + coordinate_.x] = isSelfOrEnemy_;
            for (--y, ++x; y > coordinate_.y; --y, ++x) {
                m_boards[y * 10 + x] = isSelfOrEnemy_;
            }
        }
    }
}

uint32_t bnsGame::Aspect::MakeLegalPuts(const int32_t isSelfOrEnemy_) {
    m_stoneValids.clear();

    for (int32_t retu{1}; retu <= 8; ++retu) {
        for (int32_t gyo{1}; gyo <= 8; ++gyo) {
            const int32_t prevStoneColor = ((isSelfOrEnemy_ == Self) ? Enemy : Self);
            bool isPushed{false};

            if (m_boards[retu * 10 + gyo] != StoneType::None) {
                continue;
            }
            // 上方向
            if (m_boards[(retu - 1) * 10 + gyo] == prevStoneColor) {
                int32_t y{retu - 2};
                bool isAllChanged{false};
                for (; !isAllChanged; --y) {
                    if (m_boards[y * 10 + gyo] == prevStoneColor) {
                        continue;
                    }

                    isAllChanged = true;
                    if (m_boards[y * 10 + gyo] != isSelfOrEnemy_) {
                        continue;
                    }

                    m_stoneValids.emplace_back(gyo, retu);
                    isPushed = true;
                }
                if (isPushed) {
                    continue;
                }
            }

            // 下方向
            if (m_boards[(retu + 1) * 10 + gyo] == prevStoneColor) {
                int32_t y{retu + 2};
                bool isAllChanged{false};
                for (; !isAllChanged; ++y) {
                    if (m_boards[y * 10 + gyo] == prevStoneColor) {
                        continue;
                    }

                    isAllChanged = true;
                    if (m_boards[y * 10 + gyo] != isSelfOrEnemy_) {
                        continue;
                    }

                    m_stoneValids.emplace_back(gyo, retu);
                    isPushed = true;
                }
                if (isPushed) {
                    continue;
                }
            }

            // 右方向
            if (m_boards[retu * 10 + gyo + 1] == prevStoneColor) {
                int32_t x{gyo + 2};
                bool isAllChanged{false};
                for (; !isAllChanged; ++x) {
                    if (m_boards[retu * 10 + x] == prevStoneColor) {
                        continue;
                    }

                    isAllChanged = true;
                    if (m_boards[retu * 10 + x] != isSelfOrEnemy_) {
                        continue;
                    }

                    m_stoneValids.emplace_back(gyo, retu);
                    isPushed = true;
                }
                if (isPushed) {
                    continue;
                }
            }

            // 左方向
            if (m_boards[retu * 10 + gyo - 1] == prevStoneColor) {
                int32_t x{gyo - 2};
                bool isAllChanged{false};
                for (; !isAllChanged; --x) {
                    if (m_boards[retu * 10 + x] == prevStoneColor) {
                        continue;
                    }

                    isAllChanged = true;
                    if (m_boards[retu * 10 + x] != isSelfOrEnemy_) {
                        continue;
                    }

                    m_stoneValids.emplace_back(gyo, retu);
                    isPushed = true;
                }
                if (isPushed) {
                    continue;
                }
            }

            // 右上
            if (m_boards[(retu - 1) * 10 + gyo + 1] == prevStoneColor) {
                int32_t y{retu - 2};
                int32_t x{gyo + 2};
                bool isAllChanged{false};
                for (; !isAllChanged; --y, ++x) {
                    if (m_boards[y * 10 + x] == prevStoneColor) {
                        continue;
                    }

                    isAllChanged = true;
                    if (m_boards[y * 10 + x] != isSelfOrEnemy_) {
                        continue;
                    }

                    m_stoneValids.emplace_back(gyo, retu);
                    isPushed = true;
                }
                if (isPushed) {
                    continue;
                }
            }

            // 右下
            if (m_boards[(retu + 1) * 10 + gyo + 1] == prevStoneColor) {
                int32_t y{retu + 2};
                int32_t x{gyo + 2};
                bool isAllChanged{false};
                for (; !isAllChanged; ++y, ++x) {
                    if (m_boards[y * 10 + x] == prevStoneColor) {
                        continue;
                    }

                    isAllChanged = true;
                    if (m_boards[y * 10 + x] != isSelfOrEnemy_) {
                        continue;
                    }

                    m_stoneValids.emplace_back(gyo, retu);
                    isPushed = true;
                }
                if (isPushed) {
                    continue;
                }
            }

            // 左上
            if (m_boards[(retu - 1) * 10 + gyo - 1] == prevStoneColor) {
                int32_t y{retu - 2};
                int32_t x{gyo - 2};
                bool isAllChanged{false};
                for (; !isAllChanged; --y, --x) {
                    if (m_boards[y * 10 + x] == prevStoneColor) {
                        continue;
                    }

                    isAllChanged = true;
                    if (m_boards[y * 10 + x] != isSelfOrEnemy_) {
                        continue;
                    }

                    m_stoneValids.emplace_back(gyo, retu);
                    isPushed = true;
                }
                if (isPushed) {
                    continue;
                }
            }

            // 左下
            if (m_boards[(retu + 1) * 10 + gyo - 1] == prevStoneColor) {
                int32_t y{retu + 2};
                int32_t x{gyo - 2};
                bool isAllChanged{false};
                for (; !isAllChanged; ++y, --x) {
                    if (m_boards[y * 10 + x] == prevStoneColor) {
                        continue;
                    }

                    isAllChanged = true;
                    if (m_boards[y * 10 + x] != isSelfOrEnemy_) {
                        continue;
                    }

                    m_stoneValids.emplace_back(gyo, retu);
                }
            }
        }
    }

    return static_cast<uint32_t>(m_stoneValids.size());
}
