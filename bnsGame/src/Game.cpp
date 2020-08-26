#include "Game.hpp"
#include <cstdio>
#include <conio.h>
#include <Windows.h>
#include <thread>

bnsGame::Game::Game(const InitData& init_)
    : IScene(init_)
    , m_displayBuffer()
    , m_boards()
    , m_stonePutPos(5, 0)
    , m_turn(Turn::Player)
    , m_aspect(((m_turn == Turn::Player) ? Self : Enemy))
    , m_playerColor((m_turn == Turn::Player) ? StoneState::Black : StoneState::White)
    , m_sikou(getData().GetValues(), false)
    , m_testSikou(true) {
    ClearDisplay();

    for (uint32_t y{ 1 }; y <= 8; ++y) {
        for (uint32_t x{ 1 }; x <= 8; ++x) {
            m_boards[y][x].GoToState(StoneState::None);
        }
    }
    
    m_boards[4][4].GoToState(StoneState::White);
    m_boards[5][5].GoToState(StoneState::White);
    m_boards[5][4].GoToState(StoneState::Black);
    m_boards[4][5].GoToState(StoneState::Black);

    int32_t turn = (m_turn == Turn::Player) ? Self : Enemy;
    if (m_aspect.MakeLegalPuts(turn) <= 0) {
        m_turn = Turn::Result;
    }

    Draw();
}

void bnsGame::Game::Input() {
    std::cout << "座標を入力してください。 : ";
    std::cin >> m_inputString;

    if (m_inputString == "e") {
        ExitGame();
        return;
    }

    if (m_inputString == "y") {
        ChangeScene(bnsGame::Scene::Title);
        return;
    }
}

bool bnsGame::Game::ChangeStones(const StoneState & stone_) {
    if (m_boards[m_stonePutPos.y][m_stonePutPos.x].GetCurrentState() != StoneState::None) {
        return false;
    }

    bool ret{false};
    const auto prevStoneColor = ((stone_ == StoneState::Black) ? StoneState::White : StoneState::Black);
    // 上方向
    if (m_boards[m_stonePutPos.y - 1][m_stonePutPos.x].GetCurrentState() == prevStoneColor) {
        int32_t y{m_stonePutPos.y - 2};
        bool isAllChanged{false};
        for (; !isAllChanged; --y) {
            if (m_boards[y][m_stonePutPos.x].GetCurrentState() == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[y][m_stonePutPos.x].GetCurrentState() != stone_) {
                continue;
            }

            // 実際に石を置く
            m_boards[m_stonePutPos.y][m_stonePutPos.x].GoToState(stone_);
            for (++y; y < m_stonePutPos.y; ++y) {
                m_boards[y][m_stonePutPos.x].GoToState(stone_);
            }
            ret = true;
        }
    }

    // 下方向
    if (m_boards[m_stonePutPos.y + 1][m_stonePutPos.x].GetCurrentState() == prevStoneColor) {
        int32_t y{m_stonePutPos.y + 2};
        bool isAllChanged{false};
        for (; !isAllChanged; ++y) {
            if (m_boards[y][m_stonePutPos.x].GetCurrentState() == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[y][m_stonePutPos.x].GetCurrentState() != stone_) {
                continue;
            }

            // 実際に石を置く
            m_boards[m_stonePutPos.y][m_stonePutPos.x].GoToState(stone_);
            for (--y; y > m_stonePutPos.y; --y) {
                m_boards[y][m_stonePutPos.x].GoToState(stone_);
            }
            ret = true;
        }
    }

    // 右方向
    if (m_boards[m_stonePutPos.y][m_stonePutPos.x + 1].GetCurrentState() == prevStoneColor) {
        int32_t x{m_stonePutPos.x + 2};
        bool isAllChanged{false};
        for (; !isAllChanged; ++x) {
            if (m_boards[m_stonePutPos.y][x].GetCurrentState() == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[m_stonePutPos.y][x].GetCurrentState() != stone_) {
                continue;
            }

            // 実際に石を置く
            m_boards[m_stonePutPos.y][m_stonePutPos.x].GoToState(stone_);
            for (--x; x > m_stonePutPos.x; --x) {
                m_boards[m_stonePutPos.y][x].GoToState(stone_);
            }
            ret = true;
        }
    }

    // 左方向
    if (m_boards[m_stonePutPos.y][m_stonePutPos.x - 1].GetCurrentState() == prevStoneColor) {
        int32_t x{m_stonePutPos.x - 2};
        bool isAllChanged{false};
        for (; !isAllChanged; --x) {
            if (m_boards[m_stonePutPos.y][x].GetCurrentState() == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[m_stonePutPos.y][x].GetCurrentState() != stone_) {
                continue;
            }

            // 実際に石を置く
            m_boards[m_stonePutPos.y][m_stonePutPos.x].GoToState(stone_);
            for (++x; x < m_stonePutPos.x; ++x) {
                m_boards[m_stonePutPos.y][x].GoToState(stone_);
            }
            ret = true;
        }
    }

    // 右上
    if (m_boards[m_stonePutPos.y - 1][m_stonePutPos.x + 1].GetCurrentState() == prevStoneColor) {
        int32_t y{m_stonePutPos.y - 2};
        int32_t x{m_stonePutPos.x + 2};
        bool isAllChanged{false};
        for (; !isAllChanged; --y, ++x) {
            if (m_boards[y][x].GetCurrentState() == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[y][x].GetCurrentState() != stone_) {
                continue;
            }

            // 実際に石を置く
            m_boards[m_stonePutPos.y][m_stonePutPos.x].GoToState(stone_);
            for (++y, --x; y < m_stonePutPos.y; ++y, --x) {
                m_boards[y][x].GoToState(stone_);
            }
            ret = true;
        }
    }

    // 右下
    if (m_boards[m_stonePutPos.y + 1][m_stonePutPos.x + 1].GetCurrentState() == prevStoneColor) {
        int32_t y{m_stonePutPos.y + 2};
        int32_t x{m_stonePutPos.x + 2};
        bool isAllChanged{false};
        for (; !isAllChanged; ++y, ++x) {
            if (m_boards[y][x].GetCurrentState() == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[y][x].GetCurrentState() != stone_) {
                continue;
            }

            // 実際に石を置く
            m_boards[m_stonePutPos.y][m_stonePutPos.x].GoToState(stone_);
            for (--y, --x; y > m_stonePutPos.y; --y, --x) {
                m_boards[y][x].GoToState(stone_);
            }
            ret = true;
        }
    }

    // 左上
    if (m_boards[m_stonePutPos.y - 1][m_stonePutPos.x - 1].GetCurrentState() == prevStoneColor) {
        int32_t y{m_stonePutPos.y - 2};
        int32_t x{m_stonePutPos.x - 2};
        bool isAllChanged{false};
        for (; !isAllChanged; --y, --x) {
            if (m_boards[y][x].GetCurrentState() == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[y][x].GetCurrentState() != stone_) {
                continue;
            }

            // 実際に石を置く
            m_boards[m_stonePutPos.y][m_stonePutPos.x].GoToState(stone_);
            for (++y, ++x; y < m_stonePutPos.y; ++y, ++x) {
                m_boards[y][x].GoToState(stone_);
            }
            ret = true;
        }
    }

    // 左下
    if (m_boards[m_stonePutPos.y + 1][m_stonePutPos.x - 1].GetCurrentState() == prevStoneColor) {
        int32_t y{m_stonePutPos.y + 2};
        int32_t x{m_stonePutPos.x - 2};
        bool isAllChanged{false};
        for (; !isAllChanged; ++y, --x) {
            if (m_boards[y][x].GetCurrentState() == prevStoneColor) {
                continue;
            }

            isAllChanged = true;
            if (m_boards[y][x].GetCurrentState() != stone_) {
                continue;
            }

            // 実際に石を置く
            m_boards[m_stonePutPos.y][m_stonePutPos.x].GoToState(stone_);
            for (--y, ++x; y > m_stonePutPos.y; --y, ++x) {
                m_boards[y][x].GoToState(stone_);
            }
            ret = true;
        }
    }

    return ret;
}

