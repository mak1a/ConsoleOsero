#include "Study.hpp"
#include <thread>
#include <Windows.h>
#include <string>

bnsGame::Study::Study(const InitData& init_) : IScene(init_)
, m_turn(Turn::Player)
, m_aspect(((m_turn == Turn::Player) ? Self : Enemy))
, m_playerIndex(0)
, m_enemyIndex(1)
, m_state(StudyState::Studying) {
    ClearDisplay();

    for (uint32_t i{}; i < 6; ++i) {
        m_winNums.emplace_back(static_cast<uint32_t>(0), i);
    }

    m_sikous.emplace_back(true);
    for (uint32_t i{}; i < 5; ++i) {
        m_sikous.emplace_back();
    }

    int32_t turn = (m_turn == Turn::Player) ? Self : Enemy;
    if (m_aspect.MakeLegalPuts(turn) <= 0) {
        m_turn = Turn::Result;
    }

    std::cout << "プレイヤー番号 : " << m_playerIndex << ", 敵番号 : " << m_enemyIndex;
}

void bnsGame::Study::Init(const Turn& turn_) {
    m_turn = turn_;
    m_aspect = Aspect((m_turn == Turn::Player) ? Self : Enemy);
    
    int32_t turn = (m_turn == Turn::Player) ? Self : Enemy;
    if (m_aspect.MakeLegalPuts(turn) <= 0) {
        m_turn = Turn::Result;
    }

    std::cout << "プレイヤー番号 : " << m_playerIndex << ", 敵番号 : " << m_enemyIndex;
}

void bnsGame::Study::Mating() {
    std::vector<Sikou> children;

    // 優秀な二人の遺伝子はクローンする
    {
        std::sort(m_winNums.rbegin(), m_winNums.rend());
        children.emplace_back(m_sikous[m_winNums[0].second]);
        children.emplace_back(m_sikous[m_winNums[1].second]);
    }

    // 優秀な二人の遺伝子を交配させる
    {
        std::array<std::array<int32_t, 10>, 10> child_1, child_2;

        uint32_t separateX{utl::RandomUint32(1, 8)};
        uint32_t separateY{utl::RandomUint32(1, 8)};

        for (uint32_t y{1}; y < separateY; ++y) {
            for (uint32_t x{1}; x <= 8; ++x) {
                child_1[y][x] = m_sikous[m_winNums[0].second].GetValue(x, y);
                child_2[y][x] = m_sikous[m_winNums[1].second].GetValue(x, y);
            }
        }

        for (uint32_t x{1}; x < separateX; ++x) {
            child_1[separateY][x] = m_sikous[m_winNums[0].second].GetValue(x, separateY);
            child_2[separateY][x] = m_sikous[m_winNums[1].second].GetValue(x, separateY);
        }
        for (uint32_t x{separateX}; x <= 8; ++x) {
            child_1[separateY][x] = m_sikous[m_winNums[1].second].GetValue(x, separateY);
            child_2[separateY][x] = m_sikous[m_winNums[0].second].GetValue(x, separateY);
        }

        for (uint32_t y{separateY + 1}; y <= 8; ++y) {
            for (uint32_t x{1}; x <= 8; ++x) {
                child_1[y][x] = m_sikous[m_winNums[1].second].GetValue(x, y);
                child_2[y][x] = m_sikous[m_winNums[0].second].GetValue(x, y);
            }
        }

        children.emplace_back(child_1);
        children.emplace_back(child_2);
    }

    // ランダムに選んだ二人を交配させる
    {
        std::vector<Sikou> randomParents;
        randomParents.emplace_back(m_sikous[utl::RandomUint32(0, 5)]);
        randomParents.emplace_back(m_sikous[utl::RandomUint32(0, 5)]);

        std::array<std::array<int32_t, 10>, 10> child_3, child_4;

        uint32_t separateX{utl::RandomUint32(1, 8)};
        uint32_t separateY{utl::RandomUint32(1, 8)};

        for (uint32_t y{1}; y < separateY; ++y) {
            for (uint32_t x{1}; x <= 8; ++x) {
                child_3[y][x] = randomParents[0].GetValue(x, y);
                child_4[y][x] = randomParents[1].GetValue(x, y);
            }
        }

        for (uint32_t x{1}; x < separateX; ++x) {
            child_3[separateY][x] = randomParents[0].GetValue(x, separateY);
            child_4[separateY][x] = randomParents[1].GetValue(x, separateY);
        }
        for (uint32_t x{separateX}; x <= 8; ++x) {
            child_3[separateY][x] = randomParents[1].GetValue(x, separateY);
            child_4[separateY][x] = randomParents[0].GetValue(x, separateY);
        }

        for (uint32_t y{separateY + 1}; y <= 8; ++y) {
            for (uint32_t x{1}; x <= 8; ++x) {
                child_3[y][x] = randomParents[1].GetValue(x, y);
                child_4[y][x] = randomParents[0].GetValue(x, y);
            }
        }

        children.emplace_back(child_3);
        children.emplace_back(child_4);
    }

    m_sikous = children;
    for (auto& winNum : m_winNums) {
        winNum.first = 0;
    }
    Init(Turn::Player);
}

void bnsGame::Study::PlayerTurn() {
    m_aspect.Put(Self, m_sikous[m_playerIndex].Think(Self, m_aspect));

    ChangeTurn();
}

void bnsGame::Study::EnemyTurn() {
    m_aspect.Put(Enemy, m_sikous[m_enemyIndex].Think(Enemy, m_aspect));

    ChangeTurn();
}

void bnsGame::Study::Result() {
    std::cout << "おっけーーーーーーーーいうぇ～～～い！！！" << std::endl;
    const auto winner = m_aspect.GetWinner();
    if (winner == Winner::Player) {
        ++m_winNums[m_playerIndex].first;
    }
    else if (winner == Winner::Enemy) {
        ++m_winNums[m_enemyIndex].first;
    }

    if (m_enemyIndex == 5) {
        m_enemyIndex = 0;
        ++m_playerIndex;
        Init(Turn::Player);
        return;
    }

    ++m_enemyIndex;
    if (m_playerIndex == m_enemyIndex) {
        if (m_enemyIndex == 5) {
            m_state = StudyState::Mating;
            m_enemyIndex = 0;
            m_playerIndex = 0;

            std::cout << "各インデックスの勝利数は、" << std::endl;
            for (const auto winNum : m_winNums) {
                std::cout << winNum.first << " ";
            }
            std::cout << "です。" << std::endl;
            std::string s;
            std::cin >> s;

            if (s == "e") {
                for (const auto& sikou : m_sikous) {
                    sikou.Print();
                    std::cout << std::endl;
                }
                ExitGame();
                return;
            }

            Mating();
            return;
        }
        ++m_enemyIndex;
        Init(Turn::Player);
        return;
    }

    Init(Turn::Player);
    return;
}

void bnsGame::Study::Update() {
    switch (m_turn) {
    case bnsGame::Turn::Player: {
        std::thread th(&bnsGame::Study::PlayerTurn, this);
        Sleep(0);
        // EnemyTurn();
        th.join();
        break;
    }
    case bnsGame::Turn::Enemy: {
        std::thread th(&bnsGame::Study::EnemyTurn, this);
        Sleep(0);
        // EnemyTurn();
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

void bnsGame::Study::UpdateChangeOut() {}

void bnsGame::Study::Draw() const {}
