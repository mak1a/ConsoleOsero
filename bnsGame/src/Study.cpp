#include "Study.hpp"
#include <thread>
#include <Windows.h>
#include <string>

bnsGame::Study::Study(const InitData& init_) : IScene(init_)
, m_turn(Turn::Player)
, m_aspect(((m_turn == Turn::Player) ? Self : Enemy))
, m_winNums(6, 0), m_playerIndex(0)
, m_enemyIndex(1)
, m_state(StudyState::Studying) {
    ClearDisplay();

    m_sikou.emplace_back(true);
    for (uint32_t i{}; i < 5; ++i) {
        m_sikou.emplace_back();
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

void bnsGame::Study::PlayerTurn() {
    m_aspect.Put(Self, m_sikou[m_playerIndex].Think(Self, m_aspect));

    ChangeTurn();
}

void bnsGame::Study::EnemyTurn() {
    m_aspect.Put(Enemy, m_sikou[m_enemyIndex].Think(Enemy, m_aspect));

    ChangeTurn();
}

void bnsGame::Study::Result() {
    std::cout << "おっけーーーーーーーーいうぇ～～～い！！！" << std::endl;
    const auto winner = m_aspect.GetWinner();
    if (winner == Winner::Player) {
        ++m_winNums[m_playerIndex];
    }
    else if (winner == Winner::Enemy) {
        ++m_winNums[m_enemyIndex];
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
                std::cout << winNum << " ";
            }
            std::cout << "です。" << std::endl;
            std::string s;
            std::getline(std::cin, s);
            ExitGame();
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