void bnsGame::Game::PlayerTurn() {
    Input();

    if (m_inputString.size() != 2) {
        return;
    }

    m_stonePutPos.Set(m_inputString[0] - '0', m_inputString[1] - '0');

    if ((m_stonePutPos.x < 1 || m_stonePutPos.x > 8) || (m_stonePutPos.y < 1 || m_stonePutPos.y > 8)) {
        return;
    }

    m_aspect.Put(Self, m_stonePutPos, getData().GetValues());
    if (ChangeStones(m_playerColor)) {
        ChangeTurn();
    }
}

void bnsGame::Game::PlayerAITurn() {
    m_stonePutPos.Set(m_testSikou.Think(Self, m_aspect));
    m_aspect.Put(Self, m_stonePutPos, m_testSikou.GetValues());

    if (ChangeStones(m_playerColor)) {
        ChangeTurn();
    }
}

void bnsGame::Game::EnemyTurn() {
    m_stonePutPos.Set(m_sikou.Think(Enemy, m_aspect));
    m_aspect.Put(Enemy, m_stonePutPos, getData().GetValues());

    auto enemyColor = ((m_playerColor == StoneState::Black) ? StoneState::White : StoneState::Black);
    if (ChangeStones(enemyColor)) {
        ChangeTurn();
    }
}

void bnsGame::Game::Result() {
    auto result = m_aspect.GetWinner();

    std::cout << "エンターキーを押してください。";
    
    std::string s;
    std::getline(std::cin, s);

    ChangeScene(Scene::Title);
}

void bnsGame::Game::Update() {
    switch (m_turn)
	{
    case bnsGame::Turn::Player: {
        /*std::thread th(&bnsGame::Game::PlayerAITurn, this);
        Sleep(100);
        th.join();*/
        PlayerTurn();
        break;
    }
    case bnsGame::Turn::Enemy: {
        std::thread th(&bnsGame::Game::EnemyTurn, this);
        Sleep(800);
        th.join();
        break;
    }
    case bnsGame::Turn::Result:
        Result();
        break;
    default:
        break;
	}
}

void bnsGame::Game::UpdateChangeOut() {
}

void bnsGame::Game::Draw() const {
    ClearDisplay();
    
    std::cout << " 1 2 3 4 5 6 7 8" << std::endl;
    uint32_t num{1};
    for (uint32_t y{ 1 }; y <= 8; ++y) {
        std::cout << num;
        for (uint32_t x{ 1 }; x <= 8; ++x) {
            const bool isSelect = (m_stonePutPos == utl::Point(x, y));
            m_boards[y][x].Draw(isSelect);
        }
        std::cout << std::endl;
        ++num;
    }
}
